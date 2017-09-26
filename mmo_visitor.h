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

#ifndef MMO_VISITOR_H_
#define MMO_VISITOR_H_

#include "mmo_event.h"
#include "mmo_function.h"
#include "mmo_section.h"

class MMODecl;
class MMOEvent;
class MMOExp;
class MMOFunction;
class MMOMath;
class MMOModel;
class MMOSection;

/**
 *
 */
class MMOVisitor
{
  public:
    /**
     *
     */
    virtual
    ~MMOVisitor()
    {
    }
    ;
    /**
     *
     * @param x
     */
    virtual void
    visit(MMOExp *x) = 0;
    /**
     *
     * @param x
     */
    virtual void
    visit(MMODecl *x) = 0;
    /**
     *
     * @param x
     */
    virtual void
    visit(MMODecl x) = 0;
    /**
     *
     * @param x
     */
    virtual void
    leave(MMODecl *x) = 0;
    /**
     *
     * @param x
     */
    virtual void
    visit(MMOFunction *x) = 0;
    /**
     *
     * @param x
     */
    virtual void
    visit(MMOFunction x) = 0;
    /**
     *
     * @param x
     */
    virtual void
    leave(MMOFunction *x) = 0;
    /**
     *
     * @param x
     */
    virtual void
    visit(MMOEvent *x) = 0;
    /**
     *
     * @param x
     */
    virtual void
    visit(MMOEvent x) = 0;
    /**
     *
     * @param x
     */
    virtual void
    leave(MMOEvent *x) = 0;
    /**
     *
     * @param x
     */
    virtual void
    visit(MMOSection *x) = 0;
    /**
     *
     * @param x
     */
    virtual void
    visit(MMOSection x) = 0;
    /**
     *
     * @param x
     */
    virtual void
    leave(MMOSection *x) = 0;
};

#endif /* MMO_VISITOR_H_ */
