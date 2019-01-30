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

#ifndef DEPS_INDEX_PAIR_H_
#define DEPS_INDEX_PAIR_H_

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

  class IndexPair {
  public:
    inline IndexPair() { };
    inline IndexPair(MDI dom_, MDI ran_, Offset os, Usage us): dom(dom_), ran(ran_), offset(os), usage(us) { };
    inline MDI Dom() const { return dom; }
    inline MDI Ran() const { return ran; }
    inline Offset GetOffset() const { return offset; }
    inline Usage GetUsage() const { return usage; }
    std::list<IndexPair> operator-(const IndexPair& other) const;
    std::list<IndexPair> Difference(const IndexPair& other) { return (*this) - other; }
    std::set<IndexPair> RemoveUnknowns(MDI unk2remove);
    std::set<IndexPair> RemoveEquations(MDI eqs2remove);
    bool operator<(const IndexPair& other) const;
    Option<IndexPair> operator&(const IndexPair& other) const;
    friend std::ostream& operator<<(std::ostream& os, const IndexPair& ip);
    bool Contains(const IndexPair& other) const;
    IndexPairType Type() const;
  private:
    MDI dom, ran;
    Offset offset;
    Usage usage;
  };

  std::ostream& operator<<(std::ostream &os, const std::list<IndexPair> &ipList);


  typedef std::set<IndexPair> IndexPairSet;
  std::ostream& operator<<(std::ostream& os, const IndexPairSet& ips);
}
}
#endif /* DEPS_INDEX_PAIR_H_ */