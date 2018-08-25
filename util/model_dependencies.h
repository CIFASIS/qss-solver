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

#ifndef MODEL_DEPENDENCIES_H_
#define MODEL_DEPENDENCIES_H_

#include "dependency_matrix.h"

namespace MicroModelica {
  namespace Util {

    class ModelDependencies 
    {
      public:
        ModelDependencies();
        ~ModelDependencies();
        inline DependencyMatrix 
        SD() { return _SD; };
        inline DependencyMatrix 
        DS() { return _DS; };
        inline DependencyMatrix 
        SZ() { return _SZ; };
        inline DependencyMatrix 
        ZS() { return _ZS; };
        inline DependencyMatrix 
        SO() { return _SO; };
        inline DependencyMatrix 
        OS() { return _OS; };
        inline DependencyMatrix 
        DO() { return _DO; };
        inline DependencyMatrix 
        OD() { return _OD; };
      private:
        DependencyMatrix _SD;
        DependencyMatrix _DS;
        DependencyMatrix _ZS;
        DependencyMatrix _SZ;
        DependencyMatrix _SO;
        DependencyMatrix _OS;
        DependencyMatrix _DO;
        DependencyMatrix _OD;
    };
  }
}

#endif /* MODEL_DEPENDENCIES_H_ */
