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

#include <math.h>
#include <boost/variant/variant.hpp>
#include <boost/variant/get.hpp>

#include <util/debug.h>
#include <util/table.h>
#include <util/ast_visitors/partial_eval_expression.h>

#include <causalize/for_unrolling/process_for_equations.h>
#include <causalize/for_unrolling/for_index_iterator.h>

namespace Causalize {

Equation instantiate_equation(Equation innerEq, Name variable, Real index, VarSymbolTable &symbolTable) {
  VarSymbolTable v=symbolTable;
  VarInfo vinfo = VarInfo(TypePrefixes(1,parameter), "Integer", Option<Comment>(), Modification(ModEq(Expression(index))));
  v.insert(variable,vinfo);
  if (is<Equality>(innerEq)) {
      Equality eqeq = boost::get<Equality>(innerEq);
      Expression l=eqeq.left(), r=eqeq.right();
      //std::cout << "Left= " << l << " right " << r << std::endl;
      return Equality(Apply(Modelica::PartialEvalExpression(v),l),Apply(Modelica::PartialEvalExpression(v),r));
  } else {
      ERROR("process_for_equations - instantiate_equation:\n"
            "Incorrect equation type or not supported yet.\n");
  }
  return Equation();
}

void process_for_equations(Modelica::MMO_Class &mmo_class) {
  EquationList &equations = mmo_class.equations_ref().equations_ref();
  EquationList new_equations;
  foreach_ (Equation &e, equations) {
    if (is<ForEq>(e)) {
      ForEq feq = boost::get<ForEq>(e);
      IndexList il = feq.range().indexes();
      ERROR_UNLESS(il.size() == 1,
          "process_for_equations:\n"
          "forIndexList with more than 1 forIndex are not supported yet\n");
      Index in = il.front();
      Name variable  = in.name();
      OptExp ind = in.exp();
      ERROR_UNLESS((bool)ind, "for-equation's index with implicit range not supported yet\n");
      Expression exp = ind.get();
      ForIndexIterator *forIndexIter = NULL;
      if (is<Range>(exp)) {
        forIndexIter = new RangeIterator(get<Range>(exp),mmo_class.syms_ref());
      } else if (is<Brace>(exp)) {
        forIndexIter = new BraceIterator(get<Brace>(exp),mmo_class.syms_ref());
      } else {
        ERROR("For Iterator not supported");
      }
      while (forIndexIter->hasNext()) {
        Real index_val = forIndexIter->next();
        foreach_ (Equation eq, feq.elements()) 
          new_equations.push_back(instantiate_equation(eq, variable, index_val, mmo_class.syms_ref()));
      }
      delete forIndexIter;
    } else {
      // Not a for eq
      new_equations.push_back(e);
    }

  }
  mmo_class.equations_ref().equations_ref()=new_equations;
}

}




