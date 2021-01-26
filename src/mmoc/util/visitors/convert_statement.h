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

#ifndef CONVERT_STATEMENT_H_
#define CONVERT_STATEMENT_H_

#include "../ast_util.h"

namespace MicroModelica {
namespace Util {

class ConvertStatement {
  public:
  ConvertStatement(AST_Statement statement);
  ~ConvertStatement() = default;

  AST_Statement get();

  private:
  AST_Statement convert(AST_Statement st);
  AST_StatementList convert(AST_StatementList sts);
  AST_Statement_ElseList convert(AST_Statement_ElseList stel);

  AST_Statement _statement;
};

}  // namespace Util
}  // namespace MicroModelica

#endif /* CONVERT_STATEMENT_H_ */
