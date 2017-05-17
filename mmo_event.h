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

#ifndef MMO_EVENT_H_
#define MMO_EVENT_H_

#include <list>
#include <string>

#include "mmo_decl.h"

using namespace std;

/**
 *
 */
typedef enum
{
    positive, //!< positive
    negative //!< negative
} MMOHandlerType;

/**
 *
 */
class MMOEvent : public MMOExp
{
public:
    /**
     *
     * @param id
     */
    MMOEvent (string id, bool generateInitialAsignment = false);
    /**
     *
     */
    ~MMOEvent ();
    /**
     *
     * @param visitor
     */
    void
    accept (MMOVisitor *visitor);
    /**
     *
     * @param decl
     * @param type
     */
    void
    add (MMODecl decl, MMOHandlerType type);
    /**
     *
     * @param decl
     */
    void
    add (MMODecl decl);
    /**
     *
     * @return
     */
    string
    getId ()
    {
        return (_id);
    }
    ;
    /**
     *
     * @return
     */
    bool
    generateInitialAsignments ();
    /**
     *
     * @param ia
     */
    void
    setInitialAlgorithm (bool ia);
    /**
     *
     * @return
     */
    bool
    initialAlgorithm ();
private:
    string _id;
    MMODecl _zero_crossing;
    MMODecl _condition;
    list<MMODecl> _handler_pos;
    list<MMODecl> _handler_neg;
    bool _generateInitialAsignment;
    bool _initialAlgorithm;
};

#endif  /* MMO_EVENT_H_ */
