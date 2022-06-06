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

#include <list>
#include <string>

#include "../ast/ast_types.h"
#include "../util/ast_util.h"
#include "../ir/stored_definition.h"

namespace MicroModelica {
namespace IR {

class MicroModelicaIR : public AST_Visitor {
  public:
  MicroModelicaIR(string name);
  ~MicroModelicaIR();
  StoredDefinition storedDefinition();
  list<Class> classes() const;
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
  inline const StoredDefinition definition() { return _std; };

  protected:
  void visitForStms(AST_Statement_For for_stm);

  private:
  StoredDefinition _std;
  ClassPtr _class;
  bool _initialCode;
  bool _classModification;
  bool _compositionElement;
  Function _function;
};
}  // namespace IR
}  // namespace MicroModelica
