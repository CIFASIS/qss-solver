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

#ifndef DEPS_USAGE_H_
#define DEPS_USAGE_H_

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

  /// @brief A pair representing a usage of a variable in an equation
  typedef ICL::discrete_interval<int> Interval;
  inline Interval CreateInterval(int a, int b) {
    return ICL::discrete_interval<int>(a,b, ICL::interval_bounds::closed());
  }
  typedef std::list<Interval> IntervalList;
  typedef std::vector<Interval> IntervalVector;

  class Usage {
  public:
    inline Usage():usage() { };
    inline Usage(int size):usage(std::vector<int>(size)) { }
    inline Usage(int size, int value):usage(std::vector<int>(size, value)) { }
    inline int& operator[](int index) { return usage[index]; }
    inline const int& operator[](int index) const { return usage[index]; }
    inline void push_back(const int i) { usage.push_back(i); }
    inline bool operator==(const Usage& other) const { return this->usage == other.usage; };
    inline bool operator!=(const Usage& other) const { return this->usage != other.usage; };
    inline int Size() { return usage.size(); }
    inline bool isUnused() {
      for(int i: usage) {
        if (i!=-1) return false;
      }
      return true;
    }
    typedef std::vector<int>::iterator iterator;
    typedef std::vector<int>::const_iterator const_iterator;
    inline const_iterator begin() const { return usage.begin(); }
    inline iterator begin() { return usage.begin(); }
    inline iterator end() { return usage.end(); }
  private:
    std::vector<int> usage;
  };
}
}

#endif /* DEPS_USAGE_H_ */
