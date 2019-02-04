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

#ifndef DEPENDENCY_MATRIX_H_
#define DEPENDENCY_MATRIX_H_

#include <map>

#include "../ir/index.h"
#include "../util/table.h"

namespace MicroModelica {
  namespace Util {

    typedef int depId;

    class VariableDependency 
    {
      public:
        VariableDependency() {};
        ~VariableDependency() {};
        inline const VariableDependency& 
        alloc() const { return *this; };
        inline const VariableDependency&
        init() const { return *this; }; 
        friend std::ostream& operator<<(std::ostream& out, const VariableDependency& d);
      private:
        IR::Index     _lhs;
        IR::Index     _rhs;
        IR::Range     _range;
        int           _id;
        std::string   _matrixName;
        std::string   _accessVectorName;
        std::string   _transposeAccessVectorName;
    };

    class VariableDependencyMatrix : public ModelTable<depId,VariableDependency>
    {
      public:
        VariableDependencyMatrix() {};
        ~VariableDependencyMatrix() {};
    };

    typedef std::list<depId> EquationDependency;

    typedef ModelTable<depId,EquationDependency> EquationDependencyMatrix;
  }
}

#endif /* DEPENDENCY_MATRIX_H_ */
