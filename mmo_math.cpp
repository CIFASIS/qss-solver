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

#include "mmo_math.h"

#include "mmo_assignment.h"
#include "mmo_equation.h"
#include "mmo_utils.h"
#include "mmo_zerocrossing.h"

MMOMath::MMOMath (bool replace,
		  map<string, pair<list<string>, ASTNode*> > *functions,
		  string prefix) :
    _algebraics (), _currentAlgebraic (0), _type (MATH_EQUATION), _functions (
    NULL), _replace (replace), _imports (), _currentImport (0), _asgVariable(), _isConditional (false)
{
  if (_replace)
    {
      _functions = functions;
      _prefix = prefix;
    }
  _equation = NULL;
  _zerocrossing = NULL;
  _assignment = NULL;
  _exp = NULL;
}

MMOMath::~MMOMath ()
{
}

void
MMOMath::parseEquation (const ASTNode *node)
{
  _type = MATH_EQUATION;
  _exp = new ASTNode (*node);
  _processNode (_exp);
  if (_replace)
    {
      _replaceFunctions (_exp);
    }
  _equation = new MMOEquation (_exp);
}

void
MMOMath::parseZeroCrossing (ASTNode *node)
{
  _type = MATH_ZERO_CROSSING;
  _exp = new ASTNode (*node);
  _processNode (_exp);
  _replaceFunctions (_exp);
  _equation = new MMOEquation (_exp);
  _zerocrossing = new MMOZeroCrossing (_equation->ASTExpression ());
}

void
MMOMath::parseAssignment (ASTNode *node, string asgVariable)
{
  _type = MATH_ASSIGNMENT;
  _exp = new ASTNode (*node);
  _processNode (_exp);
  if (_replace)
    {
      _replaceFunctions (_exp);
    }
  _asgVariable = asgVariable;
  _assignment = new MMOAssignment (_exp);
}

bool
MMOMath::hasZeroCrossing ()
{
  if (_equation)
    {
      return (_equation->hasZeroCrossing ());
    }
  return (false);
}

bool
MMOMath::hasEquation ()
{
  if (_equation)
    {
      return (_equation->hasEquation ());
    }
  return (false);
}

bool
MMOMath::hasAssignment ()
{
  if (_assignment)
    {
      return (true);
    }
  return (false);
}

pair<ASTNode *, list<pair<string, ASTNode *> > >
MMOMath::firstEvent ()
{
  return (_equation->first ());
}

pair<ASTNode *, list<pair<string, ASTNode *> > >
MMOMath::nextEvent ()
{
  return (_equation->next ());
}

bool
MMOMath::endEvent ()
{
  return (_equation->end ());
}

pair<string, string>
MMOMath::firstZC ()
{
  return (_zerocrossing->first ());
}

pair<string, string>
MMOMath::nextZC ()
{
  return (_zerocrossing->next ());
}

bool
MMOMath::endZC ()
{
  return (_zerocrossing->end ());
}

string
MMOMath::firstAsg ()
{
  return ("");
}

string
MMOMath::nextAsg ()
{
  return ("");
}

bool
MMOMath::endAsg ()
{
  return ("");
}

pair<string, pair<string, list<string> > >
MMOMath::firstAlgebraic ()
{
  _currentAlgebraic = 0;
  map<string, pair<string, list<string> > >::iterator it = _algebraics.begin ();
  if (it == _algebraics.end ())
    {
      return (pair<string, pair<string, list<string> > > (
	  "", pair<string, list<string> > ("", list<string> ())));
    }
  return (*it);
}

pair<string, pair<string, list<string> > >
MMOMath::nextAlgebraic ()
{
  _currentAlgebraic++;
  map<string, pair<string, list<string> > >::iterator it = _algebraics.begin ();
  for (unsigned int i = 0; i < _currentAlgebraic; i++)
    {
      it++;
    }
  if (it == _algebraics.end ())
    {
      return (pair<string, pair<string, list<string> > > (
	  "", pair<string, list<string> > ("", list<string> ())));
    }
  return (*it);
}

bool
MMOMath::endAlgebraic ()
{
  return (_currentAlgebraic == _algebraics.size ());
}

void
MMOMath::_getVariables (const ASTNode *node, list<string> *ret)
{
  if (node->getType () == AST_NAME)
    {
      ret->push_back (node->getName ());
    }
  int childs = node->getNumChildren ();
  int i;
  for (i = 0; i < childs; i++)
    {
      _getVariables (node->getChild (i), ret);
    }
}

list<string>
MMOMath::getVariables ()
{
  list<string> ret;
  _getVariables (_exp, &ret);
  return (ret);
}

