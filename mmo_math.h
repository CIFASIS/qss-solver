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

#ifndef MMO_MATH_H_
#define MMO_MATH_H_

#include <sbml/common/libsbml-namespace.h>
#include <sbml/math/ASTNode.h>
#include <list>
#include <map>
#include <string>
#include <utility>

class MMOAssignment;
class MMOEquation;
class MMOZeroCrossing;

using namespace std;

LIBSBML_CPP_NAMESPACE_USE

/**
 *
 */
class MMOMath
{
public:
    /**
     *
     */
    ~MMOMath ();
    /**
     *
     * @return
     */
    bool
    endAlgebraic ();
    /**
     *
     * @return
     */
    bool
    endAsg ();
    /**
     *
     * @return
     */
    bool
    endEvent ();
    /**
     *
     * @return
     */
    bool
    endZC ();
    /**
     *
     * @return
     */
    bool
    hasAssignment ();
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
    /**
     *
     * @param i
     * @return
     */
    bool
    isPositive (int i);
    /**
     *
     * @return
     */
    list<string>
    getVariables ();
    /**
     *
     * @param replace
     * @param functions
     */
    MMOMath (bool replace, map<string, pair<list<string>, ASTNode*> > *functions, string prefix = "");
    /**
     *
     * @return
     */
    pair<ASTNode *, list<pair<string, ASTNode *> > >
    firstEvent ();
    /**
     *
     * @return
     */
    pair<ASTNode *, list<pair<string, ASTNode *> > >
    nextEvent ();
    /**
     *
     * @return
     */
    pair<string, pair<string, list<string> > >
    firstAlgebraic ();
    /**
     *
     * @return
     */
    pair<string, string>
    firstZC ();
    /**
     *
     * @return
     */
    pair<string, pair<string, list<string> > >
    nextAlgebraic ();
    /**
     *
     * @return
     */
    pair<string, string>
    nextZC ();
    /**
     *
     * @return
     */
    string
    firstAsg ();
    /**
     *
     * @return
     */
    string
    getExp ();
    /**
     *
     * @return
     */
    string
    nextAsg ();
    /**
     *
     * @return
     */
    bool
    hasImports ();
    /**
     *
     * @return
     */
    string
    firstImport ();
    /**
     *
     * @return
     */
    string
    nextImport ();
    /**
     *
     * @return
     */
    bool
    endImport ();
    /**
     *
     * @param node
     * @param asgVariable
     */
    void
    parseAssignment (ASTNode *node, string asgVariable);
    /**
     *
     * @param node
     */
    void
    parseEquation (const ASTNode *node);
    /**
     *
     * @param node
     */
    void
    parseZeroCrossing (ASTNode *node);
    /**
     *
     * @return
     */
    bool
    isConditional ();
private:
    /**
     *
     */
    typedef enum
    {
        MATH_EQUATION, MATH_ZERO_CROSSING, MATH_ASSIGNMENT
    } MATH_type;
    void
    _processNode (ASTNode *node);
    pair<string, list<string> >
    _generateAlgebraic (pair<list<string>, ASTNode*> function, list<string> args, ASTNode *node);
    void
    _getVariables (const ASTNode *node, list<string> *ret);
    void
    _replaceFunctions (ASTNode *node);
    MMOEquation *_equation;
    MMOZeroCrossing *_zerocrossing;
    MMOAssignment *_assignment;
    map<string, pair<string, list<string> > > _algebraics;
    unsigned int _currentAlgebraic;
    MATH_type _type;
    ASTNode *_exp;
    map<string, pair<list<string>, ASTNode*> >*_functions;
    bool _replace;
    map<string, string> _imports;
    unsigned int _currentImport;
    string _prefix;
    string _asgVariable;
    bool _isConditional;
};

#endif /* MMO_MATH_H */
