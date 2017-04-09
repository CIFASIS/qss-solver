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

#include "mmo_model.h"

#include <sbml/Delay.h>
#include <sbml/EventAssignment.h>
#include <sbml/Trigger.h>
#include <sstream>

#include "biorica_writer.h"
#include "mmo_event.h"
#include "mmo_function.h"
#include "mmo_math.h"
#include "mmo_utils.h"

MMOModel::MMOModel (string prefix, bool replace, WR_Type type) :
    _replace (replace), _type (type), _imports (), _functions (), _variableParameters (), _prefix (), _constantAlgebraics (), _name (), _variables ()
{
  if (_replace)
    {
      _prefix = prefix;
    }
  _importSec = MMOSection (SEC_IMPORTS);
  _equation = MMOSection (SEC_EQUATION);
  _algorithm = MMOSection (SEC_ALGORITHM);
  _initial_algorithm = MMOSection (SEC_INITIAL_ALGORITHM);
  _external_functions = MMOSection (SEC_EXTERNAL_FUNCTIONS);
  _declarations = MMOSection (SEC_DECLARATIONS);
  MMOUtils::getInstance ()->bioricaLanguage (_replace);
}

MMOModel::MMOModel (string name) :
    _replace (false), _type (WR_MMO), _imports (), _functions (), _variableParameters (), _prefix (), _constantAlgebraics (), _name (), _variables ()
{
}

MMOModel::~MMOModel ()
{
}

void
MMOModel::write (string file)
{
  MMOVisitor *writer;
  if (_type == WR_MMO)
    {
      writer = new MMOWriter (file);
    }
  else
    {
      writer = new BioRicaWriter (file);
    }
  _addConstantAlgebraics ();
  _reorder ();
  _importSec.accept (writer);
  _external_functions.accept (writer);
  _declarations.accept (writer);
  _initial_algorithm.accept (writer);
  _algorithm.setInitialValues (true);
  _algorithm.accept (writer);
  _algorithm.setInitialValues (false);
  _equation.accept (writer);
  _algorithm.accept (writer);
  delete writer;
}

void
MMOModel::_addAlgebraicReplacement (MMOMath& eq)
{
  for (pair<string, pair<string, list<string> > > def = eq.firstAlgebraic ();
      !eq.endAlgebraic (); def = eq.nextAlgebraic ())
    {
      MMODecl* exp = new MMODecl (def.first, def.second.first,
				  DEC_ALGEBRAIC_EQUATION);
      _variables.insert (
	  pair<string, list<string> > (def.first, def.second.second));
      _add (exp, SEC_EQUATION);
    }
  for (string imp = eq.firstImport (); !eq.endImport (); imp = eq.nextImport ())
    {
      if (_imports.find (imp) == _imports.end ())
	{
	  MMODecl* exp = new MMODecl (imp, DEC_IMPORT);
	  _add (exp, SEC_IMPORTS);
	}
    }
}

void
MMOModel::addExpressionZeroCrossings (MMOMath eq, const string& id)
{
  if (eq.hasZeroCrossing ())
    {
      static MMOUtils* utils = MMOUtils::getInstance ();
      pair<ASTNode*, list<pair<string, ASTNode*> > > it;
      for (it = eq.firstEvent (); !eq.endEvent (); it = eq.nextEvent ())
	{
	  MMOMath zc = MMOMath (true, &_functions, _prefix);
	  zc.parseZeroCrossing (it.first);
	  addExpressionZeroCrossings (zc, "");
	  pair<string, string> zc_it;
	  int ev_num = 0;
	  for (zc_it = zc.firstZC (); !zc.endZC (); zc_it = zc.nextZC ())
	    {
	      MMOEvent* ev = new MMOEvent (utils->getVar ());
	      MMODecl dec = MMODecl (zc_it.first, DEC_ZC_RELATION);
	      ev->add (dec);
	      if (!zc_it.second.empty ())
		{
		  MMODecl cond = MMODecl (zc_it.second, DEC_CONDITION);
		  ev->add (cond);
		}
	      list<pair<string, ASTNode*> >::iterator asg_it;
	      for (asg_it = it.second.begin (); asg_it != it.second.end ();
		  asg_it++)
		{
		  string asgVar;
		  if (asg_it->first == "")
		    {
		      MMODecl* dec = _declarations.findDec (id);
		      dec->setType (DEC_DISCRETE);
		      asgVar = id;
		    }
		  else
		    {
		      MMODecl* asg_vars = new MMODecl (asg_it->first,
						       DEC_DISCRETE);
		      _add (asg_vars, SEC_DECLARATIONS);
		      asgVar = asg_it->first;
		    }
		  MMOMath asg_exp = MMOMath (_replace, &_functions, _prefix);
		  asg_exp.parseAssignment (asg_it->second, asgVar);
		  MMODecl asg = MMODecl (asgVar, asg_exp.getExp (),
		      					 DEC_ASSIGNMENT, asg_exp.isConditional());
		  if (zc.isPositive (ev_num))
		    {
		      ev->add (asg, positive);
		    }
		  else
		    {
		      ev->add (asg, negative);
		    }
		}
	      ev_num++;
	      _add (ev, SEC_ALGORITHM);
	    }
	}
    }
}

