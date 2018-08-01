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

#include "mmo_model_checker.h"

#include "../ast/class.h"
#include "../ast/composition.h"
#include "../ast/element.h"
#include "../ast/equation.h"
#include "../ast/expression.h"
#include "../ast/modification.h"
#include "../ast/statement.h"
#include "../ast/stored_definition.h"
#include "../util/error.h"
#include "../util/util.h"

using namespace MicroModelica::Util;

namespace MicroModelica {
  namespace IR {

    /* MicroModelica model checker interface */

    ModelChecker::ModelChecker(string name) :
      _isChild(false),
      _className(),
      _classPrefix(0),
      _classModification(false)
    {
    }

    ModelChecker::~ModelChecker()
    {
    }

    void
    ModelChecker::visit(AST_Class x)
    {
      Error::getInstance()->setClassName(*(x->name()));
      if(x->isFinal())
      {
        Error::getInstance()->add(x->lineNum(), EM_AST | EM_CLASS_DEFINITION, ER_Error, "Final definition.");
      }
      if(x->isEncapsulated())
      {
        Error::getInstance()->add(x->lineNum(), EM_AST | EM_CLASS_DEFINITION, ER_Error, "Encapsulated definition.");
      }
      if(x->hasExtends())
      {
        Error::getInstance()->add(x->lineNum(), EM_AST | EM_CLASS_DEFINITION, ER_Error, "Extend modifier to Class definition.");
      }
      if(_isChild)
      {
        if(x->prefix() != CP_FUNCTION || x->prefix() != CP_IMPURE || x->prefix() != CP_PURE)
        {
          Error::getInstance()->add(x->lineNum(), EM_AST | EM_CLASS_DEFINITION, ER_Error, "Only Function classes allowed.");
        }
      }
      else
      {
        _classPrefix = x->prefix();
        switch(_classPrefix)
        {
          case CP_PARTIAL:
            Error::getInstance()->add(x->lineNum(), EM_AST | EM_CLASS_DEFINITION, ER_Error, "Partial definition.");
            break;
          case CP_CLASS:
            Error::getInstance()->add(x->lineNum(), EM_AST | EM_CLASS_DEFINITION, ER_Error, "Class definition.");
            break;
          case CP_BLOCK:
            Error::getInstance()->add(x->lineNum(), EM_AST | EM_CLASS_DEFINITION, ER_Error, "Block definition.");
            break;
          case CP_RECORD:
            Error::getInstance()->add(x->lineNum(), EM_AST | EM_CLASS_DEFINITION, ER_Error, "Record definition.");
            break;
          case CP_CONNECTOR:
            Error::getInstance()->add(x->lineNum(), EM_AST | EM_CLASS_DEFINITION, ER_Error, "Connector definition.");
            break;
          case CP_TYPE:
            Error::getInstance()->add(x->lineNum(), EM_AST | EM_CLASS_DEFINITION, ER_Error, "Type definition.");
            break;
          case CP_OPERATOR:
            Error::getInstance()->add(x->lineNum(), EM_AST | EM_CLASS_DEFINITION, ER_Error, "Operator definition.");
            break;
          case CP_EXPANDABLE:
            Error::getInstance()->add(x->lineNum(), EM_AST | EM_CLASS_DEFINITION, ER_Error, "Expandable definition.");
            break;
          default:
            break;
        }
        if(x->hasElementComponentList())
        {
          Error::getInstance()->add(x->lineNum(), EM_AST | EM_CLASS_DEFINITION, ER_Error, "Class definition.");
        }
      }
    }

    void
    ModelChecker::leave(AST_Class x)
    {
    }

