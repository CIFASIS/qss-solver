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

#include "../ir/equation.h"
#include "../util/symbol_table.h"
#include "dependency_matrix.h"
#include "dependency.h"

namespace MicroModelica {
  namespace Deps {

    class ModelDependencies 
    {
      public:
        ModelDependencies() {};
        ModelDependencies(Util::VarSymbolTable &symbols);
        ~ModelDependencies();
        inline VariableDependencyMatrix 
        SD() { return _SD; };
        inline VariableDependencyMatrix 
        DS() { return _DS; };
        inline VariableDependencyMatrix 
        SZ() { return _SZ; };
        inline VariableDependencyMatrix 
        ZS() { return _ZS; };
        inline VariableDependencyMatrix 
        SO() { return _SO; };
        inline VariableDependencyMatrix 
        OS() { return _OS; };
        inline VariableDependencyMatrix 
        DO() { return _DO; };
        inline VariableDependencyMatrix 
        OD() { return _OD; };
        inline EquationDependencyMatrix 
        OA() { return _OA; };
        inline EquationDependencyMatrix 
        ZCA() { return _ZCA; };
        inline EquationDependencyMatrix 
        DA() { return _DA; };
        void
        compute(IR::EquationTable eqs);
      private:
        VariableDependencyMatrix _SD;
        VariableDependencyMatrix _DS;
        VariableDependencyMatrix _ZS;
        VariableDependencyMatrix _SZ;
        VariableDependencyMatrix _SO;
        VariableDependencyMatrix _OS;
        VariableDependencyMatrix _DO;
        VariableDependencyMatrix _OD;
        EquationDependencyMatrix _OA;
        EquationDependencyMatrix _ZCA;
        EquationDependencyMatrix _DA;
        Dependency               _deps;
        Util::VarSymbolTable     _symbols;
    };
  }
}

#endif /* MODEL_DEPENDENCIES_H_ */
