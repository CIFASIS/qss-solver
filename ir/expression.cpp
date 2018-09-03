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

#include "expression.h"

#include <sstream>
#include "../ast/expression.h"
#include "../util/ast_util.h"
#include "../util/error.h"

namespace MicroModelica {
  using namespace Util;
  namespace IR {

    /* MMO_Expression class. */

    Expression::Expression(AST_Expression exp, const VarSymbolTable& symbols) :
      _exp(exp),
      _symbols(symbols)
    {
    }

    Expression::Expression() :
      _exp(NULL),
      _symbols()
    {
    }

    string 
    Expression::print() const 
    {
      ExpressionPrinter printer(_symbols);
      return printer.apply(_exp);
    }

    std::ostream& operator<<(std::ostream& out, const Expression& s)
    {
      out << s.print();
      return out;
    }
    
  }
}
