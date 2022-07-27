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

#include "mmo_ir.h"

#include <iterator>

#include "../ast/class.h"
#include "../ast/composition.h"
#include "../ast/element.h"
#include "../ast/equation.h"
#include "../ast/expression.h"
#include "../ast/modification.h"
#include "../ast/statement.h"
#include "../util/visitors/eval_init_exp.h"
#include "../util/visitors/array_use.h"
#include "../util/visitors/discrete_assignment.h"
#include "../util/error.h"
#include "../util/symbol_table.h"
#include "../util/type.h"
#include "class.h"
#include "expression.h"
#include "helpers.h"
#include "stored_definition.h"

namespace MicroModelica {
using namespace Util;
namespace IR {

/* MicroModelica Intermediate Representation */

MicroModelicaIR::MicroModelicaIR(string name)
    : _std(), _class(nullptr), _initialCode(false), _classModification(false), _compositionElement(false)
{
}

MicroModelicaIR::~MicroModelicaIR() {}

void MicroModelicaIR::visit(AST_Class x)
{
  Error::instance().setClassName(*(x->name()));
  AST_TypePrefix p = x->prefix();
  if (p & CP_PACKAGE) {
    _std.setPackage(*x->name());
    _class = &(_std.package());
  } else if ((p & CP_FUNCTION) || (p & CP_IMPURE) || (p & CP_PURE)) {
    _function = Function(*x->name());
    _class = &_function;
  } else {
    _std.setModel(*x->name());
    _class = &(_std.model());
  }
}

void MicroModelicaIR::leave(AST_Class x)
{
  AST_TypePrefix p = x->prefix();
  if ((p & CP_FUNCTION) || (p & CP_IMPURE) || (p & CP_PURE)) {
    _std.addFunction(_function);
  }
}

void MicroModelicaIR::visit(AST_Composition x) {}

void MicroModelicaIR::leave(AST_Composition x) {}

void MicroModelicaIR::visit(AST_CompositionElement x) { _compositionElement = true; }

void MicroModelicaIR::leave(AST_CompositionElement x) { _compositionElement = false; }

void MicroModelicaIR::visit(AST_CompositionEqsAlgs x) { _initialCode = x->isInitial(); }

void MicroModelicaIR::leave(AST_CompositionEqsAlgs x) { _initialCode = false; }

void MicroModelicaIR::visit(AST_External_Function_Call x) { _class->insert(x); }

void MicroModelicaIR::visit(AST_Element x)
{
  ElementType e = x->elementType();
  if (e == IMPORT) {
    AST_Element_ImportClause i = x->getAsImportClause();
    _class->insert(i->name());
  } else if (e == COMPONENT) {
    AST_Element_Component c = x->getAsComponent();
    AST_TypePrefix tp = c->typePrefix();
    AST_DeclarationListReverseIterator it;
    AST_DeclarationList dl = c->nameList();
    foreachReverse(it, dl)
    {
      vector<int> size;
      bool array = current_element(it)->hasIndexes();
      if (array) {
        EvalInitExp eval;
        AST_ExpressionList elist = current_element(it)->indexes();
        AST_ExpressionListIterator elistit;
        foreach (elistit, elist) {
          size.push_back(eval.apply(current_element(elistit)));
        }
      }
      DEC_Type t = (_compositionElement ? DEC_LOCAL : DEC_PUBLIC);
      if (tp & TP_CONSTANT) {
        Variable vi(newType_Integer(), tp, current_element(it)->modification(), nullptr, size, array);
        _class->insert(current_element(it)->name(), vi, t);
      } else {
        if ((tp & TP_PARAMETER) && c->isInteger()) {
          Variable vi(newType_Integer(), tp, current_element(it)->modification(), nullptr, size, array);
          _class->insert(current_element(it)->name(), vi, t);
        } else if (c->isString()) {
          Variable vi(newType_String(), tp, current_element(it)->modification(), nullptr, size, array);
          _class->insert(current_element(it)->name(), vi, t);
        } else {
          Variable vi(newType_Real(), tp, current_element(it)->modification(), nullptr, size, array);
          _class->insert(current_element(it)->name(), vi, t);
        }
      }
    }
  } else if (e == ELCLASS) {
    AST_Class c = x->getAsClassWrapper()->getClass();
    c->accept(this);
  }
}

void MicroModelicaIR::visit(AST_Modification x)
{
  if (x->modificationType() == MODASSIGN) {
    Error::instance().add(x->lineNum(), EM_AST | EM_CLASS_DEFINITION, ER_Error, "Assign modifier.");
  }
  if (x->modificationType() == MODCLASS) {
    _classModification = true;
  }
}

void MicroModelicaIR::leave(AST_Modification x)
{
  if (x->modificationType() == MODCLASS) {
    _classModification = false;
  }
}

void MicroModelicaIR::visit(AST_Comment x) {}

void MicroModelicaIR::visit(AST_Equation x)
{
  if (x->equationType() == EQFOR) {
    AST_ForIndexList fil = x->getAsFor()->forIndexList();
    AST_ForIndexListIterator it;
    foreach (it, fil) {
      visit(current_element(it));
    }
  } else {
    AST_Equation_Equality eqe = x->getAsEquality();
    DiscreteAssignment da;
    da.apply(eqe->left());
    ArrayUse au;
    au.apply(eqe->left());
    au.apply(eqe->right());
  }
  _class->insert(x);
}

void MicroModelicaIR::visit(AST_ForIndex x)
{
  Variable vi(newType_Integer(), TP_FOR, nullptr, nullptr, vector<int>(1, 1), false);
  _class->insert(*x->variable(), vi);
}

void MicroModelicaIR::visit(AST_Equation_Else x) {}

void MicroModelicaIR::visit(AST_Expression x) {}

void MicroModelicaIR::visit(AST_Argument x)
{
  if (x->argumentType() == AR_MODIFICATION) {
    AST_Argument_Modification am = x->getAsModification();
    if (am->hasModification() && _classModification == false) {
      _class->insert(am);
    }
  }
}

void MicroModelicaIR::visitForStms(AST_Statement_For for_stm)
{
  AST_ForIndexList fil = for_stm->forIndexList();
  AST_ForIndexListIterator it;
  foreach (it, fil) {
    visit(current_element(it));
  }
  AST_StatementList stms = for_stm->statements();
  AST_StatementListIterator stm_it;
  foreach (stm_it, stms) {
    if (current_element(stm_it)->statementType() == STFOR) {
      visitForStms(current_element(stm_it)->getAsFor());
    }
    if (current_element(stm_it)->statementType() == STWHEN) {
      AST_StatementList when_stms = current_element(stm_it)->getAsWhen()->statements();
      AST_StatementListIterator when_stm_it;
      foreach (when_stm_it, when_stms) {
        if (current_element(when_stm_it)->statementType() == STFOR) {
          visitForStms(current_element(when_stm_it)->getAsFor());
        }
      }
    }
  }
}

void MicroModelicaIR::visit(AST_Statement x)
{
  if (x->statementType() == STFOR) {
    visitForStms(x->getAsFor());
  }
  StatementArrayUse au;
  au.apply(x);
  _class->insert(x, _initialCode);
}

void MicroModelicaIR::leave(AST_Statement x) {}

void MicroModelicaIR::visit(AST_Statement_Else x)
{
  AST_StatementList stl = x->statements();
  AST_StatementListIterator sti;
  foreach (sti, stl) {
    visit(current_element(sti));
  }
}

void MicroModelicaIR::visit(AST_StoredDefinition x) {}

void MicroModelicaIR::leave(AST_StoredDefinition x) { _std.postProcess(); }

int MicroModelicaIR::apply(AST_Node x)
{
  x->accept(this);
  return Error::instance().errors();
}
}  // namespace IR
}  // namespace MicroModelica
