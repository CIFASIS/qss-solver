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

#ifndef STORED_DEFINITION_H_
#define STORED_DEFINITION_H_

#include <string>

using namespace std;

#include <list>

#include <ir/mmo_base.h>
#include <ir/mmo_types.h>

/**
 *
 */
class MMO_StoredDefinition_: public MMO_Base_
{
  public:
    /**
     *
     */
    MMO_StoredDefinition_();
    /**
     *
     */
    ~MMO_StoredDefinition_();
    /**
     *
     * @return
     */
    string
    print();
    /**
     *
     * @return
     */
    list<MMO_Class>
    classes() const;
    /**
     *
     * @param c
     */
    void
    addClass(MMO_Class c);
    /**
     *
     * @return
     */
    MMO_Class
    begin();
    /**
     *
     * @return
     */
    MMO_Class
    next();
    /**
     *
     * @return
     */
    bool
    end();
    private:
    list<MMO_Class> _classList;
    list<MMO_Class>::iterator _it;
};
/**
 *
 * @return
 */
MMO_StoredDefinition
newMMO_StoredDefinition();
/**
 *
 * @param m
 */
void
deleteMMO_StoredDefinition(MMO_StoredDefinition m);

#endif  /* STORED_DEFINITION_H_ */
