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

#ifndef TABLE_H_
#define TABLE_H_

#include <map>

#include "util_types.h"

template <typename Key, typename Value>
class ModelTable {
  public:
  ModelTable() : _map(){};
  ~ModelTable(){};
  void insert(Key k, Value v)
  {
    _map.erase(k);
    _map.insert(std::pair<Key, Value>(k, v));
  }
  Option<Value> operator[](Key k) const
  {
    if (_map.find(k) == _map.end()) {
      return Option<Value>();
    }
    return _map.at(k);
  }
  void remove(Key k) { _map.erase(k); }
  typedef typename std::map<Key, Value>::iterator iterator;
  typedef typename std::map<Key, Value>::const_iterator const_iterator;
  bool lookup(Key key)
  {
    iterator it = _map.find(key);
    return it != _map.end();
  }
  inline iterator begin() { return _map.begin(); };
  inline const_iterator begin() const { return _map.begin(); };
  inline iterator end() { return _map.end(); };
  inline const_iterator end() const { return _map.end(); };
  inline Value begin(iterator& it)
  {
    it = _map.begin();
    return (end(it) ? Value() : value(it));
  };
  inline bool end(iterator& it) { return it == _map.end(); }
  inline Value next(iterator& it)
  {
    it++;
    return (end(it) ? Value() : value(it));
  }
  inline Value value(iterator& it) { return it->second; }
  inline Key key(iterator& it) { return it->first; };
  void merge(ModelTable<Key, Value> other)
  {
    ModelTable<Key, Value>::iterator it;
    for (Value v = other.begin(it); !other.end(it); v = other.next(it)) {
      insert(other.key(it), v);
    }
  }
  Value first()
  {
    iterator it;
    return begin(it);
  }
  void clear() { _map.clear(); };
  bool empty() { return _map.empty(); };
  const int size() const { return _map.size(); };

  private:
  std::map<Key, Value> _map;
};

#endif /* TABLE_H */
