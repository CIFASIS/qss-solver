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

#pragma once 

#include "../util/macros.h"
#include "../util/util_types.h"

class AST_Visitor;

using namespace std;

class AST_Node_ {
  public:
  AST_Node_();
  virtual ~AST_Node_() {};
  DEFINE_CLASS_PRINTER(AST_Node);
  DEFINE_ORDER_REL(AST_Node);
  void setLineNum(int linenum);
  int lineNum() const;
  virtual void accept(AST_Visitor *visitor) = 0;

  private:
  int _linenum;
};
