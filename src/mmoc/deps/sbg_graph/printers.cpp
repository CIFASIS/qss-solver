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

#include <deps/sbg_graph/atomic_set.h>
#include <deps/sbg_graph/interval.h>
#include <deps/sbg_graph/multi_interval.h>
#include <deps/sbg_graph/linear_map.h>
#include <deps/sbg_graph/pw_atom_map.h>
#include <deps/sbg_graph/pw_lmap.h>
#include <deps/sbg_graph/set.h>

namespace SB {

ostream &operator<<(ostream &out, AtomSet &as)
{
  MultiInterval mi = as.atomicSets();

  out << "{" << mi << "}";

  return out;
}

ostream &operator<<(ostream &out, Set &s)
{
  UnordCT<AtomSet> as = s.atomicSets();
  UnordCT<AtomSet>::iterator it = as.begin();
  AtomSet aux;

  if (as.size() == 0) {
    out << "{}";
    return out;
  }

  if (as.size() == 1) {
    aux = *it;
    out << "{" << aux << "}";
    return out;
  }

  aux = *it;
  out << "{" << aux;
  ++it;
  while (next(it, 1) != as.end()) {
    aux = *it;
    out << ", " << aux;
    ++it;
  }
  aux = *it;
  out << ", " << aux << "}";

  return out;
}

std::string mapOper(Real cte) { return (cte >= 0) ? "+ " : ""; }

ostream &operator<<(ostream &out, LMap &lm)
{
  OrdRealCT slopes = lm.slopes();
  OrdRealCT::iterator slopes_it = slopes.begin();
  OrdRealCT constants = lm.constants();
  OrdRealCT::iterator constants_it = constants.begin();

  if (slopes.size() == 0) {
    return out;
  }

  if (slopes.size() == 1) {
    out << "[" << *slopes_it << " * x " << mapOper(*constants_it) << *constants_it << "]";
    return out;
  }

  out << "[" << *slopes_it << " * x " << mapOper(*constants_it) << *constants_it;
  ++slopes_it;
  ++constants_it;
  while (next(slopes_it, 1) != slopes.end()) {
    out << ", " << *slopes_it << " * x " << mapOper(*constants_it) << *constants_it;

    ++slopes_it;
    ++constants_it;
  }
  out << ", " << *slopes_it << " * x " << mapOper(*constants_it) << *constants_it << "]";

  return out;
}

ostream &operator<<(ostream &out, Interval &i)
{
  out << "[" << i.lo() << ":" << i.step() << ":" << i.hi() << "]";
  return out;
}

ostream &operator<<(ostream &out, MultiInterval &mi)
{
  OrdCT<Interval> is = mi.intervals();
  OrdCT<Interval>::iterator it = is.begin();

  if (is.size() == 0) return out;

  if (is.size() == 1) {
    out << *it;
    return out;
  }

  out << *it;
  ++it;
  while (next(it, 1) != is.end()) {
    out << "x" << *it;
    ++it;
  }
  out << "x" << *it;

  return out;
}

ostream &operator<<(ostream &out, PWAtomLMap &pw_atom)
{
  AtomSet dom = pw_atom.dom();
  LMap lm = pw_atom.linearMap();

  out << "(" << dom << ", " << lm << ")";
  return out;
}

ostream &operator<<(ostream &out, PWLMap &pw)
{
  OrdCT<Set> dom = pw.dom();
  OrdCT<Set>::iterator dom_it = dom.begin();
  OrdCT<LMap> lmap = pw.linearMap();
  OrdCT<LMap>::iterator lmap_it = lmap.begin();

  if (dom.size() == 0) {
    out << "[]";
    return out;
  }

  if (dom.size() == 1) {
    out << "[(" << *dom_it << ", " << *lmap_it << ")]";
    return out;
  }

  out << "[(" << *dom_it << ", " << *lmap_it << ") ";
  ++dom_it;
  ++lmap_it;
  while (next(dom_it, 1) != dom.end()) {
    out << ", (" << *dom_it << ", " << *lmap_it << ")";

    ++dom_it;
    ++lmap_it;
  }
  out << ", (" << *dom_it << ", " << *lmap_it << ")]";

  return out;
}
} // namespace SB