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

#include "mmo_settings.h"

#include "../ast/class.h"
#include "../ast/modification.h"
#include "../util/error.h"
#include "class.h"

namespace MicroModelica {
using namespace Util;
namespace IR {

Settings::Settings(string name)
    : _model(), _insertAnnotation(false), _processFunction(false), _processModel(false), _classModification(false)
{
}

Settings::~Settings() {}

void Settings::visit(AST_Class x)
{
  Error::instance().setClassName(*(x->name()));
  AST_TypePrefix p = x->prefix();
  if (_processModel) {
    if ((p & CP_FUNCTION) || (p & CP_IMPURE) || (p & CP_PURE)) {
      _insertAnnotation = false;
      _processFunction = true;
    }
  } else {
    if (p & CP_MODEL) {
      _insertAnnotation = true;
      _model = Model(*x->name());
    }
  }
}

void Settings::leave(AST_Class x)
{
  if (_processFunction) {
    _processFunction = false;
    _insertAnnotation = true;
  }
}

void Settings::visit(AST_Composition x) {}

void Settings::leave(AST_Composition x) {}

void Settings::visit(AST_CompositionElement x) {}

void Settings::leave(AST_CompositionElement x) {}

void Settings::visit(AST_CompositionEqsAlgs x) {}

void Settings::leave(AST_CompositionEqsAlgs x) {}

void Settings::visit(AST_External_Function_Call x) {}

void Settings::visit(AST_Element x) {}

void Settings::visit(AST_Modification x)
{
  if (x->modificationType() == MODCLASS) {
    _classModification = false;
  }
}

void Settings::leave(AST_Modification x)
{
  if (x->modificationType() == MODCLASS) {
    _classModification = false;
  }
}

void Settings::visit(AST_Comment x) {}

void Settings::visit(AST_Equation x) {}

void Settings::visit(AST_ForIndex x) {}

void Settings::visit(AST_Equation_Else x) {}

void Settings::visit(AST_Expression x) {}

void Settings::visit(AST_Argument x)
{
  if (_insertAnnotation) {
    if (x->argumentType() == AR_MODIFICATION) {
      AST_Argument_Modification am = x->getAsModification();
      if (am->hasModification() && _classModification == false) {
        _model.insert(am);
      }
    }
  }
}

void Settings::visit(AST_Statement x) {}

void Settings::leave(AST_Statement x) {}

void Settings::visit(AST_Statement_Else x) {}

void Settings::visit(AST_StoredDefinition x) {}

void Settings::leave(AST_StoredDefinition x) {}

int Settings::apply(AST_Node x)
{
  x->accept(this);
  return Error::instance().errors();
}

ModelAnnotation Settings::annotations() { return _model.annotations(); }
}  // namespace IR
}  // namespace MicroModelica
