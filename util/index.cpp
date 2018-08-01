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

using namespace MicroModelica::IR;

namespace MicroModelica {
  namespace Util {


    IndexDefinition::IndexDefinition()
    {
    }
     
    IndexDefinition::~IndexDefinition()
    {
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
     
    void
    Index::setMap(Expression exp)
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
  }
}