string
MMOMath::getExp ()
{
  ASTNode *e;
  if (_type == MATH_EQUATION)
    {
      e = _equation->getEquation ();
      _processNode (e);
      return (MMOUtils::getInstance ()->getExp (e));
    }
  if (_type == MATH_ASSIGNMENT)
    {
      e = _assignment->getAssignment ();
      _processNode (e);
      return (MMOUtils::getInstance ()->getExp (e, _asgVariable));
    }
  return ("");
}

bool
MMOMath::isPositive (int i)
{
  return (_zerocrossing->isPositive (i));
}

bool
MMOMath::hasImports ()
{
  return (_imports.size () > 0);
}

string
MMOMath::firstImport ()
{
  _currentImport = 0;
  map<string, string>::iterator it = _imports.begin ();
  if (it == _imports.end ())
    {
      return ("");
    }
  return (it->first);
}

string
MMOMath::nextImport ()
{
  _currentImport++;
  map<string, string>::iterator it = _imports.begin ();
  for (unsigned int i = 0; i < _currentImport; i++)
    {
      it++;
    }
  if (it == _imports.end ())
    {
      return ("");
    }
  return (it->first);

}

bool
MMOMath::endImport ()
{
  return (_currentImport == _imports.size ());
}

void
MMOMath::_processNode (ASTNode* node)
{
  ASTNodeType_t t = node->getType ();
  if (t == AST_FUNCTION_ROOT)
    {
      ASTNode *first = new ASTNode (*node->getChild (0));
      ASTNode *exp = new ASTNode (AST_DIVIDE);
      ASTNode *constant = new ASTNode (AST_REAL);
      constant->setValue (1);
      exp->addChild (constant);
      exp->addChild (first);
      node->setType (AST_POWER);
      node->removeChild (0);
      node->addChild (exp);
    }
  else if (t == AST_NAME && !_prefix.empty ()
      && node->getId ().compare ("REPLACED_FUNCTION"))
    {
      string controlName = node->getName ();
      string flatName = _prefix + "_";
      if (controlName.compare (0, flatName.size (), flatName))
	{
	  flatName.append (node->getName ());
	  node->setName (flatName.c_str ());
	}
    }
  if (t == AST_FUNCTION_PIECEWISE)
    {
      _isConditional = true;
    }
  string package = MMOUtils::getInstance ()->checkPredefinedFunctions (node);
  if (!package.empty ())
    {
      _imports[package] = package;
    }
  int childs = node->getNumChildren ();
  int i;
  for (i = 0; i < childs; i++)
    {
      _processNode (node->getChild (i));
    }
}

void
MMOMath::_replaceFunctions (ASTNode* node)
{
  if (node->isFunction ())
    {
      map<string, pair<list<string>, ASTNode*> >::iterator it =
	  _functions->find (node->getName ());
      if (it != _functions->end ())
	{
	  int childs = node->getNumChildren ();
	  int i;
	  string var = MMOUtils::getInstance ()->getVar ();
	  list<string> args;
	  for (i = 0; i < childs; i++)
	    {
	      string insertArg = "(";
	      insertArg.append (
		  MMOUtils::getInstance ()->getExp (node->getChild (i))).append (
		  ")");
	      args.push_back (insertArg);
	    }
	  pair<string, list<string> > algDef = _generateAlgebraic (it->second,
								   args, node);
	  _algebraics.insert (
	      pair<string, pair<string, list<string> > > (var, algDef));
	}
    }
  int childs = node->getNumChildren ();
  int i;
  for (i = 0; i < childs; i++)
    {
      _replaceFunctions (node->getChild (i));
    }
}

pair<string, list<string> >
MMOMath::_generateAlgebraic (pair<list<string>, ASTNode*> function,
			     list<string> args, ASTNode *node)
{
  ASTNode *repNode = new ASTNode (*function.second);
  _processNode (repNode);
  list<string>::iterator defArgs = function.first.begin ();
  list<string> variables;
  for (list<string>::iterator it = args.begin (); it != args.end (); it++)
    {
      repNode->renameSIdRefs (*defArgs, *it);
      defArgs++;
    }
  string ret = MMOUtils::getInstance ()->getExp (repNode);
  _getVariables (repNode, &variables);
  int childs = node->getNumChildren (), i;
  for (i = 0; i < childs; i++)
    {
      node->removeChild (0);
    }
  if (_type == MATH_ZERO_CROSSING)
    {
      node->setType (repNode->getType ());
      childs = repNode->getNumChildren ();
      for (i = 0; i < childs; i++)
	{
	  node->addChild (new ASTNode (*repNode->getChild (i)));
	}
    }
  else
    {
      node->setType (AST_NAME);
      node->setName (ret.c_str ());
      node->setId ("REPLACED_FUNCTION");
    }
  delete repNode;
  return (pair<string, list<string> > (ret, variables));
}

bool
MMOMath::isConditional ()
{
  return (_isConditional);
}
