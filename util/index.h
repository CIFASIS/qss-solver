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

#ifndef INDEX_H_
#define INDEX_H_

#include <string>

#include "util_types.h"

using namespace std;

/**
 *
 */
#define IDX_DISJOINT 	-1
/**
 *
 */
#define IDX_EQUAL    	-2
/**
 *
 */
#define IDX_SUBSET   	-4
/**
 *
 */
#define	IDX_SUBSET_OF 	-5
/**
 *
 */
#define IDX_ARB_AB 	 	-6
/**
 *
 */
#define IDX_ARB_BA 		-7
/**
 *
 */
#define IDX_CONSTANT_AB 	-8
/**
 *
 */
#define IDX_CONSTANT_BA 	-9

/**
 *
 */
class Intersection
{
  public:
    /**
     *
     */
    Intersection();
    /**
     *
     */
    ~Intersection();
    /**
     *
     * @return
     */
    int
    begin();
    /**
     *
     * @return
     */
    int
    end();
    /**
     *
     * @param b
     * @return
     */
    bool
    equal(Intersection b);
    /**
     *
     * @param type
     */
    void
    setType(int type);
    /**
     *
     * @param value
     */
    void
    setValue(int value);
    /**
     *
     * @return
     */
    int
    modelicaValue();
    /**
     *
     * @return
     */
    int
    type();
    /**
     *
     * @return
     */
    int
    value();
    void
    setLow(int l);
    int
    low();
    void
    setHi(int h);
    int
    hi();
    bool
    hasRange();
    void
    setRange(bool range);
    int
    range();
    private:
    int _low;
    int _hi;
    int _type;
    int _value;
    bool _hasRange;
};

/**
 *
 */
class Index_
{
  public:
    /**
     *
     */
    Index_();
    /**
     *
     * @param constant
     * @param factor
     */
    Index_(int constant, int factor);
    /**
     *
     * @param constant
     * @param factor
     * @param low
     * @param high
     */
    Index_(int constant, int factor, int low, int high);
    /**
     *
     */
    ~Index_();
    /**
     *
     * @param val
     * @return
     */
    bool
    checkReverseValue(int val);
    /**
     *
     * @param l
     */
    void
    setLow(int l);
    /**
     *
     * @return
     */
    int
    low() const;
    /**
     *
     * @param h
     */
    void
    setHi(int h);
    /**
     *
     * @return
     */
    int
    hi() const;
    /**
     *
     * @param c
     */
    void
    setConstant(int c);
    /**
     *
     * @return
     */
    int
    mappedConstant() const;
    /**
     *
     * @return
     */
    int
    operConstant() const;
    /**
     *
     * @return
     */
    int
    constant() const;
    /**
     *
     * @param f
     */
    void
    setFactor(int f);
    /**
     *
     * @return
     */
    int
    factor() const;
    /**
     *
     * @param o
     */
    void
    setOffset(int o);
    /**
     *
     * @return
     */
    int
    offset() const;
    /**
     *
     * @param val
     * @param offset
     * @return
     */
    int
    value(int val = 0, int offset = 0);
    /**
     * Generates an index copy with the corresponding parameters
     * and the value indicated by val (no factor).
     * @param val
     * @return
     */
    Index_
    indexValue(int val);
    /**
     *
     * @param val
     * @param offset
     * @return
     */
    int
    mappedValue(int val = 0, int offset = 0);
    /**
     *
     * @param val
     * @return
     */
    int
    modelicaValue(int val);
    /**
     *
     * @param val
     * @return
     */
    int
    reverseValue(int val);
    /**
     *
     * @return
     */
    int
    mappedBegin() const;
    /**
     *
     * @return
     */
    int
    mappedEnd() const;
    /**
     *
     * @return
     */
    int
    begin() const;
    /**
     *
     * @return
     */
    int
    end() const;
    /**
     *
     * @return
     */
    int
    reverseBegin() const;
    /**
     *
     * @return
     */
    int
    reverseEnd() const;
    /**
     *
     * @param sub
     * @param offset
     * @return
     */
    string
    print(string sub = "", int offset = 0, bool solver = true) const;
    /**
     *
     * @param var
     * @param offset
     * @return
     */
    string
    printReverse(string variable, int offset = 0);
    /**
     *
     * @param other
     * @return
     */
    bool
    operator<(const Index_& other) const;
    /**
     *
     * @return
     */
    bool
    isSet() const;
    /**
     *
     * @return
     */
    bool
    hasRange() const;
    void
    setRange();
    /**
     *
     * @return
     */
    int
    range() const;
    /**
     *
     * @param b
     * @return
     */
    Intersection
    intersection(Index_ b) const;
    /**
     *
     * @return
     */
    int
    lowValue() const;
    /**
     *
     * @return
     */
    int
    hiValue() const;
    /**
     *
     */
    void
    setArray();
    /**
     *
     * @return
     */
    bool
    isArray() const;
    /**
     *
     */
    void
    clear();
    /**
     *
     * @param map
     * @param variable
     */
    void
    setMap(Index_ map, string variable);
    /**
     *
     * @return
     */
    bool
    hasMap() const;
    /**
     *
     * @return
     */
    string
    parameter() const;
    /**
     *
     * @return
     */
    Index_
    map() const;
    /**
     *
     * @param map
     */
    void
    setMap(Index_ map);
    /**
     *
     * @return
     */
    bool
    hasRangeOp();
    /**
     *
     * @param rangeOp
     */
    void
    setRangeOp(bool rangeOp);
    /**
     *
     * @param idx
     * @return
     */
    string
    definition(string idx);
    string
    variable(Index_ index, string var);
    Index_
    variableIndex(Index_ index);
    Index_
    applyVariableChange(Index_ index);
    bool
    variableChange(Index_ index);
    /**
     *
     * @param variable
     * @param offset
     * @return
     */
    string
    printReverseDefinition(string variable, int offset = 0);
    /**
     *
     * @param other
     * @return
     */
    bool
    operator==(const Index_ &other) const;
    bool
    equalExp(const Index_ &other) const;
    bool
    getIntersection(const Index_ &other) const;
    bool
    odd() const;
    bool
    even() const;
    private:
    void
    _setIndex();
    int _constant;
    int _factor;
    int _offset;
    int _low;
    int _hi;
    string _index;
    bool _isSet;
    bool _array;
    Index_ *_map;
    string _parameter;
    bool _rangeOp;
    bool _hasRange;
};