void
MMOModel::_parseEquation (string id, const ASTNode *node, MMODeclType tydec,
			  MMOSectionType type)
{
  MMOMath eq = MMOMath (_replace, &_functions, _prefix);
  eq.parseEquation (node);
  _addAlgebraicReplacement (eq);
  if (eq.hasEquation ())
    {
      MMODecl *exp = new MMODecl (id, eq.getExp (), tydec);
      if (tydec == DEC_ALGEBRAIC_EQUATION)
	{
	  _variables.insert (
	      pair<string, list<string> > (id, eq.getVariables ()));
	}
      _add (exp, type);
      if (_variableParameters.find (id) != _variableParameters.end ())
	{
	  MMODecl *dec = _declarations.findDec (id);
	  if (!dec->isState ())
	    {
	      dec->setType (DEC_ALGEBRAIC);
	    }
	}
    }
  addExpressionZeroCrossings (eq, id);
}

void
MMOModel::_addAsgs (const Event &x, MMOMath zc, int pos, MMOEvent *ev)
{
  int asg_size = x.getListOfEventAssignments ()->size ();
  for (int i = 0; i < asg_size; i++)
    {
      const EventAssignment *ea = x.getEventAssignment (i);
      string asg_var = ea->getVariable ();
      MMOMath as = MMOMath (_replace, &_functions, _prefix);
      as.parseAssignment (new ASTNode (*ea->getMath ()), asg_var);
      _addAlgebraicReplacement (as);
      MMODecl asg_dec;
      MMODecl *_dis = _declarations.findDec (asg_var);
      if (_dis != NULL && _equation.findDec (asg_var) == NULL)
	{
          asg_dec = MMODecl (asg_var, as.getExp (), DEC_ASSIGNMENT, as.isConditional());
	  _dis->setType (DEC_DISCRETE);
	}
      else
	{
	  asg_dec = MMODecl (asg_var, as.getExp (), DEC_REINIT);
	}
      if (zc.isPositive (pos))
	{
	  ev->add (asg_dec, positive);
	}
      else
	{
	  ev->add (asg_dec, negative);
	}
    }
}

bool
MMOModel::_delayValue (const ASTNode *n)
{
  if (n->isReal () && n->getReal () == 0)
    {
      return (false);
    }
  return (true);
}

