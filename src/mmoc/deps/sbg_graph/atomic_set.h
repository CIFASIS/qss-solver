#pragma once

#include <deps/sbg_graph/container_def.h>
#include <deps/sbg_graph/interval.h>
#include <deps/sbg_graph/multi_interval.h>

namespace SB {

template <template <typename T, typename = allocator<T>> class CT1,
          template <typename Value, typename Hash = boost::hash<Value>, typename Pred = std::equal_to<Value>,
                    typename Alloc = std::allocator<Value>>
          class CT2,
          typename MultiInterval, typename Interval, typename ElemType>
struct AtomSetImp {

  AtomSetImp()
  {
    MultiInterval empty;
    _atom_set = empty;
    _dims = 0;
  }
  AtomSetImp(MultiInterval atom_set)
  {
    _atom_set = atom_set;
    _dims = atom_set.dims();
  }

  MultiInterval atomicSets() { return _atom_set; }

  int dims() { return _dims; }

  bool empty() { return _atom_set.empty(); }

  bool isIn(CT1<ElemType> elem) { return _atom_set.isIn(elem); }

  AtomSetImp cap(AtomSetImp &other)
  {
    MultiInterval other_mi = other.atomicSets();
    AtomSetImp cap_res(_atom_set.cap(other_mi));
    return cap_res;
  }

  CT2<AtomSetImp> diff(AtomSetImp &other)
  {
    CT2<AtomSetImp> res;
    typename CT2<AtomSetImp>::iterator it_res = res.begin();
    MultiInterval other_mi = other.atomicSets();
    CT2<MultiInterval> atomic_diff = _atom_set.diff(other_mi);
    
    if (atomic_diff.empty()) {
      CT2<AtomSetImp> empty;
      return empty;
    } else {
      for (MultiInterval mi : atomic_diff) {
        it_res = res.insert(it_res, AtomSetImp(mi));
        ++it_res;
      }
    }
    return res;
  }

  AtomSetImp crossProd(AtomSetImp &other) { return AtomSetImp(_atom_set.crossProd(other.atomicSets())); }

  CT1<ElemType> minElem() { return _atom_set.minElem(); }

  AtomSetImp replace(Interval &inter, int dim) { return AtomSetImp(_atom_set.replace(inter, dim)); }

  int size() { return _atom_set.size(); }

  bool operator==(const AtomSetImp &other) const { return _atom_set == other._atom_set; }

  bool operator!=(const AtomSetImp &other) const { return _atom_set != other._atom_set; }

  size_t hash() { return _atom_set.hash(); }

  protected:
  MultiInterval _atom_set;
  int _dims;

};

template <template <typename T, typename = allocator<T>> class CT1,
          template <typename Value, typename Hash = boost::hash<Value>, typename Pred = std::equal_to<Value>,
                    typename Alloc = std::allocator<Value>>
          class CT2,
          typename MultiInterval, typename Interval, typename ElemType>
size_t hash_value(AtomSetImp<CT1, CT2, MultiInterval, Interval, ElemType> as)
{
  return as.hash();
}

typedef AtomSetImp<OrdCT, UnordCT, MultiInterval, Interval, Integer> AtomSet;

typedef UnordCT<AtomSet> UnordAtomSet;

ostream &operator<<(ostream &out, AtomSet &as);
} // namespace SB