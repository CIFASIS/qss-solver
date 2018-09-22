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

#include <causalize/vector/causalization_algorithm.h>
#include <causalize/vector/vector_graph_definition.h>
#define HAS_COUNT
#include <causalize/graph/graph_printer.h>
#include <causalize/for_unrolling/process_for_equations.h>
#include <util/debug.h>
#include <util/solve/solve.h>
#include <boost/tuple/tuple.hpp>

#include <boost/graph/adjacency_list.hpp>
#include <boost/variant/get.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/icl/discrete_interval.hpp>
#include <sstream>
#include <mmo/mmo_class.h>
#include <ast/queries.h>

#define sz(a) int((a).size())

using namespace Modelica;
using namespace std;
using namespace boost;
using namespace boost::icl;

namespace Causalize {
CausalizationStrategyVector::CausalizationStrategyVector(VectorCausalizationGraph g, MMO_Class &m): mmo(m){
	graph = g;
  step = 0;
	VectorCausalizationGraph::vertex_iterator vi, vi_end;
	equationNumber = unknownNumber = 0;
	for(boost::tie(vi, vi_end) = vertices(graph); vi != vi_end; vi++){
		VectorVertex current_element = *vi;
		if(graph[current_element].type == E){
      equationNumber += graph[current_element].count;
			equationDescriptors.push_back(current_element);
		}
		else{
      unknownNumber += graph[current_element].count;
			unknownDescriptors.push_back(current_element);
		}
	}
	DEBUG('c', "Number of equations %d\n"
	           "Number of unknowns %d\n", 
			   equationNumber, unknownNumber);
	
	if(equationNumber != unknownNumber){
		ERROR("The model being causalized is not balanced.\n"
			  "There are %d equations and %d variables\n", 
			  equationNumber, unknownNumber);		
	}

  stringstream ss;
  ss << "initial_graph.dot";
  GraphPrinter<VectorVertexProperty,VectorEdgeProperty>  gp(graph);
  gp.printGraph(ss.str());
}


void 
CausalizationStrategyVector::Causalize1toN(const Unknown unk, const Equation eq, const IndexPairSet ips){
	CausalizedVar c_var;
	c_var.unknown = unk;
	c_var.equation = eq;
	c_var.pairs = ips;
	equations1toN.push_back(c_var);
}

void 
CausalizationStrategyVector::CausalizeNto1(const Unknown unk, const Equation eq, const IndexPairSet ips){
	CausalizedVar c_var;
	c_var.unknown = unk;
	c_var.equation = eq;
	c_var.pairs = ips;
  equationsNto1.insert(equationsNto1.begin(), c_var);		
}

bool
CausalizationStrategyVector::Causalize() {	
  while(true){
    bool causalize_some=false;
    assert(equationNumber == unknownNumber);
    if(equationDescriptors.empty() && unknownDescriptors.empty()) {
      // Finished causalizing :)
      SolveEquations();
      return true;
    }

    list<VectorVertex>::size_type numAcausalEqs = equationDescriptors.size();
    list<VectorVertex>::iterator iter, auxiliaryIter;


    //First, we process the equations' side
    auxiliaryIter = equationDescriptors.begin();
    for(iter = auxiliaryIter; iter != equationDescriptors.end(); iter = auxiliaryIter){
      // Additional iterator to erase while traversing
      auxiliaryIter++;
      EquationVertex eq = *iter;
      ERROR_UNLESS(out_degree(eq, graph) != 0, "Problem is singular, not supported yet\n");
      // Try to look for a set of indexes to causalize
      Option<std::pair<VectorEdge,IndexPairSet> > op = CanCausalizeEquation(eq);
      // If we can causalize something
      if (op) {
        // We are going to causalize something
        causalize_some=true;
        // This pair holds which edge(the first component) to use for causalization and which indexes(the second component)
        std::pair<VectorEdge,IndexPairSet> causal_pair = op.get();
        VectorEdge e = causal_pair.first;
        // This is the unknown node connecting to the edge
        UnknownVertex unk = GetUnknown(e);
        equationNumber--;
        unknownNumber--;
        // Save the result of this step of causalization
        Causalize1toN(graph[unk].unknown, graph[eq].equation, causal_pair.second);
        // Update the pairs in the edge that is being causalized
        graph[e].RemovePairs(causal_pair.second);
        // Decrement the number of uncauzalized equations/unknowns
        graph[eq].count -= causal_pair.second.size();
        graph[unk].count -= causal_pair.second.size();
        // If the edge has no more pairs in it remove it
        if (graph[e].IsEmpty()) {
          remove_edge(e, graph);
        }
        // Auxiliary list to later remove empty edges
        std::list<VectorEdge> remove;
        foreach_(VectorEdge e1, out_edges(unk,graph)) {
          // Update the labels from all the edges adjacent to the unknown
          graph[e1].RemoveUnknowns(causal_pair.second);
          // If the edge is now empty schedule it for removal
          if (graph[e1].IsEmpty()) {
            remove.push_back(e1);
          }
        }
        // Now remove all scheduled edges
        foreach_(VectorEdge e1, remove) {
          WARNING_UNLESS(out_degree(GetEquation(e1),graph)>1, "Disconnecting equation node");
          remove_edge(e1, graph);
        }
        // If the equation node is now unconnected and with count==0 we can remove it
        if (out_degree(eq,graph)==0) {
          ERROR_UNLESS(graph[eq].count==0, "Disconnected node with uncausalized equations");
          remove_vertex(eq,graph);
          equationDescriptors.erase(iter);
        }
        // If the unknown node is now unconnected and with count==0 we can remove it
        if (out_degree(unk,graph)==0) {
          ERROR_UNLESS(graph[unk].count==0, "Disconnected node with uncausalized unknowns");
          remove_vertex(unk,graph);
          unknownDescriptors.remove(unk);
        }
        stringstream ss;
        ss << "graph_" << step++ << ".dot";
        GraphPrinter<VectorVertexProperty,VectorEdgeProperty>  gp(graph);
        gp.printGraph(ss.str());
      }
    }

  
    //Now, we process the unknowns' side
    auxiliaryIter = unknownDescriptors.begin();
    for(iter = auxiliaryIter; iter != unknownDescriptors.end(); iter = auxiliaryIter){
      // Additional iterator to erase while traversing
      auxiliaryIter++;
      UnknownVertex unk = *iter;
      ERROR_UNLESS(out_degree(unk, graph) != 0, "Problem is singular, not supported yet\n");
      // Try to look for a set of indexes to causalize
      Option<std::pair<VectorEdge,IndexPairSet> > op = CanCausalizeUnknown(unk);
      // If we can causalize something
      if (op) {
        // We are going to causalize something
        causalize_some=true;
        // This pair holds which edge(the first component) to use for causalization and which indexes(the second component)
        std::pair<VectorEdge,IndexPairSet> causal_pair = op.get();
        VectorEdge e = causal_pair.first;
        // This is the equation node connecting to the edge
        EquationVertex eq = GetEquation(e);
        if (debugIsEnabled('c')) {
          std::cout << "Eq vertex =  " << graph[eq].type << /*" " << graph[eq].eqs.front() << */"\n";
        }
        equationNumber--;
        unknownNumber--;
        // Save the result of this step of causalization
        CausalizeNto1(graph[unk].unknown, graph[eq].equation, causal_pair.second);
        // Update the pairs in the edge that is being causalized
        graph[e].RemovePairs(causal_pair.second);
        // Decrement the number of uncauzalized equations/unknowns
        graph[eq].count -= causal_pair.second.size();
        graph[unk].count -= causal_pair.second.size();
        // If the edge has no more pairs in it remove it
        if (graph[e].IsEmpty()) {
          remove_edge(e, graph);
        }
        // Auxiliary list to later remove empty edges
        std::list<VectorEdge> remove;
        foreach_(VectorEdge e1, out_edges(eq,graph)) {
          // Update the labels from all the edges adjacent to the equation
          if (debugIsEnabled('c')) {
            std::cout << "Removing equations from " << graph[e1]<<"\n";
          }
          graph[e1].RemoveEquations(causal_pair.second);
          // If the edge is now empty schedule it for removal
          if (graph[e1].IsEmpty()) {
            if (debugIsEnabled('c')) {
              std::cout << "Removing the edge\n";
            }
            remove.push_back(e1);
          }
        }
        // Now remove all scheduled edges
        foreach_(VectorEdge e1, remove) {
          if (e1!=e)
            WARNING_UNLESS(out_degree(GetUnknown(e1),graph)>1, "Disconnecting unknown node"); //TODO: Review this condition and error message
          remove_edge(e1, graph);
        }
        // If the equation node is now unconnected and with count==0 we can remove it
        if (out_degree(eq,graph)==0) {
          ERROR_UNLESS(graph[eq].count==0, "Disconnected node with uncausalized equations");
          remove_vertex(eq,graph);
          equationDescriptors.remove(eq);
        }
        // If the unknown node is now unconnected and with count==0 we can remove it
        if (out_degree(unk,graph)==0) {
          ERROR_UNLESS(graph[unk].count==0, "Disconnected node with uncausalized unknowns");
          remove_vertex(unk,graph);
          unknownDescriptors.erase(iter);
        }
        stringstream ss;
        ss << "graph_" << step++ << ".dot";
        GraphPrinter<VectorVertexProperty,VectorEdgeProperty>  gp(graph);
        gp.printGraph(ss.str());
      }
    }

    if(!causalize_some){
      //we have a LOOP or a FOR equation that we don't
      //handle at least yet, so we resort to the previous
      //algorithm
      ERROR("Loop detected! We don't handle loops yet!\n");
      return false;
    }
  }
}


Option<std::pair<VectorEdge,IndexPairSet> > CausalizationStrategyVector::CanCausalizeEquation(VectorEquationVertex eq) {
  VectorCausalizationGraph::out_edge_iterator vi, vi_end, others, others_end;
  VectorEdge candidate_edge; 
  IndexPairSet ret;
  IndexPairSet::iterator candidate_pair, test;
  for(boost::tie(vi,vi_end) = out_edges(eq,graph); vi != vi_end; ++vi) {
    // Try to find a pair in candidate_edge
    candidate_edge = *vi;
    VectorEdgeProperty ep = graph[*vi]; 
    if (debugIsEnabled('c')) {
      cout << "Checking edge " << ep << "\n";
    }
    // First check on ONE edge
    for (candidate_pair = ep.labels.begin(); candidate_pair!=ep.labels.end(); candidate_pair++) {
      if (debugIsEnabled('c')) {
        cout << "Checking pair (" << candidate_pair->first << "," <<candidate_pair->second  << ")\n";
      }
      for (test = ep.labels.begin(); test !=ep.labels.end(); test++) {
        // Skip the same pair in the same edge
        if (*candidate_pair==*test) 
          continue;
        // If other pair in the same edge uses the same unknown we cannot use this pair
        if (candidate_pair->first==test->first) {
          break;
        }
      }
      // If the candidate pair is not allowed in this edge continue with the next pair
      if (test!=ep.labels.end()) { 
        continue;
      }
      if (debugIsEnabled('c')) {
        cout << "That pair is OK with the candidate edge \n";
      }
      // First check on ONE edge
      // If we found a candidate_pair on the candidate_edge
      if (candidate_pair!=ep.labels.end() && test==ep.labels.end()) { 
        bool collision=false;
        if (debugIsEnabled('c')) {
          cout << "Checking with other edges\n";
        }
        // Check with other edges that this candidate works
        for(boost::tie(others,others_end) = out_edges(eq,graph); others != others_end; ++others) {
          // Skip the candidate_edge
          if (candidate_edge==*others)
            continue;
          VectorEdgeProperty ep = graph[*others]; 
          // Check that for all pairs in the other edges do not collision with the candidate_pair
          if (debugIsEnabled('c')) {
            cout << "Checking against " << ep << "\n";
          }
          for (test = ep.labels.begin(); test !=ep.labels.end(); test++) {
            if (candidate_pair->first==test->first) { 
              collision=true;
              break;
            }
          }
          // If we found a collision try another candidate_pair on candidate_edge
          if (collision)
            break;
        }
        // We traversed all the other edges and the pair seems to be causalizable
        if (others==others_end)  {
          if (debugIsEnabled('c')) {
            cout << "Pair (" << candidate_pair->first << "," << candidate_pair->second << ") works!\n";
          }
          ret.insert(*candidate_pair);
          return std::make_pair(candidate_edge,ret);
        }
      }
    }
  }
  // We traversed all the edges and found an edge and a pair with out collision => candidates
  if (vi!=vi_end) {
    if (debugIsEnabled('c')) {
      cout << "Pair (" << candidate_pair->first << "," << candidate_pair->second << ") works!\n";
    }
    // Return the candidates
    ret.insert(*candidate_pair);
    return std::make_pair(candidate_edge,ret);
  }
  return Option<std::pair<VectorEdge,IndexPairSet> >();

}

Option<std::pair<VectorEdge,IndexPairSet> > CausalizationStrategyVector::CanCausalizeUnknown(VectorUnknownVertex un) {
  VectorCausalizationGraph::out_edge_iterator vi, vi_end, others, others_end;
  VectorEdge candidate_edge; 
  IndexPairSet ret;
  IndexPairSet::iterator candidate_pair, test;
  for(boost::tie(vi,vi_end) = out_edges(un,graph); vi != vi_end; ++vi) {
    // Now try to find a pair in candidate_edge
    candidate_edge = *vi;
    VectorEdgeProperty ep = graph[*vi]; 
    if (debugIsEnabled('c')) {
      cout << "Checking edge " << ep << "\n";
    }
    // First check on ONE edge
    for (candidate_pair = ep.labels.begin(); candidate_pair!=ep.labels.end(); candidate_pair++) {
      if (debugIsEnabled('c')) {
        cout << "Checking pair (" << candidate_pair->first << "," <<candidate_pair->second  << ")\n";
      }
      for (test = ep.labels.begin(); test !=ep.labels.end(); test++) {
        // Skip the same pair in the same edge
        if (*candidate_pair==*test) 
          continue;
        // If other pair in the same edge uses the same unknown we cannot use this pair
        if (candidate_pair->second==test->second) {
          break;
        }
      }
      // If the candidate pair is not allowed in this edge continue with the next pair
      if (test!=ep.labels.end()) { 
        continue;
      }
      if (debugIsEnabled('c')) {
        cout << "That pair is OK with the candidate edge \n";
      }
      // First check on ONE edge
      // If we found a candidate_pair on the candidate_edge
      if (candidate_pair!=ep.labels.end() && test==ep.labels.end()) { 
        bool collision=false;
        if (debugIsEnabled('c')) {
          cout << "Checking with other edges\n";
        }
        // Check with other edges that this candidate works
        for(boost::tie(others,others_end) = out_edges(un,graph); others != others_end; ++others) {
          // Skip the candidate_edge
          if (candidate_edge==*others)
            continue;
          VectorEdgeProperty ep = graph[*others]; 
          // Check that for all pairs in the other edges do not collision with the candidate_pair
          if (debugIsEnabled('c')) {
            cout << "Checking against " << ep << "\n";
          }
          for (test = ep.labels.begin(); test !=ep.labels.end(); test++) {
            if (candidate_pair->second==test->second) { 
              collision=true;
              break;
            }
          }
          // If we found a collision try another candidate_pair on candidate_edge
          if (collision)
            break;
        }
        // We traversed all the other edges and the pair seems to be causalizable
        if (others==others_end)  {
          if (debugIsEnabled('c')) {
            cout << "Pair (" << candidate_pair->first << "," << candidate_pair->second << ") works!\n";
          }
          ret.insert(*candidate_pair);
          return std::make_pair(candidate_edge,ret);
        }
      }
    }
  }
  // We traversed all the edges and found an edge and a pair with out collision => candidates
  if (vi!=vi_end) {
    if (debugIsEnabled('c')) {
      cout << "Pair (" << candidate_pair->first << "," << candidate_pair->second << ") works!\n";
    }
    // Return the candidates
    ret.insert(*candidate_pair);
    return std::make_pair(candidate_edge,ret);
  }
  return Option<std::pair<VectorEdge,IndexPairSet> >();
}


void CausalizationStrategyVector::SolveEquations() {
  EquationList all;
  vector<CausalizedVar> sorted_vars = equations1toN;
  sorted_vars.insert(sorted_vars.end(),equationsNto1.begin(), equationsNto1.end());
  foreach_(CausalizedVar cv, sorted_vars) {
    Equation e = cv.equation;
    if(is<ForEq>(e)) {
      if (is<ForEq>(e)) {
        ForEq &feq = get<ForEq>(e);
        VarSymbolTable syms = mmo.syms_ref();

        int forIndex = cv.pairs.begin()->first;
        Expression varIndex = cv.pairs.begin()->second;

        cv.unknown.SetIndex(varIndex);
        Equation eq = instantiate_equation(feq.elements().front(), feq.range().indexes().front().name(), forIndex, syms);

        if (debugIsEnabled('c')) {
          std::cout <<  "Solving variable " << cv.unknown() << "\n";
          std::cout <<  "with eq " << feq << "\n";
        }
        std::list<std::string> c_code;
        ClassList cl;
        if (debugIsEnabled('c')) {
          std::cout << "Solving:\n" << e << "\nfor variable " << cv.unknown() << "\n";
        }
        std::stringstream s;
        s << mmo.name() << ".c";
        all.push_back(EquationSolver::Solve(eq, cv.unknown(), syms,c_code,cl, s.str()));
      } else {
        ERROR("Trying to solve an array variable with a non for equation");
      }
    } else{
      cv.unknown.SetIndex(cv.pairs.begin()->second);
      std::list<std::string> c_code;
      ClassList cl;
      if (debugIsEnabled('c')) {
        std::cout << "Solving\n" << e << "\nfor variable " << cv.unknown() << "\n";
      }
      std::stringstream s;
      s << mmo.name() << ".c";
      all.push_back(EquationSolver::Solve(e, cv.unknown(), mmo.syms_ref(),c_code, cl, s.str()));
    }
  }
  mmo.equations_ref().equations_ref()=all;
}

Vertex CausalizationStrategyVector::GetEquation(Edge e) {
  return ((graph[(source(e,graph))].type==E))?source(e,graph):target(e,graph);
}


Vertex CausalizationStrategyVector::GetUnknown(Edge e) {
  return ((graph[(target(e,graph))].type==U))?target(e,graph):source(e,graph);
}


void
CausalizationStrategyVector::PrintCausalizationResult(){
  vector<CausalizedVar> sorted_vars = equations1toN;
  sorted_vars.insert(sorted_vars.end(),equationsNto1.begin(), equationsNto1.end());
  cout << "Result of causalization: \n";
  foreach_(CausalizedVar cv, sorted_vars) {
    cout << "With equation \n";
    cout << cv.equation;
    cout << "\n solve variable " << cv.unknown();
    cout << " in range {";
    foreach_(IndexPair ip, cv.pairs)
      cout << "(" << ip.first << "," << ip.second << ")";
    cout << "}\n";
  }
//  vector<CausalizedVar> sorted_vars = equations1toN;
//  sorted_vars.insert(sorted_vars.end(),equationsNto1.begin(), equationsNto1.end());
//	foreach_(CausalizedVar cvar,sorted_vars) {
//		string name;
//		name = getName(cvar.unknown, cvar.edge);
//		if(cvar.edge.indexInterval.size() > 1){
//			interval_set<int>::iterator isi = cvar.edge.indexInterval.begin();
//			cout << "(" << name <<	"," << *isi << "," << cvar.equation.index << ")" << endl;
//		}else{
//			cout << "(" << name << ", " << cvar.equation.index << ")" << endl;
//		}
//	}
}
}