    void
    ModelChecker::visit(AST_Composition x)
    {
      if(_classPrefix == CP_FUNCTION || _classPrefix == CP_PURE || _classPrefix == CP_IMPURE)
      {
        if(x->hasCompositionList())
        {
          AST_CompositionElementListIterator it;
          AST_CompositionElementList cl = x->compositionList();
          foreach(it,cl)
          {
            if(current_element(it)->hasEquations())
            {
              Error::getInstance()->add(x->lineNum(), EM_AST | EM_CLASS_DEFINITION, ER_Error, "Equation section inside function definition.");
            }
          }
        }
      }
      else if(_classPrefix == CP_MODEL)
      {
        if(x->hasExternalFunctionCall())
        {
          Error::getInstance()->add(x->lineNum(), EM_AST | EM_CLASS_DEFINITION, ER_Error, "External function call inside Model class.");
        }
      }
      else if(_classPrefix == CP_PACKAGE)
      {
        if(x->hasCompositionList())
        {
          Error::getInstance()->add(x->lineNum(), EM_AST | EM_CLASS_DEFINITION, ER_Error, "Composition elements inside Package class.");
        }
        if(x->hasExternalFunctionCall())
        {
          Error::getInstance()->add(x->lineNum(), EM_AST | EM_CLASS_DEFINITION, ER_Error, "External function call inside Package class.");
        }
      }
    }

    void
    ModelChecker::leave(AST_Composition x)
    {
    }

    void
    ModelChecker::visit(AST_CompositionElement x)
    {
      if(_classPrefix == CP_MODEL)
      {
        if(x->hasElements())
        {
          Error::getInstance()->add(x->lineNum(), EM_AST | EM_CLASS_DEFINITION, ER_Error, "Protected element definition inside Model Class.");
        }
      }
    }

    void
    ModelChecker::leave(AST_CompositionElement x)
    {
    }

    void
    ModelChecker::visit(AST_CompositionEqsAlgs x)
    {
      if(_classPrefix == CP_MODEL)
      {
        if(x->hasEquations() && x->isInitial())
        {
          Error::getInstance()->add(x->lineNum(), EM_AST | EM_CLASS_DEFINITION, ER_Error, "Initial Equation section inside Model Class.");
        }
      }
      if(_classPrefix == CP_FUNCTION || _classPrefix == CP_PURE || _classPrefix == CP_IMPURE)
      {
        if(x->isInitial())
        {
          Error::getInstance()->add(x->lineNum(), EM_AST | EM_CLASS_DEFINITION, ER_Error, "Initial section inside function definition.");
        }
      }
    }

    void
    ModelChecker::leave(AST_CompositionEqsAlgs x)
    {
    }

    void
    ModelChecker::visit(AST_External_Function_Call x)
    {
      if(x->languageString() != MicroModelica::Util::Util::getInstance()->languageEspecification())
      {
        Error::getInstance()->add(x->lineNum(), EM_AST | EM_CLASS_DEFINITION,
            ER_Error, "Language Specification not recognized.");
      }
    }

