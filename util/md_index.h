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

#ifndef MD_INDEX_H_
#define MD_INDEX_H_

#include <string>
#include <vector>

#include "index.h"
#include "util_types.h"

using namespace std;

class MDIndex_
{
public:
    /**
     *
     */
    MDIndex_ ();
    /**
     *
     * @param constant
     * @param factor
     */
    MDIndex_ (int constant, int factor);
    /**
     *
     * @param constant
     * @param factor
     * @param low
     * @param high
     */
    MDIndex_ (int constant, int factor, int low, int high);
    /**
     *
     */
    ~MDIndex_ ();
    /**
     *
     * @param val
     * @return
     */
    bool
    checkReverseValue (int val, int dim = 0);
    /**
     *
     * @param l
     */
    void
    setLow (int l, int dim = 0);
    /**
     *
     * @return
     */
    int
    low (int dim = 0) const;
    /**
     *
     * @param h
     */
    void
    setHi (int h, int dim = 0);
    /**
     *
     * @return
     */
    int
    hi (int dim = 0) const;
    /**
     *
     * @param c
     */
    void
    setConstant (int c, int dim = 0);
    /**
     *
     * @return
     */
    int
    mappedConstant (int dim = 0) const;
    /**
     *
     * @return
     */
    int
    operConstant (int dim = 0) const;
    /**
     *
     * @return
     */
    int
    constant (int dim = 0) const;
    /**
     *
     * @param f
     */
    void
    setFactor (int f, int dim = 0);
    /**
     *
     * @return
     */
    int
    factor (int dim = 0) const;
    /**
     *
     * @param o
     */
    void
    setOffset (int o, int dim = 0);
    /**
     *
     * @return
     */
    int
    offset (int dim = 0) const;
    /**
     *
     * @param val
     * @param offset
     * @return
     */
    int
    value (int val = 0, int offset = 0, int dim = 0);
    /**
     * Generates an index copy with the corresponding parameters
     * and the value indicated by val (no factor).
     * @param val
     * @return
     */
    Index
    indexValue (int val, int dim = 0);
    /**
     *
     * @param val
     * @param offset
     * @return
     */
    int
    mappedValue (int val = 0, int offset = 0, int dim = 0);
    /**
     *
     * @param val
     * @return
     */
    int
    modelicaValue (int val, int dim = 0);
    /**
     *
     * @param val
     * @return
     */
    int
    reverseValue (int val, int dim = 0);
    /**
     *
     * @return
     */
    int
    mappedBegin (int dim = 0) const;
    /**
     *
     * @return
     */
    int
    mappedEnd (int dim = 0) const;
    /**
     *
     * @return
     */
    int
    begin (int dim = 0) const;
    /**
     *
     * @return
     */
    int
    end (int dim = 0) const;
    /**
     *
     * @return
     */
    int
    reverseBegin (int dim = 0) const;
    /**
     *
     * @return
     */
    int
    reverseEnd (int dim = 0) const;
    /**
     *
     * @param sub
     * @param offset
     * @return
     */
    string
    print (string sub = "", int offset = 0, bool modelica = true) const;
    /**
     *
     * @param var
     * @param offset
     * @return
     */
    string
    printReverse (string variable, int offset = 0);
    /**
     *
     * @param other
     * @return
     */
    bool
    operator< (const MDIndex_& other) const;
    /**
     *
     * @return
     */
    bool
    isSet (int dim = 0) const;
    /**
     *
     * @return
     */
    bool
    hasRange (int dim = 0) const;
    void
    setRange (int dim = 0);
    /**
     *
     * @return
     */
    int
    range (int dim = 0) const;
    /**
     *
     * @param b
     * @return
     */
    Intersection
    intersection (MDIndex_ b, int dim = 0) const;
    /**
     *
     * @return
     */
    int
    lowValue (int dim = 0) const;
    /**
     *
     * @return
     */
    int
    hiValue (int dim = 0) const;
    /**
     *
     */
    void
    setArray (int dim = 0);
    /**
     *
     * @return
     */
    bool
    isArray (int dim = 0) const;
    /**
     *
     */
    void
    clear (int dim = 0);
    /**
     *
     * @param map
     * @param variable
     */
    void
    setMap (MDIndex_ map, string variable, int dim = 0);
    /**
     *
     * @return
     */
    bool
    hasMap (int dim = 0) const;
    /**
     *
     * @return
     */
    string
    parameter (int dim = 0) const;
    /**
     *
     * @return
     */
    MDIndex_
    map (int dim = 0) const;
    /**
     *
     * @param map
     */
    void
    setMap (MDIndex_ map, int dim = 0);
    /**
     *
     * @return
     */
    bool
    hasRangeOp (int dim = 0);
    /**
     *
     * @param rangeOp
     */
    void
    setRangeOp (bool rangeOp, int dim = 0);
    /**
     *
     * @param idx
     * @return
     */
    string
    definition (string idx, int dim = 0);
    string
    variable (MDIndex_ index, string var, int dim = 0);
    MDIndex_
    variableIndex (MDIndex_ index, int dim = 0);
    MDIndex_
    applyVariableChange (MDIndex_ index, int dim = 0);
    bool
    variableChange (MDIndex_ index, int dim = 0);
    /**
     *
     * @param variable
     * @param offset
     * @return
     */
    string
    printReverseDefinition (string variable, int offset = 0, int dim = 0);
    /**
     *
     * @param other
     * @return
     */
    bool
    operator== (const MDIndex_ &other) const;
    /**
     *
     * @param other
     * @return
     */
    bool
    equalExp (const MDIndex_ &other) const;
    /**
     *
     * @param other
     * @return
     */
    bool
    getIntersection (const MDIndex_ &other) const;
    /**
     *
     * @param dim
     * @return
     */
    bool
    odd (int dim = 0) const;
    /**
     *
     * @param dim
     * @return
     */
    bool
    even (int dim = 0) const;
    /**
     *
     */
    void
    addIndex ();
    /**
     *
     * @param constant
     * @param factor
     */
    void
    addIndex (int constant, int factor);
    /**
     *
     * @param constant
     * @param factor
     * @param low
     * @param high
     */
    void
    addIndex (int constant, int factor, int low, int high);
private:
    vector<Index_> _indexes;
    vector<int> _size;
    int _dimensions;
};

/**
 *
 */
class MDVariableInterval_
{
public:
    /**
     *
     */
    MDVariableInterval_ ();
    /**
     *
     * @param index
     * @param name
     */
    MDVariableInterval_ (MDIndex_ index, string name);
    /**
     *
     */
    ~MDVariableInterval_ ();
    /**
     *
     * @return
     */
    MDIndex_
    index ();
    /**
     *
     */
    string
    name ();
    /**
     *
     * @param index
     */
    void
    setIndex (MDIndex_ index);
    /**
     *
     * @param name
     */
    void
    setName (string name);
    bool
    isEmpty ();
private:
    MDIndex_ _index;
    string _name;

};


#endif /* MD_INDEX_H_ */
