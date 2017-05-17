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

#ifndef MMO_EQUATION_H_
#define MMO_EQUATION_H_

#include <sbml/common/libsbml-namespace.h>
#include <sbml/math/ASTNode.h>
#include <list>
#include <string>
#include <utility>

class MMOSection;

using namespace std;

LIBSBML_CPP_NAMESPACE_USE
/**
 *
 */
class MMOEquation
{
public:
    /**
     *
     */
    MMOEquation ();
    /**
     *
     * @param node
     */
    MMOEquation (const ASTNode *node);
    /**
     *
     */
    ~MMOEquation ();
    /**
     *
     * @return
     */
    ASTNode *
    getEquation ();
    /**
     *
     * @return
     */
    bool
    hasEquation ();
    /**
     *
     * @return
     */
    bool
    hasZeroCrossing ();
    pair<ASTNode *, list<pair<string, ASTNode *> > >
    /**
     *
     * @return
     */
    first ();
    /**
     *
     * @return
     */
    pair<ASTNode *, list<pair<string, ASTNode *> > >
    next ();
    /**
     *
     * @return
     */
    bool
    end ();
    /**
     *
     * @return
     */
    ASTNode *
    ASTExpression ();
    /**
     *
     * @param b
     */
    void
    joinEvents (MMOEquation *b);
private:
    void
    _parseEquation (ASTNode *node);
    pair<ASTNode *, list<pair<string, ASTNode *> > >
    _make_pair ();
    ASTNode *_equation;
    bool _hasEquation;
    list<pair<ASTNode *, list<pair<string, ASTNode*> > > > _events;
    list<pair<ASTNode *, list<pair<string, ASTNode*> > > >::iterator _it;
};

#endif /* MMO_EQUATION_H_ */