    void
    ModelChecker::visit(AST_Element x)
    {
      ElementType e = x->elementType();
      if(e == EXTENDS)
      {
        Error::getInstance()->add(x->lineNum(), EM_AST | EM_CLASS_DEFINITION, ER_Error, "Extends clause not allowed.");
      }
      if(e == COMPONENT)
      {
        AST_Element_Component c = x->getAsComponent();
        AST_DeclarationListIterator it;
        AST_TypePrefix tp = c->typePrefix();
        AST_DeclarationList dl = c->nameList();
        foreach(it,dl)
        {
          if(MicroModelica::Util::Util::getInstance()->checkBuiltInFunctions( current_element(it)->name()) != BIF_NONE)
          {
            Error::getInstance()->add(x->lineNum(), EM_AST | EM_CLASS_DEFINITION, ER_Error, "Reserved word used in variable name: %s.",
                current_element(it)->name().c_str());
          }
          if(current_element(it)->hasModification() && !(tp & TP_PARAMETER) && !(tp & TP_CONSTANT))
          {
            if(current_element(it)->modification()->modificationType() == MODEQUAL)
            {
              Error::getInstance()->add(x->lineNum(), EM_AST | EM_CLASS_DEFINITION, ER_Error, "Only parameters or constants can be assigned.");
            }
          }
        }
        if(c->hasIndexes())
        {
          Error::getInstance()->add(x->lineNum(), EM_AST | EM_CLASS_DEFINITION, ER_Error, "Array subscripts modifiers in component.");
        }
        if(c->isStream())
        {
          Error::getInstance()->add(x->lineNum(), EM_AST | EM_CLASS_DEFINITION, ER_Error, "Stream type prefixes not allowed.");
        }
        if(c->isFlow())
        {
          Error::getInstance()->add(x->lineNum(), EM_AST | EM_CLASS_DEFINITION, ER_Error, "Flow type prefixes not allowed.");
        }
        if(c->isConstant())
        {
          AST_DeclarationListIterator it;
          AST_DeclarationList dl = c->nameList();
          foreach(it,dl)
          {
            if(current_element(it)->hasModification())
            {
              AST_Modification m = current_element(it)->modification();
              if(m->modificationType() == MODCLASS)
              {
                AST_ArgumentListIterator it;
                AST_Expression asgExp = m->getAsClass()->exp();
                if(asgExp->expressionType() == EXPNULL)
                {
                  Error::getInstance()->add(x->lineNum(), EM_AST | EM_CLASS_DEFINITION, ER_Error, "Wrong constant definition %s.", c->name().c_str());
                }
              }
            }
            else
            {
              Error::getInstance()->add(x->lineNum(), EM_AST | EM_CLASS_DEFINITION, ER_Error, "Wrong constant definition %s.", c->name().c_str());
            }
          }
        }
        if(!MicroModelica::Util::Util::getInstance()->checkTypeString(c->type()))
        {
          Error::getInstance()->add(x->lineNum(), EM_AST | EM_CLASS_DEFINITION, ER_Error, "Predefined type not recognized.");
        }
      }
      if(_classPrefix == CP_PACKAGE)
      {
        if(e == ELCLASS)
        {
          AST_ClassPrefix c = x->getAsClassWrapper()->getClass()->prefix();
          if(!(c & CP_FUNCTION) && !(c & CP_PURE) && !(c & CP_IMPURE))
          {
            Error::getInstance()->add(x->lineNum(), EM_AST | EM_CLASS_DEFINITION, ER_Error, "Only function class definitions allowed inside Package class.");
          }
        }
      }
      if(e == ELCLASS)
      {
        AST_Class c = x->getAsClassWrapper()->getClass();
        visit(c);
      }
    }

    void
    ModelChecker::visit(AST_Modification x)
    {
      if(x->modificationType() == MODASSIGN)
      {
        Error::getInstance()->add(x->lineNum(), EM_AST | EM_CLASS_DEFINITION, ER_Error, "Assign modifier.");
      }
      if(x->modificationType() == MODCLASS)
      {
        _classModification = true;
      }
    }

    void
    ModelChecker::leave(AST_Modification x)
    {
      if(x->modificationType() == MODCLASS)
      {
        _classModification = false;
      }
    }

    void
    ModelChecker::visit(AST_Comment x)
    {
    }

    bool
    ModelChecker::_lValue(AST_Expression left)
    {
      ExpressionType et = left->expressionType();
      if(et == EXPCOMPREF || et == EXPDERIVATIVE || et == EXPOUTPUT)
      {
        if(et == EXPDERIVATIVE)
        {
          AST_ExpressionList el = left->getAsDerivative()->arguments();
          if(el->size() > 1)
          {
            Error::getInstance()->add(left->lineNum(), EM_AST | EM_CLASS_DEFINITION, ER_Error, "Wrong LValue expression.");
            return false;
          }
          AST_Expression e = AST_ListFirst(el);
          if(e->expressionType() != EXPCOMPREF)
          {
            Error::getInstance()->add(e->lineNum(), EM_AST | EM_CLASS_DEFINITION, ER_Error, "Wrong LValue expression.");
            return false;
          }
          AST_Expression_ComponentReference cr = e->getAsComponentReference();
          if(cr->hasIndexes())
          {
            AST_ExpressionList ell = AST_ListFirst(cr->indexes());
            AST_ExpressionListIterator elit;
            foreach(elit, ell)
            {
              AST_Expression exp = current_element(elit);
              ExpressionType et = exp->expressionType();
              if(et != EXPCOMPREF && et != EXPINTEGER && et != EXPBINOP)
              {
                Error::getInstance()->add(exp->lineNum(), EM_AST | EM_CLASS_DEFINITION, ER_Error, "Wrong index formula.");
              }
            }

          }
        }
        else if(et == EXPOUTPUT)
        {
          AST_Expression_Output eout = left->getAsOutput();
          AST_ExpressionList el = eout->expressionList();
          AST_ExpressionListIterator it;
          foreach(it,el)
          {
            if(current_element(it)->expressionType() != EXPCOMPREF)
            {
              Error::getInstance()->add(current_element(it)->lineNum(), EM_AST | EM_CLASS_DEFINITION, ER_Error, "Wrong algebraic expression.");
              return false;
            }
          }
        }
        return true;
      }
      return false;
    }

