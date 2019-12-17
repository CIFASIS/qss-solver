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

#include "biorica_writer.h"

#include <iostream>
#include <iterator>
#include <sstream>

#include "mmo_decl.h"
#include "mmo_utils.h"

BioRicaWriter::BioRicaWriter(string file) :
    _indent(1), _modelName(), _out(file.c_str(),
        ios::out | ios::in | ios::trunc),
        _consts(), _formulas(), _domains(), _nodeElementStates(), _nodeElementFlows(),
        _nodeElementEvents(), _nodeFieldEqDiffs(), _nodeFieldTransitions(), _nodeFieldAssertions(),
        _nodeFieldExternals(), _nodeFieldSynchs(), _nodeFieldInits(), _currentEvent()
{
  if(!_out.good())
  {
    cout << "Can not open file: " << file << endl;
  }
  _modelName = file;
  unsigned int found = _modelName.rfind("/");
  if(found != std::string::npos)
  {
    _modelName.erase(_modelName.begin(), _modelName.begin() + found + 1);
  }
  found = _modelName.rfind(".br");
  if(found != std::string::npos)
  {
    _modelName.erase(_modelName.begin() + found, _modelName.end());
  }
}

BioRicaWriter::~BioRicaWriter()
{
  _print(&_consts);
  _print(&_formulas);
  _out << "node " << _modelName << endl;
  if(!_nodeElementStates.empty())
  {
    _out << "state" << endl;
    _print(&_nodeElementStates, " ");
  }
  if(!_nodeElementEvents.empty())
  {
    _out << "event" << endl;
    _print(&_nodeElementEvents);
  }
  if(!_nodeFieldEqDiffs.empty())
  {
    _out << "eqdiff" << endl;
    _print(&_nodeFieldEqDiffs);
  }
  if(!_nodeFieldTransitions.empty())
  {
    _out << "trans" << endl;
    map<string, pair<string, list<string>*> >::iterator it;
    for(it = _nodeFieldTransitions.begin(); it != _nodeFieldTransitions.end();
        it++)
    {
      _out << it->second.first << " |- " << it->first << " -> ";
      _print(it->second.second, ",", ";");
      _out << endl;
    }
  }
  if(!_nodeFieldInits.empty())
  {
    _out << "init" << endl;
    list<string> fields = _getList(_nodeFieldInits);
    _print(&fields, ",\n", ";\n");
  }
  _out << "edon" << endl;
  _out.close();
}

void
BioRicaWriter::_print(list<string> *l, string sep, string finalSep)
{
  int s = l->size(), count = 0;
  for(list<string>::iterator it = l->begin(); it != l->end(); it++)
  {
    _out << *it;
    if(++count == s && !finalSep.empty())
    {
      _out << finalSep;
    }
    else
    {
      _out << sep;
    }
  }
}

void
BioRicaWriter::visit(MMOExp *x)
{
  x->accept(this);
}

void
BioRicaWriter::leave(MMODecl *x)
{
}

