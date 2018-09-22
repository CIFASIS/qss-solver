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
    along with Modelica C Compiler.  If not, see <http:  www.gnu.org/licenses/>.

******************************************************************************/

#include <causalize/vector/contains_vector.h>
#include <util/debug.h>
#include <boost/variant/get.hpp>
#include <causalize/vector/vector_graph_definition.h>
#include <util/ast_visitors/eval_expression.h>
#include <boost/icl/discrete_interval.hpp>
#include <util/ast_visitors/partial_eval_expression.h>
#include <ast/queries.h>

using namespace boost;
using namespace Modelica;
using namespace Modelica::AST ;

namespace Causalize {
  ContainsVector::ContainsVector(Expression e, VectorVertexProperty v, const VarSymbolTable &s):
    exp(e),
		unk2find(v),
		syms(s),
		foreq(false),
		indexes() {};

  /**
   * Builds a ContainsVector class to find occurrences of the Expression 'e'
   * using the table of symbols 's' and index list 'indexes' in 'for-equations'.
   */
  ContainsVector::ContainsVector(VectorVertexProperty unk, VarSymbolTable &s, IndexList indexes):
    exp(unk.unknown()),
    unk2find(unk),
    syms(s),
    foreq(true),
    indexes(indexes) {
      std::cout << "Looking for exp " << exp;
      ERROR_UNLESS(indexes.size()==1, "For Loop with more than one index is not supported yet\n");
      ERROR_UNLESS((bool)indexes.front().exp(), "No index in for equation");
      ERROR_UNLESS(is<Range>(indexes.front().exp().get()), "Only range expressions supported");
      Range range = get<Range>(indexes.front().exp().get());
      EvalExpression ev(syms);
      forIndexInterval =  boost::icl::discrete_interval<int>::closed(Apply(ev,range.start_ref()), Apply(ev,range.end_ref()));
  };

  bool ContainsVector::operator()(Modelica::AST::Integer v) const { return exp==Expression(v); }

  bool ContainsVector::operator()(Boolean v) const { return exp==Expression(v); }

  bool ContainsVector::operator()(String v) const { return exp==Expression(v); }

  bool ContainsVector::operator()(Name v) const { return exp==Expression(v); }

  bool ContainsVector::operator()(Real v) const { return exp==Expression(v); }

  bool ContainsVector::operator()(SubEnd v) const { return exp==Expression(v); }

  bool ContainsVector::operator()(SubAll v) const { return exp==Expression(v); }

  bool ContainsVector::operator()(BinOp v) const {
    if (exp==Expression(v)) return true;
    Expression l=v.left(), r=v.right();
    bool rl = ApplyThis(l);
    bool rr = ApplyThis(r);
    return rr || rl;
  }

  bool ContainsVector::operator()(UnaryOp v) const {
    if (exp==Expression(v)) return true;
    Expression e=v.exp();
    return ApplyThis(e);
  }

  bool ContainsVector::operator()(IfExp v) const {
    if (exp==Expression(v)) return true;
    Expression cond=v.cond(), then=v.then(), elseexp=v.elseexp();
    const bool rc = ApplyThis(cond);
    const bool rt = ApplyThis(then);
    const bool re = ApplyThis(elseexp);
    return rc || rt || re;
  }

  bool ContainsVector::operator()(Range v) const {
    if (exp==Expression(v)) return true;
    Expression start=v.start(), end=v.end();
    bool rs = ApplyThis(start);
    bool re = ApplyThis(end);
    return rs || re;
  }

  bool ContainsVector::operator()(Brace v) const {
    if (exp==Expression(v)) return true;
    return false;
  }

  bool ContainsVector::operator()(Bracket v) const {
    if (exp==Expression(v)) return true;
    return false;
  }

  bool ContainsVector::operator()(Call call) const {
    if (is<Call>(exp)) { //exp must be a derivative expression
      Call callExpr=get<Call>(exp);
      if (call.name()=="der" && callExpr.name()=="der") {  //call and exp are derivative expressions
        ERROR_UNLESS(is<Reference>(call.args().front()) && is<Reference>(callExpr.args().front()), "Arguments to call must be a reference");
        Reference callRef=get<Reference>(call.args().front());
        Reference callExprRef=get<Reference>(callExpr.args().front());
        if (get<0>(callRef.ref().front())==get<0>(callExprRef.ref().front())) { //The references are the same
          std::cout << "build pairs with " << callRef;
          buildPairs(callRef);
          return true;
        }
        //The references are not the same
        return false;
      }
    } else { //exp is not a derivative expression, find the occurrence of exp inside the call arguments
      if (exp==Expression(call)) return true;
      bool findOccur = false;
      foreach_ (Expression e, call.args()) {
        findOccur|=(ApplyThis(e));
      }
      return findOccur;
    }

    return false;
  }