/**
 *
 */
class Compare
{
  public:
    /**
     *
     * @param x
     * @param y
     * @return
     */
    bool
    operator()(const Index_ *x, const Index_ *y) const
        {
      if(x->hasMap() && y->hasMap())
      {
        if(x->offset() != y->offset())
        {
          return x->offset() < y->offset();
        }
        else if(x->parameter() == y->parameter())
        {
          if(x->map() < y->map())
          {
            return true;
          }
          else
          {
            return x->operConstant() < y->operConstant();
          }
        }
        else
        {
          return x->parameter() < y->parameter();
        }
      }
      else if(x->hasMap() && !y->hasMap())
      {
        return true;
      }
      else if(!x->hasMap() && y->hasMap())
      {
        return false;
      }
      if(x->factor() != 0 && y->factor() == 0)
      {
        if(x->lowValue() == y->lowValue())
        {
          return true;
        }
      }
      if(x->factor() != 0 && y->factor() != 0)
      {
        if(x->lowValue() == y->lowValue())
        {
          if(x->offset() != y->offset())
          {
            return x->low() < y->low();
          }
          return x->range() < y->range();
        }
      }
      return x->lowValue() < y->lowValue();
    }
};

/**
 *
 */
//class VariableInterval_
//{
//public:
/**
 *
 */
//    VariableInterval_ ();
/**
 *
 * @param index
 * @param name
 */
//    VariableInterval_ (Index_ index, string name);
/**
 *
 */
//    ~VariableInterval_ ();
/**
 *
 * @return
 */
//    Index_
//    index ();
/**
 *
 */
//    string
//    name ();
/**
 *
 * @param index
 */
//    void
//   setIndex (Index_ index);
/**
 *
 * @param name
 */
//    void
//    setName (string name);
//    bool
//    isEmpty ();
//};
#endif  /* INDEX_H_ */
