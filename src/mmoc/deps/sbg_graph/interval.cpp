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
#include <deps/sbg_graph/interval.h>

std::string mapOper(Real cte) { return (cte >= 0) ? "+ " : ""; }

ostream &operator<<(ostream &out, LMap &lm)
{
  OrdRealCT slopes = lm.slopes();
  OrdRealCT::iterator slopes_it = slopes.begin();
  OrdRealCT constants = lm.constants();
  OrdRealCT::iterator constants_it = constants.begin();

  if (slopes.size() == 0) {
    return out;
  }

  if (slopes.size() == 1) {
    out << "[" << *slopes_it << " * x " << mapOper(*constants_it) << *constants_it << "]";
    return out;
  }

  out << "[" << *slopes_it << " * x " << mapOper(*constants_it) << *constants_it;
  ++slopes_it;
  ++constants_it;
  while (next(slopes_it, 1) != slopes.end()) {
    out << ", " << *slopes_it << " * x " << mapOper(*constants_it) << *constants_it;

    ++slopes_it;
    ++constants_it;
  }
  out << ", " << *slopes_it << " * x " << mapOper(*constants_it) << *constants_it << "]";

  return out;
}