  bool ContainsVector::operator()(FunctionExp v) const {
    if (exp==Expression(v)) return true;
    return false;
  }

  bool ContainsVector::operator()(ForExp v) const {
    if (exp==Expression(v)) return true;
    return false;
  }

  bool ContainsVector::operator()(Named v) const {
    if (exp==Expression(v)) return true;
    return false;
  }

  bool ContainsVector::operator()(Output v) const {
    if (exp==Expression(v)) return true;
    foreach_(OptExp oe, v.args())
      if (oe && ApplyThis(oe.get())) return true;
    return false;
  }

  bool ContainsVector::operator()(Reference ref) const {
    if (is<Reference>(exp)) {
      if (get<0>(ref.ref().front())==get<0>(get<Reference>(exp).ref().front())) { //The references are the same
        buildPairs(ref);
        return true;
      }
    }
    return false;
  }


  void ContainsVector::buildPairs(Reference unkRef) const {
    if (unk2find.count==1) { // The unknown is a scalar (or array of size 1)
      VectorEdgeProperty newEdge;
      if (foreq) { //The equation is a for-equation
        buildPairsNto1();
      }
      else { //The equation is not a for-equation
        buildPairs1to1();
      }
    }
    else { // The unknown is an array
      ExpList el = get<1>(unkRef.ref().front());
      if (el.size()==0) { // If there are no sub-indices the complete array is used
        if (foreq) { //The equation is a for-equation
          buildPairsNtoN();
        }
        else { //The equation is not a for-equation
          buildPairs1toN();
        }
      }
      else { // The unknown has a sub-index
        Expression i = el.front();
        Modelica::PartialEvalExpression pe(syms);
        Expression ind = Apply(pe,i);
        if (is<Modelica::AST::Integer>(ind)) { // The index is a constant value
          int index = get<Modelica::AST::Integer>(ind);
          ERROR_UNLESS(index<=unk2find.count && index >= 1,"Index out of range");
          if (foreq) { //The equation a for-equation
            buildPairsNto1(index);
          }
          else {  //The equation is not a for-equation
            buildPairs1to1(index);
          }
        }
        else if (is<Reference>(ind)) { // The index is a reference
          ERROR_UNLESS(foreq, "Generic index used outside for equation");
          Reference indRef = get<Reference>(ind);
          ERROR_UNLESS(refName(indRef)==indexes.front().name(),"Array index reference and for index variable are not the same");
          buildPairsN();
        }
        else { 
          buildPairsNExpression(ind);
        }
      }
    }
  }

  void ContainsVector::buildPairs1to1(int index) const {
    labels.insert(std::make_pair(1,index));
  }

  void ContainsVector::buildPairsNto1(int index) const {
    for (int i=forIndexInterval.lower();i<=forIndexInterval.upper();i++) {
      labels.insert(std::make_pair(i,index));
    }
  }

  void ContainsVector::buildPairsN() const {
    for (int i=forIndexInterval.lower();i<=forIndexInterval.upper();i++) {
      labels.insert(std::make_pair(i,i));
    }
  }


  void ContainsVector::buildPairsNExpression(Expression exp) const {
    Modelica::PartialEvalExpression partial_evaluator(syms);
    for (int i=forIndexInterval.lower();i<=forIndexInterval.upper();i++) {
      syms.insert(indexes.front().name(), VarInfo(TypePrefixes(1,parameter),"Integer", Option<Comment>(),Modification(ModEq(Expression(i)))));
      Expression ind = Apply(partial_evaluator,exp);
      ERROR_UNLESS(is<Modelica::AST::Integer>(ind),"Index Expression is not an integer value");
      labels.insert(std::make_pair(i,get<Modelica::AST::Integer>(ind)));
    }
  }


  void ContainsVector::buildPairs1toN() const {
    for (int i=1;i<=unk2find.count;i++) {
      labels.insert(std::make_pair(1,i));
    }
  }

  void ContainsVector::buildPairsNtoN() const {
    for (int i=forIndexInterval.lower();i<=forIndexInterval.upper();i++) {
      for (int j=1;j<=unk2find.count;j++) {
        labels.insert(std::make_pair(i,j));
      }
    }
  }

}
