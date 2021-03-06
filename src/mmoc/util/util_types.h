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

#ifndef UTIL_TYPES_H_
#define UTIL_TYPES_H_

#include <string>
#include <boost/optional.hpp>
#include <boost/foreach.hpp>

#include "macros.h"

#define foreach_ BOOST_FOREACH

template <typename T>
struct Option : public boost::optional<T> {
  public:
  Option() : boost::optional<T>(){};
  Option(boost::optional<T> t) : boost::optional<T>(t){};
  Option(T const &t) : boost::optional<T>(t){};
};

DEFINE_TYPE(AST_MicroModelica);
/**
 *
 */
typedef std::string VarName;
/**
 *
 */
typedef std::string TypeName;

#endif /*  UTIL_TYPES_H_ */
