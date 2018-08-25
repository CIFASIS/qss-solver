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

    typedef int mapId;

    class Dependency 
    {
      public:
        Dependency();
        ~Dependency();
        friend std::ostream& operator<<(std::ostream& out, const Dependency& d);
      private:
        IR::Index     _lhs;
        IR::Index     _rhs;
        IR::Range     _range;
        int           _id;
        std::string   _matrixName;
        std::string   _vectorName;
        std::string   _tVectorName;
    };

    class DependencyMatrix : public ModelTable<mapId,Dependency>
    {
      public:
        DependencyMatrix();
        ~DependencyMatrix();
    };
  }
}

#endif /* DEPENDENCY_MATRIX_H_ */
