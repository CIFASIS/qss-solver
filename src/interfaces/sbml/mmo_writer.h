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

#ifndef MMO_WRITER_H_
#define MMO_WRITER_H_

#include <fstream>
#include <list>
#include <string>

#include "mmo_event.h"
#include "mmo_function.h"
#include "mmo_section.h"
#include "mmo_visitor.h"

class MMODecl;
class MMOMath;
class MMOEvent;
class MMOFunction;
class MMOSection;
class MMOExp;

using namespace std;

/**
 *
 */
typedef enum
{
  WR_MMO,
  WR_BR
} WR_Type;

/**
 *
 */
class MMOWriter: public MMOVisitor
{
  public:
    /**
     *
     * @param file
     */
    MMOWriter(string file);
    /**
     *
     */
    ~MMOWriter();
    /**
     *
     * @param x
     */
    void
    visit(MMOExp *x);
    /**
     *
     * @param x
     */
    void
    visit(MMODecl *x);
    /**
     *
     * @param x
     */
    void
    visit(MMODecl x);
    /**
     *
     * @param x
     */
    void
    leave(MMODecl *x);
    /**
     *
     * @param x
     */
    void
    visit(MMOEvent *x);
    /**
     *
     * @param x
     */
    void
    visit(MMOEvent x);
    /**
     *
     * @param x
     */
    void
    leave(MMOEvent *x);
    /**
     *
     * @param x
     */
    void
    visit(MMOFunction *x);
    /**
     *
     * @param x
     */
    void
    visit(MMOFunction x);
    /**
     *
     * @param x
     */
    void
    leave(MMOFunction *x);
    /**
     *
     * @param x
     */
    void
    visit(MMOSection *x);
    /**
     *
     * @param x
     */
    void
    visit(MMOSection x);
    /**
     *
     * @param x
     */
    void
    leave(MMOSection *x);
    private:
    int _indent;
    string _modelName;
    fstream _out;
    list<string> _outputs;
    bool _condition;
    bool _initialAsignment;
};

#endif /* MMO_WRITER_H */
