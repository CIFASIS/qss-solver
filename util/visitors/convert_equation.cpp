/*****************************************************************************

 This file is part of QSS Solver.

 QSS Solver is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 QSS Solver is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with QSS Solver.  If not, see <http://www.gnu.org/licenses/>.

 ******************************************************************************/

#include "convert_equation.h"

#include <sstream>

#include "convert_expression.h"
#include "replace_inner_product.h"
#include "../util.h"
#include "../../parser/parse.h"
#include "../../ast/ast_types.h"
#include "../../ast/ast_builder.h"

namespace MicroModelica {
  using namespace IR;
  namespace Util {

    ConvertEquation::ConvertEquation(AST_Equation equation, VarSymbolTable& symbols) :
      _symbols(symbols)
    {
      _equation = convert(equation);
    }

    AST_Equation 
    ConvertEquation::convert(AST_Equation eq)
    {
      if(eq->equationType() == EQEQUALITY)
      {
        ReplaceInnerProduct rip(_symbols);
        AST_Expression l = eq->getAsEquality()->left();
        AST_Expression r = rip.apply(eq->getAsEquality()->right());
        string transform = ConvertExpression(l, r,_symbols).get();
        if(transform.empty())
        {
          return newAST_Equation_Equality(l, r);
        }
        int rValue;
        return parseEquation(transform, &rValue);
      }
      else if(eq->equationType() == EQFOR)
      {
        AST_Equation_For eqf = eq->getAsFor();
        AST_ForIndexList fil = eqf->forIndexList();
        AST_EquationList eqs = eqf->equationList();
        AST_EquationListIterator it;
        AST_EquationList tel = newAST_EquationList();
        foreach(it,eqs)
        {
          AST_ListAppend(tel, convert(current_element(it)));
        }
        return newAST_Equation_For(fil, tel);
      }
      return eq;
    }
  }
}
