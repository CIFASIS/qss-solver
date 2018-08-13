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

#include "../ir/expression.h"
#include "../util/symbol_table.h"

namespace MicroModelica {
  namespace Util {

    class IndexDefinition
    {
      public:
        IndexDefinition();
        ~IndexDefinition();
      private:
        string _variable;
        MicroModelica::IR::Expression _exp;
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
      private:
        map<int,IndexDefinition> _indexes;
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
