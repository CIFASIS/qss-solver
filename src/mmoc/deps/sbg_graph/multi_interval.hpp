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

#include <deps/sbg_graph/container_def.h>
#include <deps/sbg_graph/interval.h>

namespace SB {

template <template <typename T, typename = allocator<T>> class CT1,
          template <typename Value, typename Hash = boost::hash<Value>, typename Pred = std::equal_to<Value>,
                    typename Alloc = std::allocator<Value>>
          class CT2,
          typename Interval, typename ElemType>

struct MultiIntervalImp {
  MultiIntervalImp()
  {
    CT1<Interval> empty_res;
    _intervals = empty_res;
    _dims = 0;
  }

  MultiIntervalImp(CT1<Interval> intervals)
  {
    IntervalIt it = intervals.begin();
    bool are_emptys = false;

    while (it != intervals.end()) {
      if ((*it).empty()) {
        are_emptys = true;
      }
      ++it;
    }

    if (are_emptys) {
      CT1<Interval> empty_mi;
      _intervals = empty_mi;
      _dims = 0;
    } else {
      _intervals = intervals;
      _dims = intervals.size();
    }
  }

  CT1<Interval> intervals() { return _intervals; }

  CT1<Interval> &intervals_ref() { return _intervals; }

  int dims() { return _dims; }

  void addInter(Interval interval)
  {
    if (!interval.empty()) {
      _intervals.insert(_intervals.end(), interval);
      ++_dims;
    }
  }

  bool empty() { return _intervals.empty(); }

  bool isIn(CT1<ElemType> elem)
  {
    IntervalIt it = _intervals.begin();

    if ((int)elem.size() != _dims) {
      return false;
    }

    for (ElemType e : elem) {
      if (!(*it).isIn(e)) {
        return false;
      }
      ++it;
    }
    return true;
  }

  MultiIntervalImp cap(MultiIntervalImp &other)
  {
    CT1<Interval> res;
    IntervalIt it_res = res.begin();

    IntervalIt it = other.intervals_ref().begin();
    if (_dims == other.dims()) {
      for (Interval inter : _intervals) {
        Interval cap_res = inter.cap(*it);
        if (cap_res.empty()) {
          CT1<Interval> empty_res;
          return MultiIntervalImp(empty_res);
        }
        it_res = res.insert(it_res, cap_res);
        ++it_res;
        ++it;
      }
    }
    return MultiIntervalImp(res);
  }

  CT2<MultiIntervalImp> diff(MultiIntervalImp &other)
  {
    MultiIntervalImp cap_res = cap(other);

    CT2<MultiIntervalImp> res_mi;
    typename CT2<MultiIntervalImp>::iterator it_res_mi = res_mi.begin();

    if (_intervals.empty()) {
      return res_mi;
    }
    if (_dims != other.dims()) {
      return res_mi;
    }

    if (cap_res.empty()) {
      res_mi.insert(*this);
      return res_mi;
    }

    if (_intervals == cap_res.intervals()) {
      return res_mi;
    }

    IntervalIt it_cap = cap_res.intervals().begin();
    CT1<CT2<Interval>> diffs;
    typename CT1<CT2<Interval>>::iterator it_diffs = diffs.begin();

    for (Interval inter : _intervals) {
      it_diffs = diffs.insert(it_diffs, inter.diff(*it_cap));
      ++it_cap;
      ++it_diffs;
    }

    IntervalIt it = _intervals.begin();
    ++it;
    it_diffs = diffs.begin();

    int count = 0;
    for (CT2<Interval> v_diff : diffs) {
      for (Interval inter : v_diff) {
        if (!inter.empty()) {
          CT1<Interval> res_i;
          IntervalIt it_res_i = res_i.begin();

          it_cap = cap_res._intervals.begin();

          if (count > 0) {
            for (int j = 0; j < count; j++) {
              it_res_i = res_i.insert(it_res_i, *it_cap);
              ++it_res_i;
              ++it_cap;
            }
          }

          it_res_i = res_i.insert(it_res_i, inter);
          ++it_res_i;

          IntervalIt rest_it = it;
          while (rest_it != _intervals.end()) {
            it_res_i = res_i.insert(it_res_i, *rest_it);
            ++it_res_i;
            ++rest_it;
          }

          it_res_mi = res_mi.insert(it_res_mi, MultiIntervalImp(res_i));
          ++it_res_mi;
        }
      }

      ++count;
      ++it;
    }

    return res_mi;
  }

  MultiIntervalImp crossProd(MultiIntervalImp other)
  {
    CT1<Interval> res;
    IntervalIt it_res = res.begin();

    for (Interval inter : _intervals) {
      it_res = res.insert(it_res, inter);
      ++it_res;
    }
    for (Interval inter : other.intervals()) {
      it_res = res.insert(it_res, inter);
      ++it_res;
    }
    return MultiIntervalImp(res);
  }

  CT1<ElemType> minElem()
  {
    CT1<ElemType> res;
    typename CT1<ElemType>::iterator it_res = res.begin();

    for (Interval inter : _intervals) {
      if (inter.empty()) {
        CT1<ElemType> empty;
        return empty;
      }

      it_res = res.insert(it_res, inter.minElem());
      ++it_res;
    }

    return res;
  }

  MultiIntervalImp replace(Interval &replace_inter, int dim)
  {
    CT1<Interval> new_intervals;
    IntervalIt it = new_intervals.begin();
    int count = 1;

    for (Interval inter : _intervals) {
      if (dim == count) {
        it = new_intervals.insert(it, replace_inter);
      } else {
        it = new_intervals.insert(it, inter);
      }
      ++it;
      ++count;
    }

    return MultiIntervalImp(new_intervals);
  }

  int size()
  {
    int res = 1;

    for (Interval inter : _intervals) {
      res *= inter.size();
    }

    if (_intervals.empty()) {
      res = 0;
    }
    return res;
  }

  bool operator<(const MultiIntervalImp &other) const
  {
    typename CT1<Interval>::const_iterator it_other = other._intervals.begin();

    for (Interval inter : _intervals) {
      Interval other_inter = *it_other;
      if (inter.minElem() < other.minElem()) {
        return true;
      }
      ++it_other;
    }
    return false;
  }

  bool operator==(const MultiIntervalImp &other) const { return _intervals == other._intervals; }

  bool operator!=(const MultiIntervalImp &other) const { return _intervals != other._intervals; }

  size_t hash() { return _intervals.size(); }

  protected:
  typedef typename CT1<Interval>::iterator IntervalIt;

  CT1<Interval> _intervals;
  int _dims;
};

template <template <typename T, typename = allocator<T>> class CT1,
          template <typename Value, typename Hash = boost::hash<Value>, typename Pred = std::equal_to<Value>,
                    typename Alloc = std::allocator<Value>>
          class CT2,
          typename Interval, typename ElemType>
size_t hash_value(MultiIntervalImp<CT1, CT2, Interval, ElemType> mi)
{
  return mi.hash();
}

typedef MultiIntervalImp<OrdCT, UnordCT, Interval, Integer> MultiInterval;

typedef UnordCT<MultiInterval> UnordMultiInterval;

ostream &operator<<(ostream &out, MultiInterval &mi);
}  // namespace SB