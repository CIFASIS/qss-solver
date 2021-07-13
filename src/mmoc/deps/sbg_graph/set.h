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

namespace SB {

template <template <typename T, typename = allocator<T>> class CT1,
          template <typename Value, typename Hash = boost::hash<Value>, typename Pred = std::equal_to<Value>,
                    typename Alloc = std::allocator<Value>>
          class CT2,
          typename AtomSet, typename ElemType>
struct SetImp {
  typedef CT2<AtomSet> SetType;
  typedef typename SetType::iterator SetIt;

  SetImp()
  {
    SetType atom_sets;
    _atomic_sets = atom_sets;
    _dims = 0;
  }
  
  SetImp(SetType set)
  {
    AtomSet atom_set_elem;

    if (!set.empty()) {
      atom_set_elem = *(set.begin());
      int atom_set_dims = atom_set_elem.dims();
      bool equal_dims = true;
      // Check if all atomic sets have the same dimension
      for (AtomSet atom_set : set) {
        if (atom_set_dims != atom_set.dims()) {
          equal_dims = false;
        }
      }

      if (equal_dims && atom_set_dims != 0) {
        _atomic_sets = set;
        _dims = atom_set_dims;
      } else {
        SetType empty;
        _atomic_sets = empty;
        _dims = 0;
      }
    } else {
      _atomic_sets = set;
      _dims = 0;
    }
  }

  SetType atomicSets() const { return _atomic_sets; }

  int dims() const { return _dims; }

  bool empty()
  {
    if (_atomic_sets.empty()) {
      return true;
    }
    bool res = true;
    for (AtomSet as : _atomic_sets) {
      if (!as.empty()) {
        res = false;
      }
    } 
    return res;
  }

  bool isIn(CT1<ElemType> elem)
  {
    for (AtomSet as : _atomic_sets) {
      if (as.isIn(elem)) {
        return true;
      }
    }
    return false;
  }

  void addAtomSet(AtomSet &other)
  {
    if (!other.empty() && other.dims() == _dims && !_atomic_sets.empty()) {
      _atomic_sets.insert(other);
    } else if (!other.empty() && _atomic_sets.empty()) {
      _atomic_sets.insert(other);
      _dims = other.dims();
    }
  }

  void addAtomSets(SetType &other)
  {
    AtomSet new_atom_set;

    SetIt it = other.begin();

    while (it != other.end()) {
      new_atom_set = *it;
      addAtomSet(new_atom_set);
      ++it;
    }
  }

  SetImp cap(SetImp &other)
  {
    AtomSet aux1, aux2;

    if (_atomic_sets.empty() || other.atomicSets().empty()) {
      SetImp empty;
      return empty;
    }

    SetType res;
    for (AtomSet atom_set : _atomic_sets) {
      for (AtomSet other_atom_set : other.atomicSets()) {
        AtomSet cap_res = atom_set.cap(other_atom_set);
        if (!cap_res.empty()) {
          res.insert(cap_res);
        }
      }
    }

    return SetImp(res);
  }

  SetImp diff(SetImp &other)
  {
    SetImp res;
    SetType cap_res = cap(other).atomicSets();

    if (!cap_res.empty()) {
      for (AtomSet atom_set : _atomic_sets) {
        SetType diff_set;
        diff_set.insert(atom_set);

        for (AtomSet cap_atom_set : cap_res) {
          SetImp new_sets;

          for (AtomSet partial_diff_atom_set : diff_set) {
            SetType diff_res = partial_diff_atom_set.diff(cap_atom_set);
            new_sets.addAtomSets(diff_res);
          }
          diff_set = new_sets.atomicSets();
        }

        res.addAtomSets(diff_set);
      }
    } else {
      res.addAtomSets(_atomic_sets);
    }
    return res;
  }

  SetImp cup(SetImp &other)
  {
    SetImp res = *this;
    SetImp diff_other = other.diff(*this);

    if (!diff_other.empty()) {
        SetType diff_atomics = diff_other.atomicSets();
        res.addAtomSets(diff_atomics);
    }
    return res;
  }

  SetImp crossProd(SetImp &other)
  {
    SetType res;

    for (AtomSet atom_set : _atomic_sets.end()) {
      for (AtomSet other_atom_set : other.atomicSets()) {
        AtomSet partial_cross_prod = atom_set.crossProd(other_atom_set);
        res.addAtomSet(partial_cross_prod);
      }
    }
    return SetImp(res);
  }

  bool subsetEq(SetImp &other)
  {
    SetImp set_diff = (*this).diff(other);

    if (set_diff.empty()) {
      return true;
    }
    return false;
  }

  bool subset(SetImp &other)
  {
    SetImp set_diff_other = (*this).diff(other);
    SetImp set_diff = other.diff(*this);

    if (set_diff_other.empty() && !set_diff.empty()) {
      return true;
    }
    return false;
  }

  CT1<ElemType> minElem()
  {
    CT1<ElemType> res;

    if (empty()) {
      return res;
    }
    
    AtomSet min = *(_atomic_sets.begin());

    for (AtomSet atom_set : _atomic_sets) {
      if (atom_set.atomicSets().minElem() < min.minElem()) {
        min = atom_set;
      }
    }
    return min.minElem();
  }

  int size()
  {
    int res = 0;

    for (AtomSet atom_set : _atomic_sets) {
      res += atom_set.size();
    }

    return res;
  }

  bool operator==(const SetImp &other) const { return _atomic_sets == other._atomic_sets; }

  bool operator!=(const SetImp &other) const { return _atomic_sets != other._atomic_sets; }

  size_t hash() { return _atomic_sets.size(); }

  protected:
  SetType _atomic_sets;
  int _dims;
};

template <template <typename T, typename = allocator<T>> class CT1,
          template <typename Value, typename Hash = boost::hash<Value>, typename Pred = std::equal_to<Value>,
                    typename Alloc = std::allocator<Value>>
          class CT2,
          typename AtomSet, typename ElemType>
size_t hash_value(SetImp<CT1, CT2, AtomSet, ElemType> s)
{
  return s.hash();
}

typedef SetImp<OrdCT, UnordCT, AtomSet, Integer> Set;

typedef OrdCT<Set> OrdSet;

ostream &operator<<(ostream &out, Set &s);
} // namespace SB
