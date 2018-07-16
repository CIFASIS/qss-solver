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

#ifndef MODEL_DEPENDENCIES_H_
#define MODEL_DEPENDENCIES_H_

#include <string>
#include <map>

#include "../ast/ast_types.h"
#include "util_types.h"
#include "incidence_matrix.h"

using namespace std;

class ModelDependencies 
{
  public:
    ModelDependencies();
    ~ModelDependencies();
  private:
    IncidenceMatrix _SD;
    IncidenceMatrix _DS;
    IncidenceMatrix _ZS;
    IncidenceMatrix _SZ;
};

#endif /* MODEL_DEPENDENCIES_H_ */
