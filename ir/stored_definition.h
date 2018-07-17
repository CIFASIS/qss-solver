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

#ifndef MMO_STORED_DEFINITION_H_
#define MMO_STORED_DEFINITION_H_

#include <string>

using namespace std;

#include <list>
#include "../ir/model.h"
#include "../ir/package.h"

/**
 *
 */
class MMO_StoredDefinition
{
  public:
    /**
     *
     */
    MMO_StoredDefinition();
    /**
     *
     */
    ~MMO_StoredDefinition();
    MMO_Class*
    begin();
    MMO_Class*
    next();
    bool 
    end();
  private:
    list<MMO_Model>   _stdModel;
    list<MMO_Package> _stdPackage;
};

#endif  /* MMO_STORED_DEFINITION_H_ */
