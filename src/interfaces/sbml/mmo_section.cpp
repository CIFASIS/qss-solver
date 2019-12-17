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

#include "mmo_section.h"

#include <utility>

#include "mmo_decl.h"
#include "mmo_event.h"
#include "mmo_visitor.h"

MMOSection::MMOSection(MMOSectionType type) :
    _expIt(0), _algebraics(), _setInitialValues(false)
{
  _type = type;
  switch(type)
  {
    case SEC_EQUATION:
      _id = "equation";
      break;
    case SEC_INITIAL_ALGORITHM:
      _id = "initial algorithm";
      break;
    case SEC_EXTERNAL_FUNCTIONS:
      _id = "";
      break;
    case SEC_ALGORITHM:
      _id = "algorithm";
      break;
    case SEC_DECLARATIONS:
      _id = "";
      break;
    case SEC_IMPORTS:
      _id = "";
      break;
  }
}

MMOSection::~MMOSection()
{
}

void
MMOSection::accept(MMOVisitor *visitor)
{
  if(!_setInitialValues)
  {
    visitor->visit(this);
  }
  if(_type == SEC_DECLARATIONS)
  {
    for(std::map<string, MMOExp*>::iterator it = _exps.begin();
        it != _exps.end(); ++it)
    {
      MMODecl *tmp = (MMODecl*) it->second;
      if(tmp->isImport())
      {
        visitor->visit(it->second);
      }
    }
    for(std::map<string, MMOExp*>::iterator it = _exps.begin();
        it != _exps.end(); ++it)
    {
      MMODecl *tmp = (MMODecl*) it->second;
      if(!tmp->isImport())
      {
        visitor->visit(it->second);
      }
    }

  }
  else if(_type == SEC_EQUATION)
  {
    for(list<string>::iterator it = _algebraics.begin();
        it != _algebraics.end(); it++)
    {
      MMODecl *v = findDec(*it);
      if(v->isAlgebraicEquation())
      {
        visitor->visit(v);
      }
    }
    for(std::map<string, MMOExp*>::iterator it = _exps.begin();
        it != _exps.end(); ++it)
    {
      MMODecl *tmp = (MMODecl*) it->second;
      if(tmp->isDerivative())
      {
        visitor->visit(it->second);
      }
    }
  }
  else if(_type == SEC_ALGORITHM && _setInitialValues)
  {
    for(std::map<string, MMOExp*>::iterator it = _exps.begin();
        it != _exps.end(); ++it)
    {
      MMOEvent *tmp = (MMOEvent*) it->second;
      tmp->setInitialAlgorithm(true);
      visitor->visit(it->second);
      tmp->setInitialAlgorithm(false);
    }
  }
  else
  {
    for(std::map<string, MMOExp*>::iterator it = _exps.begin();
        it != _exps.end(); ++it)
    {
      visitor->visit(it->second);
    }
  }
  if(!_setInitialValues)
  {
    visitor->leave(this);
  }
}

void
MMOSection::add(MMOExp *exp)
{
  _exps.insert(pair<string, MMOExp*>(exp->getId(), exp));
}

MMOExp *
MMOSection::find(string id)
{
  map<string, MMOExp*>::iterator it = _exps.find(id);
  if(it != _exps.end())
  {
    return it->second;
  }
  return NULL;
}

MMODecl *
MMOSection::findDec(string id)
{
  if(_type == SEC_DECLARATIONS || _type == SEC_EQUATION)
  {
    return (MMODecl*) find(id);
  }
  return NULL;
}

void
MMOSection::setAlgebraics(list<string> algebraics)
{
  _algebraics = algebraics;
}

MMODecl *
MMOSection::first()
{
  _expIt = 0;
  map<string, MMOExp*>::iterator it = _exps.begin();
  if(it == _exps.end())
  {
    return NULL;
  }
  return (MMODecl*) it->second;
}

MMODecl*
MMOSection::next()
{
  _expIt++;
  map<string, MMOExp*>::iterator it = _exps.begin();
  for(unsigned int i = 0; i < _expIt; i++)
  {
    it++;
  }
  if(it == _exps.end())
  {
    return NULL;
  }
  return (MMODecl*) it->second;
}

bool
MMOSection::end()
{
  return _expIt == _exps.size();
}

void
MMOSection::setInitialValues(bool iv)
{
  _setInitialValues = iv;
}
