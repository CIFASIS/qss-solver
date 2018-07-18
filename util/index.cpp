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

#include "index.h"


IndexDefinition::IndexDefinition()
{
}
 
IndexDefinition::~IndexDefinition()
{
}

RangeDefinition::RangeDefinition()
{
}

RangeDefinition::~RangeDefinition()
{
}

Range::Range()
{
}

Range::~Range()
{
}
 
int
Range::begin(int dim)
{
  return 0;
}

int
Range::end(int dim)
{
  return 0;
}
 
void
Range::setBegin(int v, int d)
{
}
 
void
Range::setEnd(int v, int d)
{
}
 
int
Range::dimension()
{
  return 0;
}

int
Range::size(int dim)
{
  return 0;
}
 
int
Range::size()
{
  return 0;
}
 
void
Range::clear()
{
}

bool
Range::check()
{
  return false;
}

void
Range::get(AST_Equation_For eqf, VarSymbolTable vt)
{
}
 
void
Range::get(AST_Statement_For stf, VarSymbolTable vt)
{
}
 
bool
Range::empty()
{
  return false;
}
 
void
Range::setVariable(string v, int d)
{
}
 
string
Range::variable(int dim)
{
  return "";
}

void
Range::setLocalVariable(string v, int dim)
{
}
 
string
Range::localVariable(string v)
{
  return "";
}

    
Index::Index()
{
}
 
Index::~Index()
{
}
 
string
Index::print() const 
{
  return "";
}
 
bool
Index::hasRange() const 
{
  return false;
}
 
Range
Index::range() const 
{
  return Range();
}
 
void
Index::setRange(Range r)
{
}
 
void
Index::setMap(MMO_Expression exp)
{
}
 
bool
Index::hasMap() const 
{
  return false;
}
 
bool
Index::operator==(const Index &other) const 
{
  return false;
}
 
void
Index::addIndex(IndexDefinition id)
{
}

int
Index::dimension()
{
  return 0;
}

VariableInterval::VariableInterval()
{
}
 
VariableInterval::~VariableInterval()
{
}
