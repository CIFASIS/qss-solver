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

template<typename Key,typename Value>
class ModelTable : public std::map<Key,Value> 
{
  public:
    ModelTable(){};
    ~ModelTable() {};
    void insert(Key k, Value v) 
    {
	    std::map<Key,Value>::erase(k);
      std::map<Key,Value>::insert(std::pair<Key,Value>(k,v));
    }
    Option<Value> operator[](Key k) const 
    {
      if (std::map<Key,Value>::find(k)==std::map<Key,Value>::end())
      {
        return Option<Value>();
      }
      return std::map<Key,Value>::at(k); 
    }
    void remove(Key k) 
    {
      std::map<Key,Value>::erase(k);
    }
};

#endif /* TABLE_H */
