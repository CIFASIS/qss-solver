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

#ifndef MMO_UTIL_H_
#define MMO_UTIL_H_

#include <list>
#include <string>

#include "../ast/ast_types.h"
#include "../util/index.h"

/**
 *
 */
class MMO_ModelConfig
{
  public:
    /**
     *
     */
    MMO_ModelConfig();
    /**
     *
     */
    ~MMO_ModelConfig();
};

/**
 *
 */
class MMO_FunctionConfig
{
  public:
    /**
     *
     * @param var
     * @param name
     * @param args
     * @param data
     */
    MMO_FunctionConfig();
    /**
     *
     */
    ~MMO_FunctionConfig();
};

/**
 *
 */
class MMO_PackageConfig
{
  public:
    /**
     *
     * @param name
     * @param fit
     * @param objects
     */
    MMO_PackageConfig();
    /**
     *
     */
    ~MMO_PackageConfig();
};

#endif  /* MMO_UTIL_H_ */
