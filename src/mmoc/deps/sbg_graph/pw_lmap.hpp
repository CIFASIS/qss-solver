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
#include <deps/sbg_graph/pw_atom_map.h>
#include <deps/sbg_graph/set.h>

namespace SB {

template <template <typename T, class = allocator<T>> class CT1,
          template <typename Value, typename Hash = boost::hash<Value>, typename Pred = std::equal_to<Value>,
                    typename Alloc = std::allocator<Value>>
          class CT2,
          typename PWAtomMap, typename LMap, typename Set, typename AtomSet, typename Integer, typename Real>
struct PWLMapImp {
  typedef CT1<Set> SetContainer;
  typedef typename CT1<Set>::iterator SetContainerIt;
  typedef CT1<LMap> LMapContainer;
  typedef typename CT1<LMap>::iterator LMapContainerIt;


  PWLMapImp() { initialize(); }
  PWLMapImp(SetContainer dom, LMapContainer lmap)
  {
    initialize();
    LMapContainerIt lmap_it = lmap.begin();
    int dom_dims = (*(dom.begin())).dims();
    bool different = false;

    if (dom.size() == lmap.size()) {
      for (Set set : dom) {
        for (AtomSet atom_set : set.atomicSets()) {
          PWAtomMap pw_atom(atom_set, *lmap_it);
          if (pw_atom.empty()) {
            different = true;
          }
        }
        ++lmap_it;
      }

      if (!different) {
        _dom = dom;
        _lmap = lmap;
        _dims = dom_dims;
      }
    } 
  }
  
  // Id PWLMap, the set stays the same, the map is the id map
  PWLMapImp(Set &set)
  {
    SetContainer dom;
    LMapContainer lmaps;

    LMap lmap(set.dims());

    dom.insert(dom.begin(), set);
    lmaps.insert(lmaps.begin(), lmap);

    _dom = dom;
    _lmap = lmaps;
    _dims = 1;
  }

  SetContainer dom() { return _dom; }

  LMapContainer linearMap() { return _lmap; }

  int dims() { return _dims; }

  bool empty() { return _dom.empty() && _lmap.empty(); }

  void addSetLM(Set s, LMap lm)
  {
    _dom.insert(_dom.end(), s);
    _lmap.insert(_lmap.end(), lm);
    PWLMapImp new_pw_map(_dom, _lmap);

    _dom = new_pw_map.dom();
    _lmap = new_pw_map.linearMap();
  }

  void addLMSet(LMap lm, Set s)
  {
    _dom.insert(_dom.begin(), s);
    _lmap.insert(_lmap.begin(), lm);
    PWLMapImp new_pw_map(_dom, _lmap);

    _dom = new_pw_map.dom();
    _lmap = new_pw_map.linearMap();
  }

  Set image(Set &s)
  {
    LMapContainerIt lmap_it = _lmap.begin();

    Set res;

    for (Set set : _dom) {
      Set set_inter = set.cap(s);
      Set partial_res;

      CT2<AtomSet> inter_atom_sets = set_inter.atomicSets();
      for (AtomSet atom_set : inter_atom_sets) {
        PWAtomMap atom_set_map(atom_set, *lmap_it);
        AtomSet atom_set_image = atom_set_map.image(atom_set);
        partial_res.addAtomSet(atom_set_image);
      }
      res = res.cup(partial_res);
      ++lmap_it;
    }

    return res;
  }

  Set preImage(Set &sub_set)
  {
    LMapContainerIt lmap_it = _lmap.begin();

    Set res;

    for (Set set : _dom) {
      Set partial_res;

      CT2<AtomSet> atomic_sets = set.atomicSets();
      for (AtomSet atom_set : atomic_sets) {
        PWAtomMap atom_map(atom_set, *lmap_it);

        CT2<AtomSet> sub_set_atomics = sub_set.atomicSets();
        for (AtomSet sub_set_atomic : sub_set_atomics) {
          AtomSet sub_set_pre_image = atom_map.preImage(sub_set_atomic);
          partial_res.addAtomSet(sub_set_pre_image);
        }
      }
      res = res.cup(partial_res);
      ++lmap_it;
    }

    return res;
  }

  PWLMapImp compPW(PWLMapImp &other)
  {
    LMapContainerIt lmap_it = _lmap.begin();
    LMapContainerIt other_lmap_it = other.linearMap().begin();

    SetContainer res;
    SetContainerIt res_it = res.begin();
    LMapContainer res_lm;
    LMapContainerIt res_lm_it = res_lm.begin();

    Set int_set;
    Set new_dom;

    for (Set set : _dom) {
      other_lmap_it = other.linearMap().begin();

      for (Set other_set : other.dom()) {
        int_set = other.image(other_set);
        int_set = int_set.cap(set);
        int_set = other.preImage(int_set);
        new_dom = int_set.cap(other_set);

        if (!new_dom.empty()) {
          LMap new_lm((*lmap_it).compose(*other_lmap_it));

          res_it = res.insert(res_it, new_dom);
          ++res_it;
          res_lm_it = res_lm.insert(res_lm_it, new_lm);
          ++res_lm_it;
        }

        ++other_lmap_it;
      }

      ++lmap_it;
    }

    return PWLMapImp(res, res_lm);
  }

