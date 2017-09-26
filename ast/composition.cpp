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

#include "composition.h"

#include <iostream>
#include <list>

#include "../util/ast_util.h"
#include "ast_builder.h"
#include "element.h"
#include "equation.h"
#include "expression.h"
#include "modification.h"
#include "statement.h"

/* Composition Class */

AST_Composition_::AST_Composition_(AST_ElementList el,
    AST_CompositionElementList cl) :
    _composition_list(cl), _element_list(el), _annot(NULL), _ext(NULL)
{
}

CLASSP_PRINTER_IMP(AST_Composition);

AST_ElementList
AST_Composition_::elementList() const
{
  return _element_list;
}

AST_CompositionElementList
AST_Composition_::compositionList() const
{
  return _composition_list;
}

ostream &
operator<<(ostream &ret, const AST_Composition_ &cm)
{
  AST_CompositionElementListIterator it;
  AST_ElementListIterator el_it;
  foreach(el_it,cm.elementList())
    ret << current_element(el_it) << endl;
  foreach (it,cm.compositionList())
  {
    ret << current_element(it);
  }
  if(cm.externalCall() != NULL)
  {
    ret << "    external ";
    if(cm.externalCall()->language() != NULL)
    {
      ret << "\"" << cm.externalCall()->language() << "\"";
    }
    if(cm.externalCall()->annotation()->size())
    {
      AST_ListPrint(cm.externalCall()->annotation(), ret, "annotation(", ",",
          "", ")", true);
    }
    ret << ";" << endl;
  }
  return ret;
}

void
AST_Composition_::setExternalFunctionCall(AST_External_Function_Call ext)
{
  _ext = ext;
}

AST_External_Function_Call
AST_Composition_::externalCall() const
{
  return _ext;
}

void
AST_Composition_::setAnnotation(AST_ArgumentList al)
{
  _annot = al;
}

void
AST_Composition_::accept(AST_Visitor visitor)
{
  visitor->visit(this);
  AST_ElementListIterator _element_list_it;
  foreach(_element_list_it,_element_list)
  {
    current_element(_element_list_it)->accept(visitor);
  }
  AST_CompositionElementListIterator _composition_list_it;
  foreach(_composition_list_it,_composition_list)
  {
    if(current_element(_composition_list_it)->hasElements())
    {
      current_element(_composition_list_it)->accept(visitor);
    }
  }
  foreach(_composition_list_it,_composition_list)
  {
    if(current_element(_composition_list_it)->hasEquations())
    {
      current_element(_composition_list_it)->accept(visitor);
    }
  }
  foreach(_composition_list_it,_composition_list)
  {
    if(current_element(_composition_list_it)->hasStatements())
    {
      current_element(_composition_list_it)->accept(visitor);
    }
  }
  AST_ArgumentListIterator _annot_it;
  foreach(_annot_it,_annot)
  {
    current_element(_annot_it)->accept(visitor);
  }
  if(_ext != NULL)
  {
    _ext->accept(visitor);
  }
  visitor->leave(this);
}

bool
AST_Composition_::hasCompositionList()
{
  if(_composition_list == NULL)
  {
    return false;
  }
  return _composition_list->size() > 0;
}

bool
AST_Composition_::hasExternalFunctionCall()
{
  if(_ext == NULL)
  {
    return false;
  }
  return true;
}

bool
AST_Composition_::hasAnnotation()
{
  if(_annot == NULL)
  {
    return false;
  }
  return true;
}

/* Composition Element class */

AST_CompositionElement_::AST_CompositionElement_(
    AST_CompositionEqsAlgs eqs_algs) :
    _eqs_algs(eqs_algs), _el(newAST_ElementList())
{
}

AST_CompositionElement_::AST_CompositionElement_(AST_ElementList el) :
    _eqs_algs(newAST_NullCompositionEquations()), _el(el)
{
  AST_ElementListIterator it;
}

AST_CompositionEqsAlgs
AST_CompositionElement_::getEquationsAlgs()
{
  return _eqs_algs;
}

AST_ElementList
AST_CompositionElement_::getElementList()
{
  return _el;
}

