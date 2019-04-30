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

#include "graph/graph_helpers.h"
#include "../ir/index.h"
#include "../util/table.h"

namespace MicroModelica {
  namespace Deps {

    namespace VDM {
        typedef enum {
          Normal,
          Transpose    
        } Mode;
        
        typedef enum {
          Alloc = 0,
          Init = 2   
        } Method;
    }

    typedef int depId;

    typedef std::string varId;

    class VariableDependency 
    {
      public:
        VariableDependency() {};
        ~VariableDependency() {};
        inline void 
        setVariable(std::string var) { _variable = var; };
        inline void 
        setDom(MDI dom) { _dom = dom; };
        inline void 
        setRan(MDI ran) { _ran = ran; };
        inline void 
        setId(int id) { _id = id; };
        inline void 
        setEquationId(int eqid) { _equationId = eqid; };
        inline const IR::Index
        ifr() { return IR::Index(_ifr); };
        inline const IR::Index
        ife() { return IR::Index(_ife); };
        IR::Range
        range() { return _range; };
        inline void
        setIfr(IR::Expression exp) { _ifr = exp; };
        inline void
        setIfe(IR::Expression exp) { _ife = exp; };
        void 
        setRange(Usage ifrUsg, Usage ifeUsg);
      private:
        MDI            _dom;
        MDI            _ran;
        IR::Expression _ifr;
        IR::Expression _ife;
        IR::Range      _range;
        int            _id;
        int            _equationId;
        std::string    _variable;
    };

    typedef list<VariableDependency> VariableInfluences;
    
    typedef struct {
        VariableInfluences algs;
        VariableDependency ifce;   
        inline const IR::Index 
        ifr() { return ifce.ifr(); };
        inline const IR::Index 
        ife() { return ifce.ife(); };     
    } Influences;

    std::ostream& operator<<(std::ostream& out, const Influences& d);

    typedef list<Influences> VariableDependencies;

    struct MatrixConfig {
        std::string container;
        std::string names[4];
        std::string access[2];
    };

    class VariableDependencyMatrix : public ModelTable<varId,VariableDependencies>
    {
      public:
        VariableDependencyMatrix(MatrixConfig cfg);
        ~VariableDependencyMatrix() {};
        inline const VariableDependencyMatrix& 
        alloc() { _method = VDM::Alloc; return *this; };
        inline const VariableDependencyMatrix&
        init() { _method = VDM::Init; return *this; }; 
        inline void 
        setMode(VDM::Mode mode) { _mode = mode; };
        std::string 
        print() const;    
        friend std::ostream& operator<<(std::ostream& out, const VariableDependencyMatrix& d);
      private:
        MatrixConfig _cfg;
        VDM::Mode    _mode;
        VDM::Method  _method;  
    };

    typedef std::list<depId> EquationDependency;

    typedef ModelTable<depId,EquationDependency> EquationDependencyMatrix;
  }
}

#endif /* DEPENDENCY_MATRIX_H_ */
