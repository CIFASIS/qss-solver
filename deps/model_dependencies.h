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
#include "../ir/event.h"
#include "../util/symbol_table.h"
#include "dependency_matrix.h"
#include "dependency.h"

namespace MicroModelica {
  namespace Deps {

    class ModelDependencies 
    {
      public:
        ModelDependencies();
        ~ModelDependencies() = default;
        inline VariableDependencyMatrix 
        SD() { _SD.setMode(VDM::Normal); return _SD; };
        inline VariableDependencyMatrix 
        DS() { _SD.setMode(VDM::Transpose); return _SD; };
        inline VariableDependencyMatrix 
        SZ() { _SZ.setMode(VDM::Normal); return _SZ; };
        inline VariableDependencyMatrix 
        ZS() { _SZ.setMode(VDM::Transpose); return _SZ; };
        inline VariableDependencyMatrix 
        SO() { _SO.setMode(VDM::Normal); return _SO; };
        inline VariableDependencyMatrix 
        OS() { _SO.setMode(VDM::Transpose); return _SO; };
        inline VariableDependencyMatrix 
        DO() { _DO.setMode(VDM::Normal); return _DO; };
        inline VariableDependencyMatrix 
        OD() { _DO.setMode(VDM::Transpose); return _DO; };
        inline VariableDependencyMatrix
        HD() { _HD.setMode(VDM::Normal); return _HD; };
        inline VariableDependencyMatrix 
        DH() { _HD.setMode(VDM::Transpose); return _HD; };
        inline VariableDependencyMatrix
        HZ() { _HZ.setMode(VDM::Normal); return _HZ; };
        inline VariableDependencyMatrix 
        ZH() { _HZ.setMode(VDM::Transpose); return _HZ; };
        inline VariableDependencyMatrix 
        HH() { _HH.setMode(VDM::Normal); return _HH; };
        inline VariableDependencyMatrix 
        LHSDsc() { _LHSDsc.setMode(VDM::Normal); return _LHSDsc; };
        inline VariableDependencyMatrix 
        LHSSt() { _LHSSt.setMode(VDM::Normal); return _LHSSt; };
        inline VariableDependencyMatrix 
        RHSSt() { _RHSSt.setMode(VDM::Normal); return _RHSSt; };
        inline EquationDependencyMatrix 
        OA() { return _OA; };
        inline EquationDependencyMatrix 
        ZCA() { return _ZCA; };
        inline EquationDependencyMatrix 
        DA() { return _DA; };
        void
        compute(IR::EquationTable eqs, IR::EquationTable outputs, IR::EquationTable algs,
                IR::EventTable events, Util::VarSymbolTable symbols);

      private:
        VariableDependencyMatrix _SD;
        VariableDependencyMatrix _SZ;
        VariableDependencyMatrix _SO;
        VariableDependencyMatrix _DO;
        VariableDependencyMatrix _HD;
        VariableDependencyMatrix _HZ;
        VariableDependencyMatrix _LHSDsc;
        VariableDependencyMatrix _LHSSt;
        VariableDependencyMatrix _RHSSt;
        VariableDependencyMatrix _HH;
        
        EquationDependencyMatrix _OA;
        EquationDependencyMatrix _ZCA;
        EquationDependencyMatrix _DA;
        Dependency               _deps;
    };
  }
}

#endif /* MODEL_DEPENDENCIES_H_ */