ostream &
operator<<(ostream &ret, const AST_CompositionElement_ &ce)
{
  AST_EquationListIterator it;
  AST_StatementListIterator st_it;
  if(ce._eqs_algs != NULL)
  {
    if(ce._eqs_algs->getEquations()->size() > 0)
    {
      MAKE_SPACE
      ;
      ret << (ce._eqs_algs->isInitial() ? "initial " : "");
      ret << "equation" << endl;
    }
    BEGIN_BLOCK
    ;
    foreach (it,ce._eqs_algs->getEquations())
    {
      ret << current_element(it);
    }
    END_BLOCK
    ;
    if(ce._eqs_algs->getAlgorithms()->size() > 0)
    {
      MAKE_SPACE
      ;
      ret << (ce._eqs_algs->isInitial() ? "initial " : "");
      ret << "algorithm" << endl;
    }
    BEGIN_BLOCK
    ;
    foreach (st_it,ce._eqs_algs->getAlgorithms())
    {
      ret << current_element(st_it);
    }
    END_BLOCK
    ;
  }
  AST_ElementListIterator et;
  if(ce._el != NULL)
  {
    if(ce._el->size() > 0)
    {
      ret << "public" << endl;
    }
    foreach (et,ce._el)
    {
      ret << "  " << current_element(et) << endl;
    }
  }

  return ret;
}

void
AST_CompositionElement_::accept(AST_Visitor visitor)
{
  visitor->visit(this);
  if(_eqs_algs != NULL)
  {
    _eqs_algs->accept(visitor);
  }
  AST_ElementListIterator _el_it;
  foreach(_el_it,_el)
  {
    current_element(_el_it)->accept(visitor);
  }
  visitor->leave(this);
}

bool
AST_CompositionElement_::hasEquations()
{
  if(_eqs_algs == NULL)
  {
    return false;
  }
  return _eqs_algs->hasEquations();
}

bool
AST_CompositionElement_::hasStatements()
{
  if(_eqs_algs == NULL)
  {
    return false;
  }
  return _eqs_algs->hasStatements();
}

bool
AST_CompositionElement_::hasElements()
{
  if(_el == NULL)
  {
    return false;
  }
  return _el->size() > 0;
}

/* Composition Equations and Algorithm class */

AST_CompositionEqsAlgs_::AST_CompositionEqsAlgs_(AST_EquationList eq) :
    _initial(false), _eq(eq), _st(newAST_StatementList())
{
}

AST_CompositionEqsAlgs_::AST_CompositionEqsAlgs_(AST_EquationList eq, bool i) :
    _initial(i), _eq(eq), _st(newAST_StatementList())
{
}

AST_CompositionEqsAlgs_::AST_CompositionEqsAlgs_(AST_StatementList st, bool i) :
    _initial(i), _eq(newAST_EquationList()), _st(st)
{
}

AST_CompositionEqsAlgs_::AST_CompositionEqsAlgs_(AST_StatementList st) :
    _initial(false), _eq(newAST_EquationList()), _st(st)
{
}

AST_EquationList
AST_CompositionEqsAlgs_::getEquations()
{
  return _eq;
}

AST_StatementList
AST_CompositionEqsAlgs_::getAlgorithms()
{
  return _st;
}

bool
AST_CompositionEqsAlgs_::isInitial()
{
  return _initial;
}

void
AST_CompositionEqsAlgs_::accept(AST_Visitor visitor)
{
  visitor->visit(this);
  AST_EquationListIterator _eq_it;
  foreach(_eq_it,_eq)
  {
    current_element(_eq_it)->accept(visitor);
  }
  AST_StatementListIterator _st_it;
  foreach(_st_it,_st)
  {
    current_element(_st_it)->accept(visitor);
  }
  visitor->leave(this);
}

bool
AST_CompositionEqsAlgs_::hasEquations()
{
  if(_eq == NULL)
  {
    return false;
  }
  return _eq->size() > 0;
}

bool
AST_CompositionEqsAlgs_::hasStatements()
{
  if(_st == NULL)
  {
    return false;
  }
  return _st->size() > 0;
}

/* External function call class */

AST_String
AST_External_Function_Call_::language()
{
  return _lang;
}

string
AST_External_Function_Call_::languageString()
{
  return *_lang;
}

AST_ArgumentList
AST_External_Function_Call_::annotation()
{
  return _annot;
}

AST_External_Function_Call_::AST_External_Function_Call_(AST_String lang,
    AST_Expression_ComponentReference cr, AST_Expression args,
    AST_ArgumentList annot) :
    _lang(lang), _annot(annot), _exp(args), _cr(cr)
{
  _call = args->getAsCall();
}

AST_ExpressionList
AST_External_Function_Call_::args()
{
  return _call->arguments();
}

string
AST_External_Function_Call_::name()
{
  return *_call->name();
}

AST_Expression_ComponentReference
AST_External_Function_Call_::componentReference()
{
  return _cr;
}

bool
AST_External_Function_Call_::hasComponentReference()
{
  return _cr != NULL;
}

void
AST_External_Function_Call_::accept(AST_Visitor visitor)
{
  visitor->visit(this);
  AST_ArgumentListIterator _annot_it;
  foreach(_annot_it,_annot)
  {
    current_element(_annot_it)->accept(visitor);
  }
}
