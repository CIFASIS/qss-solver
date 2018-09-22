/*****************************************************************************

    This file is part of Modelica C Compiler.

    Modelica C Compiler is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Modelica C Compiler is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Modelica C Compiler.  If not, see <http://www.gnu.org/licenses/>.

******************************************************************************/

#include <causalize/vector/graph_builder.h>
#include <causalize/vector/vector_graph_definition.h>
#include <causalize/vector/contains_vector.h>
#include <util/ast_visitors/eval_expression.h>


#include <boost/graph/adjacency_list.hpp>
#include <boost/icl/interval_set.hpp>
#include <mmo/mmo_class.h>
#include <util/debug.h>
#include <ast/queries.h>

//#ifdef ENABLE_DEBUG_MSG
//#define DEBUG_MSG(str) do {std::cout << str << std::endl;} while( false )
//#else
//#define DEBUG_MSG(str) do {} while( false )
//#endif
#define DEBUG_MSG(str) do {std::cout << str << std::endl;} while( false )

using namespace std;
using namespace boost;
using namespace boost::icl;
using namespace Modelica;
using namespace Modelica::AST;

namespace Causalize { 
ReducedGraphBuilder::ReducedGraphBuilder(MMO_Class &mmo_cl): mmo_class(mmo_cl), state_finder(mmo_cl) {
}

VectorCausalizationGraph ReducedGraphBuilder::makeGraph() {
  /* Create nodes for the Equations*/
  foreach_ (Equation e, mmo_class.equations().equations()) {
    static int index = 0;
    VectorVertexProperty vp;
    vp.type = E;
    vp.equation = e;
    if (is<ForEq>(e)) {
      vp.count=getForRangeSize(get<ForEq>(e));
    } else if (is<Equality>(e)) {
      vp.count=1;
    } else {
      ERROR("Only causalization of for and equality equations");
    }
    vp.index=index++;
    equationDescriptorList.push_back(add_vertex(vp,graph));
  }
 /* Create nodes for the unknowns: We iterate through the VarSymbolTable
  * and create one vertex per unknown */
  state_finder.findStateVariables();
  foreach_ (Name var, mmo_class.variables()) {
    static int index = 0;
    const VarSymbolTable &syms = mmo_class.syms_ref();
    VarInfo varInfo = syms[var].get();
    if (!isConstant(var,syms) && !isBuiltIn(var,syms) && !isDiscrete(var,syms) && !isParameter(var,syms)) {
      VectorVertexProperty vp;
      vp.type=U;
      vp.index=index++;
      vp.unknown = Unknown(varInfo, var);
      if ("Real"==varInfo.type()) {
        if (!varInfo.indices())  {
          vp.count=1;
        } else if (varInfo.indices().get().size()==1) {
          EvalExpression ev(mmo_class.syms_ref()); 
          vp.count = Apply(ev,varInfo.indices().get().front());
        } else {
          ERROR("ReducedGraphBuilder::makeGraph Arrays of arrays are not supported yet\n");  
        }
      }
      unknownDescriptorList.push_back(add_vertex(vp, graph));
    }
  }
   if(debugIsEnabled('c')){
     DEBUG_MSG("Equations");
     foreach_ (VectorEquationVertex eq, equationDescriptorList){
       DEBUG_MSG(graph[eq].index << ": " << graph[eq].equation) ;
     }
     DEBUG_MSG("Unknowns");
     foreach_(VectorUnknownVertex un, unknownDescriptorList){
       DEBUG_MSG(graph[un].index << ": " << graph[un].unknown()) ;
     }
   }


  foreach_ (VectorEquationVertex eq, equationDescriptorList){
    foreach_(VectorUnknownVertex un, unknownDescriptorList){
      Expression unknown = graph[un].unknown();
      VarSymbolTable syms = mmo_class.syms_ref();
      Equation e = graph[eq].equation;
      if (is<Equality>(e)) {
        Causalize::ContainsVector occurrs(unknown, graph[un], syms);
        Equality eqq = boost::get<Equality>(e);
        std::cerr << "Checking var: " << unknown ;
        std::cerr << eqq.left_ref() << "***********\n";
        const bool rl = Apply(occurrs,eqq.left_ref());
        std::cerr << eqq.right_ref() << "***********\n";
        const bool rr = Apply(occurrs,eqq.right_ref()); 
        std::cerr << "Result: " << rl << " " << rr << "\n";
        if(rl || rr) {
          VectorEdgeProperty ep;
          ep.labels = occurrs.getOccurrenceIndexes();
          add_edge(eq, un, ep, graph);
          DEBUG('c', "(%d, %d) ", graph[eq].index, graph[un].index);
        } 
      } else if (is<ForEq>(e)) {
        ForEq feq = get<ForEq>(e);
        ERROR_UNLESS(feq.elements().size()==1, "For equation with more than one equation not supported");
        Equation inside = feq.elements().front();
        ERROR_UNLESS(is<Equality>(inside), "Only equality equation inside for loops supported");
        Equality eqq = boost::get<Equality>(inside);
        IndexList ind = feq.range().indexes();
        ERROR_UNLESS(ind.size() == 1, "graph_builder:\n For Loop with more than one index is not supported yet\n");
        Index i = ind.front();
        ERROR_UNLESS((bool)i.exp(), "graph_builder:\n No expression on for equation");
        Expression exp = i.exp().get();
        ERROR_UNLESS(is<Range>(exp), "Only range expression in for equations");
        Range range = get<Range>(exp);
        ERROR_UNLESS(!range.step(), "Range with step not supported");

        VarSymbolTable syms_for = mmo_class.syms_ref();
        syms_for.insert(i.name(),VarInfo(TypePrefixes(0),"Integer"));
        Causalize::ContainsVector occurrs_for(graph[un], syms_for, ind);
        
        const bool rl = Apply(occurrs_for,eqq.left_ref());
        const bool rr = Apply(occurrs_for,eqq.right_ref()); 
        if(rl || rr) {
          VectorEdgeProperty ep;
          ep.labels = occurrs_for.getOccurrenceIndexes();
          add_edge(eq, un, ep, graph);
          DEBUG('c', "(%d, %d) ", graph[eq].index, graph[un].index);
        } 
      } else
        ERROR_UNLESS(is<Equality>(e), "Only causalization of equality and for equation is supported");
    }
  }
  DEBUG('c', "\n");
  return graph;
}


int ReducedGraphBuilder::getForRangeSize(ForEq feq) {
  IndexList ind = feq.range().indexes();
  ERROR_UNLESS(ind.size() == 1, "graph_builder:\n For Loop with more than one index is not supported yet\n");
  Index i = ind.front();
  ERROR_UNLESS((bool)i.exp(), "graph_builder:\n No expression on for equation");
  Expression exp = i.exp().get();
  if (is<Brace>(exp))
    return get<Brace>(exp).args().size();
  if (is<Range>(exp)) {
    Range range = get<Range>(exp);
    ERROR_UNLESS(!range.step(), "graph_builder: FOR ranges with leaps not supported yet");
    EvalExpression ev(mmo_class.syms_ref()); 
    return Apply(ev,range.end_ref())-Apply(ev,range.start_ref())+1;
  }
  ERROR("Expression in FOR Index not supported\n");
  return 0;
}
}