void
MMOModel::add (const Event &x)
{
  MMOMath zc = MMOMath (true, &_functions, _prefix);
  _addAlgebraicReplacement (zc);
  zc.parseZeroCrossing (new ASTNode (*x.getTrigger ()->getMath ()));
  bool setInitialValues = !(x.getTrigger ()->getInitialValue ());
  pair<string, string> zc_def;
  int _pos = 0;
  MMOMath delay_as = MMOMath (_replace, &_functions, _prefix);
  MMOMath dv_zc = MMOMath (true, &_functions, _prefix);
  string delay_var_str = "";
  if (x.isSetDelay () && _delayValue (x.getDelay ()->getMath ()))
    {
      static MMOUtils *utils = MMOUtils::getInstance ();
      // Create delay event.
      delay_var_str = "delay_" + x.getId ();
      MMOEvent *ev = new MMOEvent ("delay_" + x.getId (), setInitialValues);
      ASTNode *delay_zc = new ASTNode (AST_RELATIONAL_GEQ);
      ASTNode *delay_cte = new ASTNode (AST_INTEGER);
      delay_cte->setValue (0);
      ASTNode *delay_var = new ASTNode (AST_NAME);
      delay_var->setName (delay_var_str.c_str ());
      ASTNode *_time = new ASTNode (AST_NAME);
      _time->setName ("time");
      ASTNode *delay_minus = new ASTNode (AST_MINUS);
      delay_minus->addChild (delay_var);
      delay_minus->addChild (_time);
      delay_zc->addChild (delay_minus);
      delay_zc->addChild (delay_cte);
      MMODecl *delay_var_dec = new MMODecl (delay_var_str, 1e20, DEC_DISCRETE);
      _declarations.add (delay_var_dec);
      dv_zc.parseZeroCrossing (delay_zc);
      pair<string, string> zc_def = dv_zc.firstZC ();
      MMODecl zc_dec = MMODecl (zc_def.first, DEC_ZC_RELATION);
      ev->add (zc_dec);
      if (!x.getTrigger ()->isSetPersistent ())
	{
	  MMODecl cond = MMODecl (utils->getExp (delay_zc), DEC_CONDITION);
	  ev->add (cond);
	}
      ASTNode *delay_eq = new ASTNode (AST_PLUS);
      delay_eq->addChild (new ASTNode (*x.getDelay ()->getMath ()));
      delay_eq->addChild (_time);
      delay_as.parseAssignment (delay_eq, delay_var_str);
      _addAsgs (x, dv_zc, 0, ev);
      _algorithm.add (ev);
    }
  for (zc_def = zc.firstZC (); !zc.endZC (); zc_def = zc.nextZC ())
    {
      MMOEvent *ev = new MMOEvent (x.getId (), setInitialValues);
      MMODecl zc_dec = MMODecl (zc_def.first, DEC_ZC_RELATION);
      ev->add (zc_dec);
      if (!zc_def.second.empty ())
	{
	  MMODecl zc_cond = MMODecl (zc_def.second, DEC_CONDITION);
	  ev->add (zc_cond);
	}
      if (x.isSetDelay () && _delayValue (x.getDelay ()->getMath ()))
	{
	  MMODecl asg_dec;
	  asg_dec = MMODecl (delay_var_str, delay_as.getExp (), DEC_ASSIGNMENT, delay_as.isConditional());
	  ev->add (asg_dec, positive);
	}
      else
	{
	  _addAsgs (x, zc, _pos, ev);
	}
      _algorithm.add (ev);
      _pos++;
    }
}

void
MMOModel::add (string id, bool isDerivative, const ASTNode *value,
	       MMOSectionType type)
{
  MMODeclType t;
  MMODecl *dec = _declarations.findDec (id);
  if (dec == NULL)
    {
      t = DEC_IMPLICIT_EQUATION;
    }
  else
    {
      if (isDerivative)
	{
	  t = DEC_DERIVATIVE;
	  dec->setType (DEC_STATE);
	}
      else
	{
	  t = DEC_ALGEBRAIC_EQUATION;
	  dec->setType (DEC_ALGEBRAIC);
	}
    }
  if (type == SEC_EQUATION)
    {
      _parseEquation (id, value, t, type);
    }
  else if (type == SEC_INITIAL_ALGORITHM)
    {
      _parseEquation (id, value, DEC_INITIAL_ASSIGNMENT, type);
    }
}

void
MMOModel::add (string id, bool isConstant, double value, MMOSectionType type)
{
  MMODecl *exp = NULL;
  if (isConstant || (value != 0))
    {
      if (!isConstant)
	{
	  _variableParameters[id] = id;
	}
      exp = new MMODecl (id, value, DEC_PARAMETER);
    }
  else
    {
      exp = new MMODecl (id, value, DEC_ALGEBRAIC);
    }
  _add (exp, type);
}

