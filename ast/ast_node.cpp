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

#include "ast_node.h"

#include <iostream>

#include "../parser/mocc_parser.h"

using namespace std;

AST_Node_::AST_Node_()
{
  _linenum = MCC_Parser::lineno();
}

ostream &
operator<<(ostream &os, const AST_Node_ &n)
{
  os << "Printing not implemented!!" << endl;
  return os;
}
;

void
AST_Node_::setLineNum(int linenum)
{
  _linenum = linenum;
}

int
AST_Node_::lineNum() const
{
  return _linenum;
}