void
BioRicaWriter::visit(MMODecl *x)
{
  static MMOUtils *utils = MMOUtils::getInstance();
  if(x->isDerivative())
  {
    _nodeFieldEqDiffs.push_back(
        utils->indent(_indent) + "d" + _flatId(x) + " = " + x->getExp() + ";");
  }
  else if(x->isAlgebraicEquation())
  {
    _formulas.push_back(
        utils->indent(_indent) + "formula " + _flatId(x) + " = " + x->getExp()
            + ";");
  }
  else if(x->isInitialAssignment())
  {
    _nodeFieldInits[_flatId(x)] = utils->indent(_indent) + _flatId(x) + " := "
        + x->getExp();
  }
  else if(x->isAssignment() || x->isReinit())
  {
    pair<string, list<string>*> ev = _nodeFieldTransitions[_currentEvent];
    ev.second->push_back(
        utils->indent(_indent) + _flatId(x) + " := " + x->getExp());
  }
  else if(x->isZeroCrossing())
  {
    pair<string, list<string>*> ev = _nodeFieldTransitions[_currentEvent];
    ev.first = x->getExp();
    _nodeFieldTransitions[_currentEvent] = ev;
  }
  else if(x->isOppositeZeroCrossing())
  {
    _currentEvent = utils->getVar("event");
    _nodeElementEvents.push_back(utils->indent(_indent) + _currentEvent);
    pair<string, list<string>*> ev = _nodeFieldTransitions[_currentEvent];
    ev.first = x->getExp();
  }
  else if(x->isCondition())
  {
    pair<string, list<string>*> ev = _nodeFieldTransitions[_currentEvent];
    ev.first.append(" and ").append(x->getExp());
    _nodeFieldTransitions[_currentEvent] = ev;
  }
  else if(x->isState())
  {
    _nodeElementStates.push_back(
        utils->indent(_indent) + _flatId(x) + ":FLOAT;");
    if(x->hasValue())
    {
      stringstream buff;
      buff << utils->indent(_indent) << _flatId(x) << " := " << x->getValue();
      _nodeFieldInits[_flatId(x)] = buff.str();
    }
  }
  else if(x->isParameter())
  {
    if(x->hasValue())
    {
      stringstream buff;
      buff << utils->indent(_indent) << "const " << _flatId(x) << " = "
          << x->getValue() << ";";
      _consts.push_back(buff.str());
    }
    else
    {
      _nodeElementStates.push_back(
          utils->indent(_indent) + _flatId(x) + ":FLOAT;");
    }
  }
  else if(x->isConstant())
  {
    if(x->hasValue())
    {
      stringstream buff;
      buff << utils->indent(_indent) << "const " << _flatId(x) << " = "
          << x->getValue() << ";";
      _consts.push_back(buff.str());
    }
  }
  else if(x->isDiscrete())
  {
    _nodeElementStates.push_back(
        utils->indent(_indent) + _flatId(x) + ":FLOAT;");
    if(x->hasValue())
    {
      stringstream buff;
      buff << utils->indent(_indent) << _flatId(x) << " := " << x->getValue();
      _nodeFieldInits[_flatId(x)] = buff.str();
    }
  }
}

string
BioRicaWriter::_flatId(MMODecl *x)
{
  return _modelName + "_" + x->getId();
}

void
BioRicaWriter::visit(MMODecl x)
{
  visit(&x);
}

void
BioRicaWriter::visit(MMOEvent *x)
{
  static MMOUtils *utils = MMOUtils::getInstance();
  _currentEvent = utils->getVar("event");
  pair<string, list<string>*> def("", new list<string>());
  _nodeFieldTransitions[_currentEvent] = def;
  _nodeElementEvents.push_back(utils->indent(_indent) + _currentEvent);
}

void
BioRicaWriter::visit(MMOEvent x)
{
  visit(&x);
}

void
BioRicaWriter::leave(MMOEvent *x)
{
  _currentEvent = "";
}

void
BioRicaWriter::visit(MMOSection *x)
{
  if(!x->isEmpty())
  {
    _indent++;
  }
}

void
BioRicaWriter::leave(MMOSection *x)
{
  if(!x->isEmpty())
  {
    _indent--;
  }
}

void
BioRicaWriter::visit(MMOSection x)
{
  visit(&x);
}

void
BioRicaWriter::visit(MMOFunction *x)
{

}

void
BioRicaWriter::visit(MMOFunction x)
{
  visit(&x);
}

void
BioRicaWriter::leave(MMOFunction *x)
{

}

list<string>
BioRicaWriter::_getList(map<string, string> m)
{
  map<string, string>::iterator it;
  list<string> ret;
  for(it = m.begin(); it != m.end(); it++)
  {
    ret.push_back(it->second);
  }
  return ret;
}
