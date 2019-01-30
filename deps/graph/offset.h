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

#ifndef DEPS_OFFSET_H_
#define DEPS_OFFSET_H_

/*#include <utility>
#include <set>
#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/icl/discrete_interval.hpp>

#include <ast/equation.h>
#include <causalize/graph/graph_definition.h>

#include <ast/ast_types.h>
#include <boost/algorithm/string/join.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <sstream>
#include <string>*/

namespace MicroModelica {
  namespace Deps {
    class Offset {
      public:
        inline Offset(std::vector<int> offset): offset(offset) { };
        inline Offset(): offset() { };
        inline bool operator<(const Offset& other) const { return this->offset < other.offset; };
        inline bool operator==(const Offset& other) const { return this->offset == other.offset; };
        inline bool operator!=(const Offset& other) const { return this->offset != other.offset; };
        inline int operator[](const int& index) const { return offset[index]; };
        inline bool isZeros() { 
          for(int i: offset) {
            if (i!=0) return false;
          }
          return true;
        }
        Offset operator-() const;
        typedef std::vector<int>::iterator iterator;
        typedef std::vector<int>::const_iterator const_iterator;
        inline const_iterator begin() const { return offset.begin(); }
        inline iterator begin() { return offset.begin(); }
        inline iterator end() { return offset.end(); }
        inline unsigned int Size () const { return offset.size(); }
      private:
          std::vector<int> offset;
    };
  }
}
#endif /* DEPS_OFFSET_H_ */