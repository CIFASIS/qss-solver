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

#include <string>

#include <ast/ast_types.h>
#include <util/ast_util.h>
#include <util/util_types.h>

namespace MicroModelica {
namespace IR {

class ModelChecker : public AST_Visitor {
  public:
  ModelChecker(string name);
  ~ModelChecker();
  void visit(AST_Class x);
  void leave(AST_Class x);
  void visit(AST_Composition x);
  void leave(AST_Composition x);
  void visit(AST_CompositionElement x);
  void leave(AST_CompositionElement x);
  void visit(AST_CompositionEqsAlgs x);
  void leave(AST_CompositionEqsAlgs x);
  void visit(AST_External_Function_Call);
  void visit(AST_Element x);
  void visit(AST_Modification x);
  void leave(AST_Modification x);
  void visit(AST_Comment x);
  void visit(AST_Equation x);
  void visit(AST_ForIndex x);
  void visit(AST_Equation_Else x);
  void visit(AST_Expression x);
  void visit(AST_Argument x);
  void visit(AST_Statement x);
  void leave(AST_Statement x);
  void visit(AST_Statement_Else x);
  void visit(AST_StoredDefinition x);
  void leave(AST_StoredDefinition x);
  int apply(AST_Node x);

  private:
  bool _lValue(AST_Expression left);
  bool _whenStatement(AST_Expression cond);
  bool _has_parent;
  std::string _className;
  AST_ClassPrefix _classPrefix;
  bool _classModification;
  bool _elseWhen;
};
}  // namespace IR
}  // namespace MicroModelica
