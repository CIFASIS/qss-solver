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

#include <ir/stored_definition.h>
#include <ir/class.h>

MMO_StoredDefinition_::MMO_StoredDefinition_() :
    _classList()
{
}

MMO_StoredDefinition_::~MMO_StoredDefinition_()
{
}

list<MMO_Class>
MMO_StoredDefinition_::classes() const
{
  return _classList;
}

void
MMO_StoredDefinition_::addClass(MMO_Class c)
{
  _classList.push_back(c);
}

MMO_Class
MMO_StoredDefinition_::begin()
{
  _it = _classList.begin();
  if(_it == _classList.end())
  {
    return NULL;
  }
  return *_it;
}

MMO_Class
MMO_StoredDefinition_::next()
{
  _it++;
  if(_it == _classList.end())
  {
    return NULL;
  }
  return *_it;
}

bool
MMO_StoredDefinition_::end()
{
  return _it != _classList.end();
}

string
MMO_StoredDefinition_::print()
{
  string ret;
  return ret;
}

MMO_StoredDefinition
newMMO_StoredDefinition()
{
  return new MMO_StoredDefinition_();
}

void
deleteMMO_StoredDefinition(MMO_StoredDefinition m)
{
  delete m;
}
