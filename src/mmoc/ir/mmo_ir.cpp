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

#include <ast/class.h>
#include <ast/composition.h>
#include <ast/element.h>
#include <ast/equation.h>
#include <ast/expression.h>
#include <ast/modification.h>
#include <ast/statement.h>
#include <ir/class.h>
#include <ir/expression.h>
#include <ir/helpers.h>
#include <ir/stored_definition.h>
#include <util/visitors/eval_init_exp.h>
#include <util/visitors/array_use.h>
#include <util/visitors/discrete_assignment.h>
#include <util/error.h>
#include <util/symbol_table.h>
#include <util/type.h>

namespace MicroModelica {
using namespace Util;
namespace IR {

/* MicroModelica Intermediate Representation */

MicroModelicaIR::MicroModelicaIR(string name)
    : _std(), _class(nullptr), _father_class(nullptr), _initialCode(false), _classModification(false), _compositionElement(false)
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
    _father_class = _class;
  }
}

void MicroModelicaIR::leave(AST_Class x)
{
  _class = nullptr;
  AST_TypePrefix p = x->prefix();
  if ((p & CP_FUNCTION) || (p & CP_IMPURE) || (p & CP_PURE)) {
    _std.addFunction(_function);
    if (_father_class) {
      _class = _father_class;
    }
  } else if (p & CP_MODEL) {
    _father_class = nullptr;
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

AST_StatementList MicroModelicaIR::getStatementList(AST_EquationList eqs)
{
  AST_StatementList stms = newAST_StatementList();
  AST_EquationListIterator eqs_it;
  foreach (eqs_it, eqs) {
    AST_Statement stm = transformEquation(current_element(eqs_it));
    if (stm) {
      stms = AST_ListAppend(stms, stm);
    }
  }
  return stms;
}

bool MicroModelicaIR::checkForEquation(AST_Equation_For for_eq)
{
  AST_EquationList for_eqs = for_eq->equationList();
  AST_EquationListIterator eqs_it;
  AST_StatementList when_stms = newAST_StatementList();
  AST_EquationList split_for_eqs = newAST_EquationList();
  foreach (eqs_it, for_eqs) {
    if (current_element(eqs_it)->equationType() == EQWHEN) {
      when_stms = AST_ListAppend(when_stms, transformWhenEquation(current_element(eqs_it)->getAsWhen()));
    } else {
      split_for_eqs = AST_ListAppend(split_for_eqs, current_element(eqs_it));
    }
  }
  if (!when_stms->empty()) {
    AST_Statement stm = newAST_Statement_For(for_eq->forIndexList(), when_stms);
    visit(stm);
  }
  if (!split_for_eqs->empty()) {
    AST_ForIndexList fil = for_eq->forIndexList();
    AST_ForIndexListIterator it;
    foreach (it, fil) {
      visit(current_element(it));
    }
    AST_Equation eq = newAST_Equation_For(for_eq->forIndexList(), split_for_eqs);
    _class->insert(eq);
  }
  return when_stms->empty();
}

AST_Statement MicroModelicaIR::transformWhenEquation(AST_Equation_When when_eq)
{
  AST_StatementList true_stms = getStatementList(when_eq->equationList());
  AST_Statement_ElseList else_when_stms = newAST_Statement_ElseList();
  AST_Equation_ElseList else_when_eqs = when_eq->equationElseWhen();
  AST_Equation_ElseListIterator else_when_eqs_it;
  foreach (else_when_eqs_it, else_when_eqs) {
    AST_Equation_Else eq_else_when = current_element(else_when_eqs_it);
    AST_StatementList current_else_when_stms = getStatementList(eq_else_when->equations());
    AST_Statement_Else add_stm_else = newAST_Statement_Else(eq_else_when->condition(), current_else_when_stms);
    else_when_stms = AST_ListAppend(else_when_stms, add_stm_else);
  }
  return newAST_Statement_When(when_eq->condition(), true_stms, else_when_stms, when_eq->comment());
}

bool MicroModelicaIR::checkWhenEquation(AST_Equation eq)
{
  AST_Statement stm = nullptr;
  if (eq->equationType() == EQFOR) {
    return checkForEquation(eq->getAsFor());
  } else if (eq->equationType() == EQWHEN) {
    stm = transformWhenEquation(eq->getAsWhen());
    visit(stm);
    return true;
  }
  return false;
}

AST_Statement MicroModelicaIR::transformEquation(AST_Equation x)
{
  AST_Statement stm = nullptr;
  if (x->equationType() == EQEQUALITY) {
    AST_Equation_Equality eqe = x->getAsEquality();
    AST_Expression_ComponentReference lhs = eqe->left()->getAsComponentReference();
    stm = newAST_Statement_Assign(lhs, eqe->right());
  } else if (x->equationType() == EQCALL) {
    AST_Equation_Call eq_call = x->getAsCall();
    AST_Expression call = eq_call->call();
    if (call->expressionType() == EXPCALL) {
      AST_Expression_Call exp_call = call->getAsCall();
      AST_Expression_ComponentReference output_name = newAST_Expression_ComponentReference();
      AST_ExpressionList l = newAST_ExpressionList();
      output_name = AST_Expression_ComponentReference_Add(output_name, newAST_String(exp_call->name()), l);
      stm = newAST_Statement_OutputAssign(exp_call->outputArguments(), output_name, exp_call->arguments());
    }
  } else if (x->equationType() == EQFOR) {
    AST_Equation_For for_eq = x->getAsFor();
    AST_StatementList stms = getStatementList(for_eq->equationList());
    stm = newAST_Statement_For(for_eq->forIndexList(), stms);
  } else if (x->equationType() == EQIF) {
    AST_Equation_If if_eq = x->getAsIf();
    AST_StatementList true_stms = getStatementList(if_eq->equationList());
    AST_StatementList else_stms = getStatementList(if_eq->equationElseList());
    AST_Statement_ElseList else_if_stms = newAST_Statement_ElseList();
    AST_Equation_ElseList else_if_eqs = if_eq->equationElseIf();
    AST_Equation_ElseListIterator else_if_eqs_it;
    foreach (else_if_eqs_it, else_if_eqs) {
      AST_Equation_Else eq_else_if = current_element(else_if_eqs_it);
      AST_StatementList current_else_if_stms = getStatementList(eq_else_if->equations());
      AST_Statement_Else add_stm_else = newAST_Statement_Else(eq_else_if->condition(), current_else_if_stms);
      else_if_stms = AST_ListAppend(else_if_stms, add_stm_else);
    }
    stm = newAST_Statement_If(if_eq->condition(), true_stms, else_if_stms, else_stms);
  }
  return stm;
}

void MicroModelicaIR::visit(AST_Equation x)
{
  if (_initialCode) {
    visit(transformEquation(x));
    return;
  }
  if (checkWhenEquation(x)) {
    return;
  }
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