    void
    ModelChecker::visit(AST_Equation x)
    {
      EquationType e = x->equationType();
      if(e == EQCALL || e == EQCONNECT || e == EQWHEN || e == EQIF)
      {
        Error::getInstance()->add(x->lineNum(), EM_AST | EM_CLASS_DEFINITION, ER_Error, "Equation type not allowed %d.", e);
      }
      if(e == EQEQUALITY)
      {
        AST_Equation_Equality ec = x->getAsEquality();
        if(!_lValue(ec->left()))
        {
          Error::getInstance()->add(x->lineNum(), EM_AST | EM_CLASS_DEFINITION,
              ER_Error, "Wrong left hand side equation.");
        }
        visit(ec->right());
      }
      if(e == EQFOR)
      {
        AST_Equation_For ef = x->getAsFor();
        if(ef->forIndexList()->size() == 0)
        {
          Error::getInstance()->add(x->lineNum(), EM_AST | EM_CLASS_DEFINITION,
              ER_Error, "No index found in For equation.");
        }
        AST_ForIndexList fil = ef->forIndexList();
        AST_ForIndexListIterator filit;
        foreach(filit, fil)
        {
          visit(current_element(filit));
        }
        AST_EquationList eql = ef->equationList();
        AST_EquationListIterator eq;
        foreach(eq,eql)
        {
          visit(current_element(eq));
        }
      }
    }

    void
    ModelChecker::visit(AST_ForIndex x)
    {
      if(x->in_exp()->expressionType() != EXPRANGE)
      {
        Error::getInstance()->add(x->lineNum(), EM_AST | EM_CLASS_DEFINITION, ER_Error, "Wrong For index expression.");
        AST_Expression_Range er = x->in_exp()->getAsRange();
        if(er->expressionList()->size() > 2)
        {
          Error::getInstance()->add(x->lineNum(), EM_AST | EM_CLASS_DEFINITION, ER_Error, "Wrong For index expression.");
        }
      }
    }

    void
    ModelChecker::visit(AST_Equation_Else x)
    {
      Error::getInstance()->add(x->lineNum(), EM_AST | EM_CLASS_DEFINITION, ER_Error, "If equation.");
    }

    void
    ModelChecker::visit(AST_Expression x)
    {
      ExpressionType et = x->expressionType();
      switch(et)
      {
        case EXPIF:
          case EXPELSEIF:
          Error::getInstance()->add(x->lineNum(), EM_AST | EM_CLASS_DEFINITION, ER_Error, "If equation not allowed.");
          break;
        case EXPCOMPREF:
          {
          AST_Expression_ComponentReference cr = x->getAsComponentReference();
          if(cr->names()->size() > 1)
          {
            Error::getInstance()->add(x->lineNum(), EM_AST | EM_CLASS_DEFINITION, ER_Error, "Wrong Component reference.");
          }
          AST_ExpressionList el = cr->indexes()->front();
          if(el->size() > 0)
          {
            visit(el->front());
          }
          break;
        }
        default:
          break;
      }
    }

    void
    ModelChecker::visit(AST_Argument x)
    {
      if(x->argumentType() == AR_REDECLARATION)
      {
        Error::getInstance()->add(x->lineNum(), EM_AST | EM_DEFINITION_NOT_ALLOWED, ER_Error, "Argument Re-declaration.");
      }
      if(x->isFinal())
      {
        Error::getInstance()->add(x->lineNum(), EM_AST | EM_DEFINITION_NOT_ALLOWED, ER_Error, "Final modifier.");
      }
      if(x->argumentType() == AR_MODIFICATION)
      {
        AST_Argument_Modification am = x->getAsModification();
        if(am->hasModification() && _classModification == false)
        {
          if(MicroModelica::Util::Util::getInstance()->checkExperimentAnnotations(am->name()))
          {
            Error::getInstance()->add(x->lineNum(), EM_AST | EM_DEFINITION_NOT_ALLOWED, ER_Error, "Annotation not recognized.");
          }
        }
      }
    }
    
