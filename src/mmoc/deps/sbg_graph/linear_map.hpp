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

namespace SB {

template <template <typename T, typename = std::allocator<T>> class CT, typename ElemType>
struct LMapImp {
  typedef CT<ElemType> ElemContainer;
  typedef typename ElemContainer::iterator ElemContainerIt;

  LMapImp() { initialize(); }

  LMapImp(ElemContainer slopes, ElemContainer constants)
  {
    initialize();
    bool negative = false;

    for (ElemType slope : slopes) {
      if (slope < 0) {
        negative = true;
      }
    }
    if (!negative) {
      if (slopes.size() == constants.size()) {
        _slopes = slopes;
        _constants = constants;
        _dims = slopes.size();
      }
    }
  }

  // Constructs the id of LMaps
  LMapImp(int dim)
  {
    ElemContainer slopes;
    ElemContainerIt slopes_it = slopes.begin();
    ElemContainer constants;
    ElemContainerIt constants_it = constants.begin();

    for (int i = 0; i < dim; i++) {
      slopes_it = slopes.insert(slopes_it, 1.0);
      ++slopes_it;
      constants_it = constants.insert(constants_it, 0);
      ++constants_it;
    }

    _slopes = slopes;
    _constants = constants;
    _dims = dim;
  }

  ElemContainer slopes() { return _slopes; }

  ElemContainer constants() { return _constants; }

  int dims() { return _dims; }

  bool empty()
  {
    if (_slopes.empty() && _constants.empty()) {
      return true;
    }
    return false;
  }

  void add(ElemType slope, ElemType constant)
  {
    _slopes.insert(_slopes.end(), slope);
    _constants.insert(_constants.end(), constant);
    ++_dims;
  }

  LMapImp compose(LMapImp &other)
  {
    ElemContainer res_slopes;
    ElemContainerIt res_slopes_it = res_slopes.begin();
    ElemContainer res_constants;
    ElemContainerIt res_constants_it = res_constants.begin();

    ElemContainerIt constants_it = _constants.begin();
    ElemContainerIt other_slopes_it = other.slopes().begin();
    ElemContainerIt other_constants_it = other.constants().begin();

    if (_dims == other.dims()) {
      for (ElemType slope : _slopes) {
        res_slopes_it = res_slopes.insert(res_slopes_it, slope * (*other_slopes_it));
        ++res_slopes_it;
        res_constants_it = res_constants.insert(res_constants_it, (*other_constants_it) * slope + (*constants_it));
        ++res_constants_it;

        ++constants_it;
        ++other_slopes_it;
        ++other_constants_it;
      }
    } else {
      LMapImp empty;
      return empty;
    }

    return LMapImp(res_slopes, res_constants);
  }

  LMapImp invlinearMap()
  {
    ElemContainer res_slopes;
    ElemContainerIt res_slopes_it = res_slopes.begin();
    ElemContainer res_constants;
    ElemContainerIt res_constants_it = res_constants.begin();

    ElemContainerIt constants_it = _constants.begin();

    for (ElemType slope : _slopes) {
      if (slope != 0) {
        res_slopes_it = res_slopes.insert(res_slopes_it, 1 / slope);
        ++res_slopes_it;
        res_constants_it = res_constants.insert(res_constants_it, -(*constants_it) / slope);
        ++res_constants_it;
      } else {
        res_slopes_it = res_slopes.insert(res_slopes_it, Inf);
        ++res_slopes_it;
        res_constants_it = res_constants.insert(res_constants_it, -Inf);
        ++res_constants_it;
      }
      ++constants_it;
    }

    return LMapImp(res_slopes, res_constants);
  }

  bool operator==(const LMapImp &other) const { return _slopes == other._slopes && _constants == other._constants; }

  protected:
  void initialize()
  {
    ElemContainer empty_slopes;
    ElemContainer empty_constants;
    _slopes = empty_slopes;
    _constants = empty_constants;
    _dims = 0;
  }

  std::string mapOper(Real cte) { return (cte >= 0) ? "+ " : ""; }

  ElemContainer _slopes;
  ElemContainer _constants;
  int _dims;
};

typedef LMapImp<OrdCT, Real> LMap;
typedef OrdCT<LMap> OrdLMap;

ostream &operator<<(ostream &out, LMap &lm);
} // namespace SB