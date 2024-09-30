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

#include <util/macros.h>
#include <util/util_types.h>
#include <ast/ast_node.h>
#include <ast/ast_types.h>

class AST_Imports;

class AST_StoredDefinition_ : public AST_Node_ {
  public:
  AST_StoredDefinition_(AST_ClassList ml, AST_String within);
  virtual ~AST_StoredDefinition_(){};
  DEFINE_CLASS_PRINTER(AST_StoredDefinition);
  AST_ClassList models() const;
  AST_String within() const;
  void accept(AST_Visitor *visitor);
  bool hasWithin();
  AST_StringList imports();

  private:
  AST_ClassList _model_list;
  AST_String _within;
  AST_Imports *_imports;
};
