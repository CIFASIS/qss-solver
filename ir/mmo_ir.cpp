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
#include "../util/error.h"
#include "../util/symbol_table.h"
#include "../util/type.h"
#include "class.h"
#include "expression.h"
#include "stored_definition.h"

/* MicroModelica Intermediate Representation */

MMO_MicroModelicaIR_::MMO_MicroModelicaIR_(string name) :
    _class(NULL), _father(NULL), _child(NULL), _classModification(false),
        _compositionElement(false), _storedDefinition(NULL), _funcs(1), _childPrefix(), _fatherName(),
        _childName(), _initialCode(), _className(), _fatherPrefix(), _classPrefix(), _elseWhen(
            false)
{
  _externalFunctions = newMMO_FunctionTable();
}

MMO_MicroModelicaIR_::~MMO_MicroModelicaIR_()
{
}

void
MMO_MicroModelicaIR_::visit(AST_Class x)
{
  Error::getInstance()->setClassName(*(x->name()));
  AST_TypePrefix p = x->prefix();
  if(_father != NULL)
  {
    if((p & CP_FUNCTION) || (p & CP_IMPURE) || (p & CP_PURE))
    {
      _childName = x->name();
      _childPrefix = x->prefix();
      _child = newMMO_Function(*x->name());
      if(_father->classType() == CL_MODEL)
      {
        _child->getAsFunction()->setFunctions(
            _father->getAsModel()->functions(), _externalFunctions,
            _father->getAsModel()->calledFunctions());
      }
      _child->getAsFunction()->setImports(_father->imports());
      _child->setFather(_father);
      _class = _child;
      _className = _childName;
      _classPrefix = _childPrefix;
    }
  }
  else
  {
    _fatherName = x->name();
    _fatherPrefix = x->prefix();
    if(p & CP_PACKAGE)
    {
      _father = newMMO_Package(*x->name());
    }
    else if((p & CP_FUNCTION) || (p & CP_IMPURE) || (p & CP_PURE))
    {
      _father = newMMO_Function(*x->name());
      Index i(_funcs++, 0);
      i.setOffset(_funcs);
      _externalFunctions->insert(i, _father->getAsFunction());
    }
    else
    {
      _father = newMMO_Model(*x->name());
      _father->getAsModel()->setExternalFunctions(_externalFunctions);
    }
    _class = _father;
    _className = _fatherName;
    _classPrefix = _fatherPrefix;
  }
}

/**
 * @brief Leave the class and set the corresponding father and child pointers if needed.
 *
 * When leaving the class, if the class has a father then reset the child pointer and
 * child prefix fields and add the child (that must be a function) into the father's children
 * list.
 *
 * @param x @ref AST_Class to visit.
 *
 */
void
MMO_MicroModelicaIR_::leave(AST_Class x)
{
  if(_father != NULL && _child != NULL)
  {
    _childName = NULL;
    _childPrefix = 0;
    _father->insert(_child->getAsFunction());
    Error::getInstance()->setClassName(*(_fatherName));
    _child = NULL;
    _class = _father;
    _className = _fatherName;
    _classPrefix = _fatherPrefix;
  }
  else
  {
    _fatherName = NULL;
    _fatherPrefix = 0;
    _storedDefinition->addClass(_class);
    _father = NULL;
    _class = NULL;
  }
}

void
MMO_MicroModelicaIR_::visit(AST_Composition x)
{
}

void
MMO_MicroModelicaIR_::leave(AST_Composition x)
{
  if(_class->classType() == CL_MODEL)
  {
    MMO_Model m = _class->getAsModel();
    bool setEqs = false;
    if(x->hasCompositionList())
    {
      AST_CompositionElementList el = x->compositionList();
      AST_CompositionElementListIterator it;
      foreach(it,el)
      {
        if(current_element(it)->hasEquations())
        {
          setEqs = true;
          break;
        }
      }
    }
    if(setEqs)
    {
      m->setEquations();
    }
    m->setEvents();
  }
}

void
MMO_MicroModelicaIR_::visit(AST_CompositionElement x)
{
  _compositionElement = true;
}

void
MMO_MicroModelicaIR_::leave(AST_CompositionElement x)
{
  _compositionElement = false;
}

void
MMO_MicroModelicaIR_::visit(AST_CompositionEqsAlgs x)
{
  _initialCode = x->isInitial();
}

void
MMO_MicroModelicaIR_::leave(AST_CompositionEqsAlgs x)
{
  _initialCode = false;
}

void
MMO_MicroModelicaIR_::visit(AST_External_Function_Call x)
{
  _class->insert(x);
}