  PWLMapImp minInvCompact(Set s)
  {
    SetContainer res;
    LMapContainer lm_res;

    if (_dom.size() == 1) {
      Set whole_set = wholeDom();
      CT1<Integer> minElem = whole_set.minElem();
      typename CT1<Integer>::iterator min_it = minElem.begin();
      Set im = image(whole_set);
      Set inv = im.cap(s);

      if (inv.empty()) {
        PWLMapImp empty;
        return empty;
      }

      res.insert(res.begin(), inv);

      LMap lm = *(_lmap.begin());
      LMap lm_inv = lm.invlinearMap();
      CT1<Real> slopes = lm_inv.slopes();
      CT1<Real> constants = lm_inv.constants();
      typename CT1<Real>::iterator constants_it = constants.begin();
      CT1<Real> slopes_res;
      typename CT1<Real>::iterator slopes_res_it = slopes_res.begin();
      CT1<Real> constants_res;
      typename CT1<Real>::iterator constants_res_it = constants_res.begin();

      for (Real slope : slopes) {
        if (slope == Inf) {
          slopes_res_it = slopes_res.insert(slopes_res_it, 0);
          constants_res_it = constants_res.insert(constants_res_it, *min_it);
        } else {
          slopes_res_it = slopes_res.insert(slopes_res_it, slope);
          constants_res_it = constants_res.insert(constants_res_it, *constants_it);
        }

        ++constants_it;
        ++slopes_res_it;
        ++constants_res_it;
        ++min_it;
      }

      LMap new_lmap(slopes_res, constants_res);
      lm_res.insert(lm_res.begin(), new_lmap);
    }

    PWLMapImp res_pw_map(res, lm_res);
    return res_pw_map;
  }

  Set wholeDom()
  {
    Set res;
    for (Set s : _dom) {
      res = res.cup(s);
    }
    return res;
  }

  PWLMapImp combine(PWLMapImp &other)
  {
    SetContainer set_res = _dom;
    SetContainerIt set_res_it = set_res.end();
    LMapContainer lmap_res = _lmap;
    LMapContainerIt lmap_res_it = lmap_res.end();

    if (empty()) {
      return other;
    } else if (other.empty()) {
      return *this;
    } else {
      Set whole_dom = wholeDom();
      SetContainer other_dom = other.dom();
      LMapContainer other_lmap = other.linearMap();
      LMapContainerIt other_lmap_it = other_lmap.begin();

      for (Set other_set : other_dom) {
        Set new_dom = other_set.diff(whole_dom);

        if (!new_dom.empty()) {
          set_res_it  = set_res.insert(set_res_it , new_dom);
          ++set_res_it;
          lmap_res_it = lmap_res.insert(lmap_res_it, *other_lmap_it);
          ++lmap_res_it;
        }
        ++other_lmap_it;
      }
    }

    PWLMapImp res(set_res, lmap_res);
    return res;
  }

  PWLMapImp atomize()
  {
    SetContainer dom_res;
    SetContainerIt dom_res_it = dom_res.begin();
    LMapContainer lmap_res;
    LMapContainerIt lmap_res_it = lmap_res.begin();

    LMapContainerIt lmap_it = _lmap.begin();
    for (Set d : _dom) {
      for (AtomSet atom_set : d.atomicSetss()) {
        Set new_set;
        new_set.addAtomSet(atom_set);
        dom_res_it = dom_res.insert(dom_res_it, new_set);
        ++dom_res_it;
        lmap_res_it = lmap_res.insert(lmap_res_it, *lmap_it);
        ++lmap_res_it;
      }
      ++lmap_it;
    }

    PWLMapImp res(dom_res, lmap_res);
    return res;
  }

  PWLMapImp concat(PWLMapImp &other)
  {
    SetContainerIt dom_it = _dom.end();
    LMapContainerIt lmap_it = _lmap.end();

    for (Set s : other.dom()) {
      dom_it = _dom.insert(dom_it, s);
      ++dom_it;
    }

    for (LMap lm : other.linearMap()) {
      lmap_it = _lmap.insert(lmap_it, lm);
      ++lmap_it;
    }

    PWLMapImp res(_dom, _lmap);
    return res;
  }

  PWLMapImp restrictMap(Set new_dom)
  {
    SetContainer dom_res;
    SetContainerIt dom_res_it = dom_res.begin();
    LMapContainer lmap_res;
    LMapContainerIt lmap_res_it = lmap_res.begin();

    SetContainerIt dom_it = _dom.begin();
    LMapContainerIt lmap_it = _lmap.begin();

    for (; dom_it != _dom.end(); ++dom_it) {
      Set scap = new_dom.cap(*dom_it);

      if (!scap.empty()) {
        dom_res_it = dom_res.insert(dom_res_it, scap);
        ++dom_res_it;
        lmap_res_it = lmap_res.insert(lmap_res_it, *lmap_it);
        ++lmap_res_it;
      }
      ++lmap_it;
    }

    PWLMapImp res(dom_res, lmap_res);
    return res;
  }

  bool operator==(const PWLMapImp &other) const { return _dom == other.dom() && _lmap == other.linearMap(); }

  protected:
  void initialize()
  {
    SetContainer empty_dom;
    LMapContainer empty_lmap;
    _dom = empty_dom;
    _lmap = empty_lmap;
    _dims = 0;
  }

  SetContainer _dom;
  LMapContainer _lmap;
  int _dims;
};

typedef PWLMapImp<OrdCT, UnordCT, PWAtomLMap, LMap, Set, AtomSet, Integer, Real> PWLMap;

ostream &operator<<(ostream &out, PWLMap &pw);
} // namespace SB