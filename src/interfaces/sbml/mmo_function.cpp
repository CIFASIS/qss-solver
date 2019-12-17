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

#include "mmo_function.h"

#include "mmo_visitor.h"

MMOFunction::MMOFunction(string id) :
    _id(id), _definition(NULL), _inputs(), _output(NULL)
{
}

MMOFunction::~MMOFunction()
{
}

void
MMOFunction::accept(MMOVisitor *visitor)
{
  visitor->visit(this);
  for(list<MMODecl*>::iterator it = _inputs.begin(); it != _inputs.end(); it++)
  {
    visitor->visit(*it);
  }
  visitor->visit(_output);
  visitor->visit(_definition);
  visitor->leave(this);
}

void
MMOFunction::add(MMODecl *decl)
{
  if(decl->isFunctionInput())
  {
    _inputs.push_back(decl);
  }
  else if(decl->isFunctionDefinition())
  {
    _definition = decl;
    _output = new MMODecl(decl->getId(), DEC_FUNCTION_OUTPUT);
  }
}
