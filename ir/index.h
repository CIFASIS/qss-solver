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

    namespace RANGE {
      typedef enum {
        For,
        If 
      } Type;
    }


    class IndexDefinition
    {
      public:
        IndexDefinition();
        ~IndexDefinition() {};

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
        Index(IndexDefinition id);
        ~Index() {};
        string
        print() const;
        void
        setMap(IR::Expression exp);
        bool
        hasMap() const;
        bool
        operator==(const Index &other) const;
        void
        add(IndexDefinition id);
        inline int
        dimension() { return _dim; };
        friend std::ostream& operator<<(std::ostream& out, const Index& i);
      private:
        map<int,IndexDefinition> _indexes;
        int                      _dim;
    };

    typedef ModelTable<std::string, Index> IndexTable;

    class RangeDefinition
    {
      public:
        RangeDefinition() {};
        RangeDefinition(int begin, int end, int step = 1);
        ~RangeDefinition() {};
        inline int 
        begin() { return _begin; };
        inline int 
        end() { return _end; };
        inline int 
        step() { return _step; };
        inline int 
        size() const { return (_end-_begin)/_step; };
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
        Range(AST_Equation_For eqf, Util::VarSymbolTable symbols, RANGE::Type type = RANGE::For);
        Range(AST_Statement_For stf, Util::VarSymbolTable symbols, RANGE::Type type = RANGE::For);
        ~Range() {};
        inline int 
        size() const { return _size; };
        friend std::ostream& operator<<(std::ostream& out, const Range& r);
        inline RangeDefinitionTable 
        definition() const { return _ranges; };
        std::string 
        print() const;
        std::string 
        end() const;
        string 
        indexes() const; 
        void 
        addLocalVariables();
      private:
        void 
        setRangeDefinition(AST_ForIndexList fil, Util::VarSymbolTable symbols);
        RangeDefinitionTable _ranges;
        int                  _size;
        RANGE::Type          _type;
    };
  }
}
#endif /* INDEX_H_ */
