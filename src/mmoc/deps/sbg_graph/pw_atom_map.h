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

#pragma once

#include <deps/sbg_graph/atomic_set.h>
#include <deps/sbg_graph/container_def.h>
#include <deps/sbg_graph/linear_map.h>

namespace SB {

template <template <typename T, typename = allocator<T>> class CT, typename LMap, typename AtomSet, typename MultiInterval,
          typename Interval, typename Integer, typename Real>
struct PWAtomLMapImp {

  PWAtomLMapImp() {}
  PWAtomLMapImp(AtomSet dom, LMap lmap)
  {
    AtomSet empty_set;
    LMap empty_map;

    if (dom.dims() != lmap.dims()) {
      _dom = empty_set;
      _lmap = empty_map;
    } else {
      CT<Interval> intervals = dom.atomicSetsRef().intervals();
      CT<Real> slopes = lmap.slopes();
      typename CT<Real>::iterator slope_it = slopes.begin();
      CT<Real> constants = lmap.constants();
      typename CT<Real>::iterator constants_it = constants.begin();
      bool incompatible = false;

      for (Interval interval : intervals) {
        Real test_low = interval.lo() * (*slope_it) + (*constants_it);
        Real test_step = interval.step() * (*slope_it);
        Real test_hi = interval.hi() * (*slope_it) + (*constants_it);

        if (*slope_it < Inf) {
          if (test_low != (int)test_low && interval.lo()) {
            incompatible = true;
          }
          if (test_step != (int)test_step && interval.step()) {
            incompatible = true;
          }
          if (test_hi != (int)test_hi && interval.hi()) {
            incompatible = true;
          }
          ++slope_it;
          ++constants_it;
        }
      }

      if (incompatible) {
        _dom = empty_set;
        _lmap = empty_map;
      } else {
        _dom = dom;
        _lmap = lmap;
      }
    }
  }

  AtomSet dom() { return _dom; }

  LMap linearMap() { return _lmap; }

  bool empty() { return _dom.empty() && _lmap.empty(); }

  Integer checkInf(Real candidate)
  {
    Integer value;
    if (candidate >= Inf) {
      value = Inf;
    } else {
      value = (Integer)candidate;
    }
    return value;
  }
  
  AtomSet image(AtomSet &dom)
  {
    CT<Interval> intervals = (dom.cap(_dom)).atomicSets().intervals();
    CT<Real> slopes = _lmap.slopes();
    typename CT<Real>::iterator slopes_it = slopes.begin();
    CT<Real> constants = _lmap.constants();
    typename CT<Real>::iterator constants_it = constants.begin();

    CT<Interval> res;
    typename CT<Interval>::iterator res_it = res.begin();

    if (_dom.empty()) {
      AtomSet empty;
      return empty;
    }

    for (Interval cap_interval : intervals) {
      Integer new_lo;
      Integer new_step;
      Integer new_hi;

      Real possible_lo = cap_interval.lo() * (*slopes_it) + (*constants_it);
      Real possible_step = cap_interval.step() * (*slopes_it);
      Real possible_hi = cap_interval.hi() * (*slopes_it) + (*constants_it);

      if (*slopes_it < Inf) {
        new_lo = checkInf(possible_lo);
        new_step = checkInf(possible_step);
        new_hi = checkInf(possible_hi);
      } else {
        new_lo = 1;
        new_step = 1;
        new_hi = Inf;
      }

      Interval new_interval(new_lo, new_step, new_hi);
      res_it = res.insert(res_it, new_interval);
      ++res_it;

      ++slopes_it;
      ++constants_it;
    }

    MultiInterval new_mi(res);
    return AtomSet(new_mi);
  }

  AtomSet preImage(AtomSet &other)
  {
    AtomSet full_image = image(_dom);
    AtomSet actual_image = full_image.cap(other);
    PWAtomLMapImp inv(actual_image, _lmap.invlinearMap());

    AtomSet inv_image = inv.image(actual_image);
    return _dom.cap(inv_image);
  }

  bool operator==(const PWAtomLMapImp &other) const { return _dom == other.dom() && _lmap == other.linearMap(); }

  protected:
  AtomSet _dom;
  LMap _lmap;
};

typedef PWAtomLMapImp<OrdCT, LMap, AtomSet, MultiInterval, Interval, Integer, Real> PWAtomLMap;

ostream &operator<<(ostream &out, PWAtomLMap &pw_atom);
} // namespace SB