void
MMO_MicroModelicaIR_::_insertComponent(AST_Element_Component x)
{
  AST_Element_Component c = x->getAsComponent();
  AST_TypePrefix tp = c->typePrefix();
  AST_DeclarationListReverseIterator it;
  AST_DeclarationList dl = c->nameList();
  foreachReverse(it,dl)
  {
    vector<int> size;
    bool array = current_element(it)->hasIndexes();
    if(array)
    {
      MMO_EvalInitExp_ e(_class->varTable());
      AST_ExpressionList elist = current_element(it)->indexes();
      AST_ExpressionListIterator elistit;
      foreach (elistit, elist)
      {
        size.push_back(e.foldTraverse(current_element(elistit)));
      }
    }
    else
    {
      size.push_back(1);
    }

    DEC_Type t = DEC_PUBLIC;
    if(_compositionElement)
    {
      t = DEC_LOCAL;
    }
    if(tp & TP_CONSTANT)
    {
      _class->insert(
      current_element(it)->name(),
          newVarInfo(newType_Integer(), tp,
          current_element(it)->modification(),
          NULL, size, array),
          t);
    }
    else
    {
      if((tp & TP_PARAMETER) && c->isInteger())
      {
        _class->insert(
        current_element(it)->name(),
            newVarInfo(newType_Integer(), tp,
            current_element(it)->modification(),
            NULL, size, array),
            t);
      }
      else if (c->isString())
      {
        _class->insert(
        current_element(it)->name(),
            newVarInfo(newType_String(), tp,
            current_element(it)->modification(),
            NULL, size, array),
            t);
      }
      else
      {
        _class->insert(
        current_element(it)->name(),
            newVarInfo(newType_Real(), tp,
            current_element(it)->modification(),
            NULL, size, array),
            t);
      }
    }
  }
}

void
MMO_MicroModelicaIR_::visit(AST_Element x)
{
  ElementType e = x->elementType();

  if(e == IMPORT)
  {
    AST_Element_ImportClause i = x->getAsImportClause();
    _class->insert(i->name());
  }
  else if(e == COMPONENT)
  {
    AST_Element_Component c = x->getAsComponent();
    _insertComponent(c);
  }
  else if(e == ELCLASS)
  {
    AST_Class c = x->getAsClassWrapper()->getClass();
    c->accept(this);
  }
}

void
MMO_MicroModelicaIR_::visit(AST_Modification x)
{
  if(x->modificationType() == MODASSIGN)
  {
    Error::getInstance()->add(x->lineNum(), EM_AST | EM_CLASS_DEFINITION,
        ER_Error, _className->c_str(), "Assign modifier.");
  }
  if(x->modificationType() == MODCLASS)
  {
    _classModification = true;
  }
}

void
MMO_MicroModelicaIR_::leave(AST_Modification x)
{
  if(x->modificationType() == MODCLASS)
  {
    _classModification = false;
  }
}

void
MMO_MicroModelicaIR_::visit(AST_Comment x)
{
}

bool
MMO_MicroModelicaIR_::_lValue(AST_Expression left)
{
  ExpressionType et = left->expressionType();
  if(et == EXPCOMPREF || et == EXPDERIVATIVE || et == EXPOUTPUT)
  {
    return true;
  }
  return false;
}

void
MMO_MicroModelicaIR_::visit(AST_Equation x)
{
  if(x->equationType() == EQFOR)
  {
    AST_ListFirst(x->getAsFor()->forIndexList())->accept(this);
  }
  _class->insert(x);
}

void
MMO_MicroModelicaIR_::visit(AST_ForIndex x)
{
  _class->insert(*x->variable(),
      newVarInfo(newType_Integer(), TP_FOR, NULL, NULL, vector<int>(1, 1),
          false));
}

void
MMO_MicroModelicaIR_::visit(AST_Equation_Else x)
{
}

void
MMO_MicroModelicaIR_::visit(AST_Expression x)
{
}

void
MMO_MicroModelicaIR_::visit(AST_Argument x)
{
  if(x->argumentType() == AR_MODIFICATION)
  {
    AST_Argument_Modification am = x->getAsModification();
    if(am->hasModification() && _classModification == false)
    {
      _class->insert(am);
    }
  }
}

bool
MMO_MicroModelicaIR_::_whenStatement(AST_Expression cond)
{
  if(cond->expressionType() != EXPBINOP)
  {
    return false;
  }
  BinOpType bot = cond->getAsBinOp()->binopType();
  if(bot != BINOPLOWER && bot != BINOPLOWEREQ && bot != BINOPGREATER
      && bot != BINOPGREATEREQ)
  {
    return false;
  }
  return true;
}

void
MMO_MicroModelicaIR_::visit(AST_Statement x)
{
  if(x->statementType() == STFOR)
  {
    visit(AST_ListFirst(x->getAsFor()->forIndexList()));
  }
  _class->insert(x, _initialCode);
}

void
MMO_MicroModelicaIR_::leave(AST_Statement x)
{
  if(x->statementType() == STWHEN)
  {
    if(x->getAsWhen()->hasElsewhen())
    {
      _elseWhen = false;
    }
  }
}

void
MMO_MicroModelicaIR_::visit(AST_Statement_Else x)
{
  if(_elseWhen == true)
  {
    _whenStatement(x->condition());
  }
  AST_StatementList stl = x->statements();
  AST_StatementListIterator sti;
  foreach(sti,stl)
  {
    visit(current_element(sti));
  }
}

void
MMO_MicroModelicaIR_::visit(AST_StoredDefinition x)
{
  _storedDefinition = newMMO_StoredDefinition();
}

void
MMO_MicroModelicaIR_::leave(AST_StoredDefinition x)
{
}

int
MMO_MicroModelicaIR_::apply(AST_Node x)
{
  x->accept(this);
  return Error::getInstance()->errors();
}

MMO_StoredDefinition
MMO_MicroModelicaIR_::storedDefinition()
{
  return _storedDefinition;
}

list<MMO_Class>
MMO_MicroModelicaIR_::classes() const
{
  return _storedDefinition->classes();
}

MMO_MicroModelicaIR
newMMO_MicroModelicaIR(string name)
{
  return new MMO_MicroModelicaIR_(name);
}

void
deleteMMO_MicroModelicaIR(MMO_MicroModelicaIR m)
{
  delete m;
}
