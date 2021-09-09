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

#include <iostream>
#include <map>
#include <math.h>

#include <deps/sbg_graph/container_def.h>
#include <util/logger.h>

namespace SB {

using namespace std;

#define Inf numeric_limits<int>::max()


template <template <typename Value, typename Hash = boost::hash<Value>, typename Pred = std::equal_to<Value>,
                    typename Alloc = std::allocator<Value>>
          class CT>
struct IntervalImp {
  
  IntervalImp() = default;
  IntervalImp(bool is_empty)
  {
    _lo = -1;
    _step = -1;
    _hi = -1;
    _empty = is_empty;
  };
  
  IntervalImp(int vlo, int vstep, int vhi)
  {
    if (vlo >= 0 && vstep > 0 && vhi >= 0) {
      _empty = false;
      _lo = vlo;
      _step = vstep;

      if (vlo <= vhi && vhi < Inf) {
        int rem = std::fmod(vhi - vlo, vstep);
        _hi = vhi - rem;
      } else if (vlo <= vhi && vhi == Inf) {
        _hi = Inf;
      } else {
        _empty = true;
      }
    } else if (vlo >= 0 && vstep == 0 && vhi == vlo) {
      _empty = false;
      _lo = vlo;
      _hi = vhi;
      _step = 1;
    } else {
      _lo = -1;
      _step = -1;
      _hi = -1;
      _empty = true;
    }
  }

  int gcd(int a, int b)
  {
    if (b ==0) return b;
    int c;

    do {
      c = a % b;
      if (c > 0) {
        a = b;
        b = c;
      }
    } while (c != 0);

    return b;
  }

  int lcm(int a, int b)
  {
    if (b == 0) return 0;
    if (a < 0 || b < 0) return -1;

    return (a * b) / gcd(a, b);
  }

  int lo() const { return _lo; }

  int step() const { return _step; }

  int hi() const { return _hi; }

  bool empty() const { return _empty; }

  bool isIn(int x)
  {
    if (x < _lo || x > _hi || _empty) {
      return false;
    }

    float aux = fmod(x - _lo, _step);
    if (aux == 0) {
      return true;
    }
    return false;
  }

  IntervalImp cap(IntervalImp &other)
  {
    int max_low = max(_lo, other._lo), new_low = -1;
    int new_step = lcm(_step, other._step);
    int new_end = min(_hi, other._hi);

    if (!_empty && !other.empty()) {
      for (int i = 0; i < new_step; i++) {
        int elem = max_low + i;
        if (isIn(elem) && other.isIn(elem)) {
          new_low = elem;
          break;
        }
      }
    } else {
      return IntervalImp(true);
    }
    if (new_low < 0) {
      return IntervalImp(true);
    }
    return IntervalImp(new_low, new_step, new_end);
  }

  CT<IntervalImp> diff(IntervalImp &other)
  {
    CT<IntervalImp> res;
    IntervalImp cap_res = cap(other);

    if (cap_res._empty) {
      res.insert(*this);
      return res;
    }

    if (cap_res == *this) {
        return res;
    }

    // "Before" intersection
    if (_lo < cap_res.lo()) {
      IntervalImp diff_left = IntervalImp(_lo, 1, cap_res.lo() - 1);
      IntervalImp left = cap(diff_left);
      res.insert(left);
    }

    // "During" intersection
    if (cap_res.step() <= (cap_res.hi() - cap_res.lo())) {
      int n_inters = cap_res.step() / _step;
      for (int i = 1; i < n_inters; i++) {
        IntervalImp diff_middle = IntervalImp(cap_res.lo() + i * _step, cap_res.step(), cap_res.hi());
        res.insert(diff_middle);
      }
    }

    // "After" intersection
    if (_hi > cap_res.hi()) {
      IntervalImp diff_right = IntervalImp(cap_res.hi() + 1, 1, _hi);
      IntervalImp right = cap(diff_right);
      res.insert(right);
    }

    return res;
  }

  int minElem() { return _lo; }

  // Cardinality of interval
  int size()
  {
    int res = (_hi - _lo) / _step + 1;
    return res;
  }

  bool operator==(const IntervalImp &other) const
  {
    return (_lo == other.lo()) && (_step == other.step()) && (_hi == other.hi()) && (_empty == other.empty());
  }

  bool operator!=(const IntervalImp &other) const
  {
    return (_lo != other.lo()) || (_step != other.step()) || (_hi != other.hi()) || (_empty != other.empty());
  }

  size_t hash() { return _lo; }

  protected:
  int _lo;
  int _step;
  int _hi;
  bool _empty;
};

template <template <typename Value, typename Hash = boost::hash<Value>, typename Pred = std::equal_to<Value>,
                    typename Alloc = std::allocator<Value>>
          class CT>
size_t hash_value(IntervalImp<CT> inter)
{
  return inter.hash();
}

typedef IntervalImp<UnordCT> Interval;
typedef UnordCT<Interval> UnordInterval;
typedef OrdCT<Interval> OrdInterval;

ostream &operator<<(ostream &out, Interval &i);

} // namespace SB