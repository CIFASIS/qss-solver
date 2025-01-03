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

#include <list>
#include <utility>

#include <boost/config.hpp>
#include <boost/unordered_set.hpp>

namespace SB {

typedef int Integer;
typedef float Real;

template <typename T, class = std::allocator<T>>
using OrdCT = std::list<T>;

template <typename Value, typename Hash = boost::hash<Value>, typename Pred = std::equal_to<Value>, typename Alloc = std::allocator<Value>>
using UnordCT = boost::unordered_set<Value>;

typedef OrdCT<Integer> OrdIntegerCT;
typedef OrdCT<Real> OrdRealCT;
} // namespace SB
