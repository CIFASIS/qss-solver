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

#ifndef DEPENDENCIES_H_
#define DEPENDENCIES_H_

using namespace std;

#include <util/ast_util.h>
#include <util/util_types.h>
#include <util/symbol_table.h>

/**
 *
 */
typedef enum
{
  DEP_STATE,          //!< DEP_STATE
  DEP_DISCRETE,       //!< DEP_DISCRETE
  DEP_ALGEBRAIC,      //!< DEP_ALGEBRAIC
  DEP_ALGEBRAIC_DEF,  //!< DEP_ALGEBRAIC_DEF
  DEP_ALGEBRAIC_STATE,  //!< DEP_ALGEBRAIC_STATE
  DEP_ALGEBRAIC_DISCRETE,  //!< DEP_ALGEBRAIC_DISCRETE
  DEP_STATE_VECTOR,   //!< DEP_STATE_VECTOR
  DEP_DISCRETE_VECTOR,   //!< DEP_DISCRETE_VECTOR
  DEP_ALGEBRAIC_VECTOR,   //!< DEP_ALGEBRAIC_VECTOR
  DEP_ALGEBRAIC_VECTOR_DEF,   //!< DEP_ALGEBRAIC_VECTOR_DEF
  DEP_ALGEBRAIC_STATES  //!< DEP_ALGEBRAIC_STATES
} DEP_Type;

/**
 *
 */
class Dependencies_
{
  public:
    /**
     *
     */
    Dependencies_();
    /**
     *
     */
    ~Dependencies_();
    /**
     *
     * @return
     */
    bool
    autonomous();
    /**
     *
     * @param a
     */
    void
    setAutonomous(bool a);
    /**
     *
     * @param d
     */
    void
    join(Dependencies d);
    /**
     *
     * @param d
     * @param index
     * @param t
     */
    void
    join(Dependencies d, Index index, DEP_Type t);
    /**
     *
     * @param d
     * @param val
     */
    void
    join(Dependencies d, int val);
    /**
     *
     * @param d
     * @param t
     */
    void
    join(Dependencies d, DEP_Type t);
    /**
     *
     * @param d
     * @param t
     * @param val
     */
    void
    join(Dependencies d, DEP_Type t, int val);
    /**
     *
     * @param d
     * @param st
     * @param t
     * @param val
     */
    void
    join(Dependencies d, Index st, DEP_Type t, int val);
    /**
     *
     * @param d
     * @param index
     */
    void
    join(Dependencies d, Index index);

    /**
     *
     * @param vi
     */
    void
    insert(VarInfo vi);
    /**
     *
     * @param idx
     * @param t
     */
    void
    insert(Index idx, DEP_Type t);
    void
    insert(Index idx, Index dep, DEP_Type t);
    Index
    key(DEP_Type t);
    Index
    key(DEP_Type t, int at);
    /**
     *
     * @param t
     * @return
     */
    Index *
    begin(DEP_Type t);
    /**
     *
     * @param t
     * @return
     */
    bool
    end(DEP_Type t);
    /**
     *
     * @param t
     * @return
     */
    Index *
    next(DEP_Type t);
    /**
     *
     * @param idx
     * @param t
     * @return
     */
    bool
    find(Index idx, DEP_Type t);
    /**
     *
     * @return
     */
    int
    states();
    /**
     *
     * @return
     */
    int
    discretes();
    /**
     *
     * @return
     */
    int
    algebraics();
    /**
     *
     * @return
     */
    int
    stateVectors();
    /**
     *
     * @return
     */
    int
    discreteVectors();
    /**
     *
     * @return
     */
    bool
    hasStates();
    /**
     *
     * @return
     */
    bool
    hasDiscretes();
    /**
     *
     * @return
     */
    bool
    hasAlgebraics();
    bool
    hasAlgebraicDiscretes();
    bool
    hasAlgebraicStates();
    bool
    isVector(Index *idx);
    Index *
    algebraicState(Index *idx);
    void
    insert(Index idx, string stateId, DEP_Type type);
    string
    identifier(Index idx, DEP_Type type);
    private:
    MMO_IndexTable
    _select(DEP_Type t);
    map<Index, string>
    _identifier(DEP_Type t);
    bool
    _testIntersection(MMO_IndexTable m, Index key);
    MMO_IndexTable _states;
    MMO_IndexTable _discretes;
    MMO_IndexTable _algebraics;
    MMO_IndexTable _algebraicsDefs;
    MMO_IndexTable _algebraicsState;
    MMO_IndexTable _algebraicsDiscrete;
    MMO_IndexTable _stateVectors;
    MMO_IndexTable _discreteVectors;
    MMO_IndexTable _algebraicVectorsDef;
    MMO_IndexTable _algebraicVectors;
    MMO_IndexTable _algebraicStates;
    map<Index, string> _stateIds;
    map<Index, string> _algebraicIds;
    bool _autonomous;
    int _stateVectorsCount;
    int _discreteVectorsCount;
    int _algebraicVectorsCount;
};
/**
 *
 * @return
 */
Dependencies
newDependencies();
/**
 *
 * @param m
 */
void
deleteDependencies(Dependencies m);

#endif  /* DEPENDENCIES_H_ */