    bool
    ModelChecker::_whenStatement(AST_Expression cond)
    {
      AST_Expression exp = cond;
      if(cond->expressionType() == EXPOUTPUT)
      {
        exp = AST_ListFirst(cond->getAsOutput()->expressionList());
      }
      if(exp->expressionType() != EXPBINOP)
      {
        return false;
      }
      BinOpType bot = exp->getAsBinOp()->binopType();
      if(bot != BINOPLOWER && bot != BINOPLOWEREQ && bot != BINOPGREATER
          && bot != BINOPGREATEREQ)
      {
        return false;
      }
      return true;
    }

    void
    ModelChecker::visit(AST_Statement x)
    {
      StatementType st = x->statementType();
      switch(st)
      {
        case STWHILE:
          Error::getInstance()->add(x->lineNum(), EM_AST | EM_DEFINITION_NOT_ALLOWED, ER_Error, "While statement.");
          break;
        case STBREAK:
          Error::getInstance()->add(x->lineNum(), EM_AST | EM_DEFINITION_NOT_ALLOWED, ER_Error, "Break statement.");
          break;
        case STOUTASSING:
          Error::getInstance()->add(x->lineNum(), EM_AST | EM_DEFINITION_NOT_ALLOWED, ER_Error, "Output assignment statement.");
          break;
        case STFOR:
          {
            AST_Statement_For ef = x->getAsFor();
            if(ef->forIndexList()->size() == 0)
            {
              Error::getInstance()->add(x->lineNum(), EM_AST | EM_CLASS_DEFINITION, ER_Error, "No index found in for statement.");
            }
            AST_ForIndexList fil = ef->forIndexList();
            AST_ForIndexListIterator filit;
            foreach (filit,fil)
            {
              visit(current_element(filit));
            }
            AST_StatementList eql = ef->statements();
            AST_StatementListIterator eq;
            foreach(eq,eql)
            {
              visit(current_element(eq));
            }
            break;
          }
        case STRETURN:
          if(_classPrefix != CP_PURE && _classPrefix != CP_IMPURE && _classPrefix != CP_FUNCTION)
          {
            Error::getInstance()->add(x->lineNum(), EM_AST | EM_CLASS_DEFINITION, ER_Error, "return statement outside function class.");
          }
          break;
        case STWHEN:
          {
            AST_Statement_When sw = x->getAsWhen();
            if(!_whenStatement(sw->condition()))
            {
              Error::getInstance()->add(x->lineNum(), EM_AST | EM_CLASS_DEFINITION, ER_Error, "When condition must be a simple relation.");
            }
            AST_StatementList stl = sw->statements();
            AST_StatementListIterator st;
            foreach(st,stl)
            {
              visit(current_element(st));
            }
            if(sw->hasElsewhen())
            {
              _elseWhen = true;
              AST_Statement_ElseList ell = sw->else_when();
              AST_Statement_ElseListIterator elt;
              foreach(elt,ell)
              {
                visit(current_element(elt));
              }
            }
            break;
          }
        default:
          break;
      }
    }

    void
    ModelChecker::leave(AST_Statement x)
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
    ModelChecker::visit(AST_Statement_Else x)
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
    ModelChecker::visit(AST_StoredDefinition x)
    {
      if(x->hasWithin())
      {
        Error::getInstance()->add(x->lineNum(), EM_AST | EM_DEFINITION_NOT_ALLOWED, ER_Error, "Within inside stored definition.");
      }
    }

    void
    ModelChecker::leave(AST_StoredDefinition x)
    {
      Error::getInstance()->show();
    }

    int
    ModelChecker::apply(AST_Node x)
    {
      x->accept(this);
      return Error::getInstance()->errors();
    }
  }
}
