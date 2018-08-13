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

#ifndef INDEX_H_
#define INDEX_H_

#include <string>
#include <map>

#include "expression.h"
#include "../util/symbol_table.h"
#include "../util/table.h"

namespace MicroModelica {
  namespace IR {

    class IndexDefinition
    {
      public:
        IndexDefinition();
        ~IndexDefinition();

        friend std::ostream& operator<<(std::ostream& out, const IndexDefinition& id);
      private:
        string _variable;
        Expression _exp;
    };

    class Index
    {
      public:
        /**
         *
         */
        Index();
        ~Index();
        string
        print() const;
        void
        setMap(MicroModelica::IR::Expression exp);
        bool
        hasMap() const;
        bool
        operator==(const Index &other) const;
        void
        addIndex(IndexDefinition id);
        int
        dimension();

        friend std::ostream& operator<<(std::ostream& out, const Index& i);
      private:
        map<int,IndexDefinition> _indexes;
    };

    class RangeDefinition
    {
      public:
        RangeDefinition(int begin, int end, int step = 1);
        ~RangeDefinition();
        inline int 
        begin() { return _begin; };
        inline int 
        end() { return _end; };
        inline int 
        step() { return _step; };
        friend std::ostream& operator<<(std::ostream& out, const RangeDefinition& rd);
      private:
        int   _begin;
        int   _end;
        int   _step;
        
    };

    typedef ModelTable<std::string, RangeDefinition> RangeDefinitionTable;
    class Range 
    {
      public:
        Range();
        Range(AST_Equation_For eqf, Util::VarSymbolTable symbols);
        Range(AST_Statement_For stf, Util::VarSymbolTable symbols);
        ~Range();
        friend std::ostream& operator<<(std::ostream& out, const Range& r);
      private:
        void 
        setRangeDefinition(AST_ForIndexList fil, Util::VarSymbolTable symbols);
        RangeDefinitionTable _ranges;
    };


    class VariableInterval 
    {
      public:
        VariableInterval();
        ~VariableInterval();
    };
  }
}
#endif /* INDEX_H_ */
