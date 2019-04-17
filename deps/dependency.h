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

#ifndef DEP_DEPENDENCY_H_
#define DEP_DEPENDENCY_H_

#include <string>

#include "dependency_matrix.h"
#include "graph/graph_helpers.h"
#include "graph/graph.h"

namespace MicroModelica {
  namespace Deps {
    class Dependency 
    {
      public:
        Dependency() : _ifr() {};
        ~Dependency() {};
        void
        compute(DepsGraph g, VariableDependencyMatrix& vdm);
      private:
        void
        influencees(DepsGraph g, Vertex vd, MDI infs, VariableDependencies &deps, VariableInfluences& algs);
        VariableDependency 
        getVariableDependency(string name, MDI dom, MDI ran, int id);
        MDI 
        variableMDI(Util::Variable var);  
      private: 
        IR::Expression _ifr;
        Usage          _ifrUsg;
        Usage          _ifeUsg;
    };
  }
}

#endif // DEP_DEPENDENCY_H