void
MMOModel::add (string id, bool isVar, MMOSectionType type)
{
  MMODecl *exp = NULL;
  if (isVar)
    {
      exp = new MMODecl (id, DEC_PARAMETER);
    }
  else
    {
      exp = new MMODecl (id, DEC_ALGEBRAIC);
    }
  _add (exp, type);
}

void
MMOModel::_add (MMOExp *exp, MMOSectionType type)
{
  switch (type)
    {
    case SEC_EQUATION:
      _equation.add (exp);
      break;
    case SEC_ALGORITHM:
      _algorithm.add (exp);
      break;
    case SEC_INITIAL_ALGORITHM:
      _initial_algorithm.add (exp);
      break;
    case SEC_EXTERNAL_FUNCTIONS:
      _external_functions.add (exp);
      break;
    case SEC_DECLARATIONS:
      _declarations.add (exp);
      break;
    case SEC_IMPORTS:
      _importSec.add (exp);
    }
}

bool
MMOModel::_searchAlg (string f, list<string> *s)
{
  for (list<string>::iterator it = s->begin (); it != s->end (); it++)
    {
      if (f.compare (*it) == 0)
	{
	  return (true);
	}
    }
  return (false);
}

void
MMOModel::_addAlgs (list<string> algs, list<string> *equations)
{
  list<string>::iterator lit;
  list<string> l = algs;
  for (lit = l.begin (); lit != l.end (); lit++)
    {
      map<string, list<string> >::iterator f = _variables.find (*lit);
      if (f != _variables.end ())
	{
	  _addAlgs (f->second, equations);
	  if (!_searchAlg (*lit, equations))
	    {
	      equations->push_back (*lit);
	    }
	}
    }
}

void
MMOModel::_reorder ()
{
  list<string> equations;
  map<string, list<string> >::iterator it;
  for (it = _variables.begin (); it != _variables.end (); it++)
    {
      _addAlgs (it->second, &equations);
      if (!_searchAlg (it->first, &equations))
	{
	  equations.push_back (it->first);
	}
    }
  list<string>::iterator expIt;
  for (expIt = _constantAlgebraics.begin ();
      expIt != _constantAlgebraics.end (); expIt++)
    {
      equations.push_back (*expIt);
    }
  _equation.setAlgebraics (equations);
}

void
MMOModel::add (const FunctionDefinition &f)
{
  string fname = f.getId ();
  MMOFunction *func = new MMOFunction (fname);
  int na = f.getNumArguments ();
  MMOMath d = MMOMath (_replace, &_functions, _prefix);
  list<string> funcArgs;
  for (int i = 0; i < na; i++)
    {
      d.parseEquation (f.getArgument (i));
      string argExp = d.getExp ();
      MMODecl *adec = new MMODecl (argExp, DEC_FUNCTION_INPUT);
      func->add (adec);
      funcArgs.push_back (argExp);
    }
  string var = MMOUtils::getInstance ()->getVar ();
  d.parseEquation (f.getBody ());
  _addAlgebraicReplacement (d);
  MMODecl *adec = new MMODecl (var, d.getExp (), DEC_FUNCTION_DEFINITION);
  func->add (adec);
  _functions[fname] = pair<list<string>, ASTNode*> (
      funcArgs, new ASTNode (*f.getBody ()));
  _add (func, SEC_EXTERNAL_FUNCTIONS);
}

string
MMOModel::id ()
{
  return (_name);
}

void
MMOModel::_addConstantAlgebraics ()
{
  for (MMODecl *e = _declarations.first (); !_declarations.end (); e =
      _declarations.next ())
    {
      if (e->isAlgebraic ())
	{
	  if (_equation.findDec (e->getId ()) == NULL)
	    {
	      stringstream buffer;
	      buffer << e->getValue ();
	      MMODecl* exp = new MMODecl (e->getId (), buffer.str (),
					  DEC_ALGEBRAIC_EQUATION);
	      _add (exp, SEC_EQUATION);
	      _constantAlgebraics.push_back (e->getId ());
	    }
	}
    }
}
