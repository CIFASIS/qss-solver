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

#include "replace_index.h"

#include <sstream> 

#include "../../ast/ast_builder.h"
#include "../error.h"
#include "../symbol_table.h"

namespace MicroModelica {
  namespace Util {

    ReplaceIndex::ReplaceIndex(VarSymbolTable vt) :
        _vt(vt)
    {
    }

    AST_Expression
    ReplaceIndex::foldTraverseElement(AST_Expression exp)
    {
      switch(exp->expressionType())
      {
        case EXPCOMPREF:
        {
          AST_Expression_ComponentReference cr = exp->getAsComponentReference();
          Option<Variable> var = _symbols[cr->name()];
          if(!var)
          {
            Error::instance().add(exp->lineNum(), EM_IR | EM_VARIABLE_NOT_FOUND, ER_Error, "%s", cr->name().c_str());
            break;
          }
          if(cr->hasIndexes())
          {
            AST_Expression_ComponentReference ret = newAST_Expression_ComponentReference();
            AST_ExpressionList indexes = cr->firstIndex();
            AST_ExpressionListIterator it;
            int size = indexes->size(), i = 0;
            foreach(it, indexes)
            {
              buffer << apply(current_element(it)) << (++i < size ? "," : "");
            }
          }
          break;
        }
        default:
          break;
      }
      return exp;
    }

    AST_Expression
    ReplaceIndex::foldTraverseElementUMinus(AST_Expression exp)
    {
      return newAST_Expression_UnaryMinus(apply(exp->getAsUMinus()->exp()));
    }

    AST_Expression
    ReplaceIndex::foldTraverseElement(AST_Expression l, AST_Expression r, BinOpType bot)
    {
      return newAST_Expression_BinOp(l, r, bot);
    }
  }
}
