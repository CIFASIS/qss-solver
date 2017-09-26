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

#include "index.h"

#include <sstream>

#include <iostream>

Index_::Index_() :
    _constant(-1), _factor(0), _offset(0), _low(1), _hi(1), _isSet(false), _array(
        false), _map(NULL), _parameter(), _rangeOp(false), _hasRange(
        false)
{
}

Index_::Index_(int constant, int factor, int low, int hi) :
    _constant(constant), _factor(factor), _offset(0), _low(low), _hi(hi), _isSet(
        true), _array(false), _map(NULL), _parameter(), _rangeOp(
        false), _hasRange(false)
{
  _constant--;
}

Index_::Index_(int constant, int factor) :
    _constant(constant), _factor(factor), _offset(0), _low(1), _hi(1), _isSet(
        true), _array(false), _map(NULL), _parameter(), _rangeOp(
        false), _hasRange(false)
{
  _constant--;
}

Index_::~Index_()
{
}

void
Index_::setLow(int l)
{
  _low = l;
  _setIndex();
}

int
Index_::low() const
{
  if(_low < 1)
  {
    return 1;
  }
  else
  {
    return _low;
  }
}

void
Index_::setHi(int h)
{
  _hi = h;
  _setIndex();
}

int
Index_::hi() const
{
  if(_hi < 1)
  {
    return 1;
  }
  else
  {
    return _hi;
  }
}

int
Index_::mappedConstant() const
{
  if(_factor != 0)
  {
    return operConstant() + _offset;
  }
  else
  {
    return constant() + _offset;
  }
}

void
Index_::setConstant(int c)
{
  _constant = c - 1;
  _setIndex();
}

string
Index_::parameter() const
{
  return _parameter;
}

Index_
Index_::map() const
{
  return Index_(*_map);
}

bool
Index_::hasRangeOp()
{
  return _rangeOp;
}

void
Index_::setRangeOp(bool rangeOp)
{
  _rangeOp = rangeOp;
  _setIndex();
}

void
Index_::setRange()
{
  _hasRange = true;
  _setIndex();
}

bool
Index_::equalExp(const Index_& other) const
    {
  return operConstant() == other.operConstant() && factor() == other.factor();
}

bool
Index_::getIntersection(const Index_& other) const
    {
  if(factor() == 1 || other.factor() == 1)
  {
    return true;
  }
  else
  {
    return (odd() && other.odd()) || (even() && other.even());
  }
}

bool
Index_::odd() const
{
  return factor() % 2 == 0 && operConstant() % 2 != 0;
}

bool
Index_::even() const
{
  return factor() % 2 == 0 && operConstant() % 2 == 0;
}

void
Index_::_setIndex()
{
  _isSet = true;
}

/*! \brief Returns the mapped constant of the index expression  \f$ constant + factor * idx\f$.
 *
 * 	\return Mapped constant.
 *
 * 	Description:
 * 		We consider the following cases:
 * 			-# If \f$ factor = 0\f$ then, then return the mapped constant. 
 * 			   The value of constant has to be greater than zero, because Modelica indexes begin at one.
 * 			   If we get a negative index constant, then there must be an error in the index generator 
 * 			   algorithm.
 * 			-# If \f$ factor <> 0 \f$ and \f$ constant = -1\f$ (corresponding to \f$ factor*idx\f$), then 
 * 			   return zero, given that no constant value is defined (the original value is zero). Otherwise 
 * 			   return the mapped constant.
 *
 * 	\note{This is the only place in the code where the index constant should be modified.}
 *
 */

int
Index_::constant() const
{
  if(_constant == -1)
  {
    return 0;
  }
  else
  {
    return _constant;
  }
}

int
Index_::operConstant() const
{
  if(factor() < 0)
  {
    return _constant;
  }
  else
  {
    return _constant + 1;
  }
}

void
Index_::setFactor(int f)
{
  _factor = f;
  if(f == 0)
  {
    _hasRange = false;
  }
  _setIndex();
}

int
Index_::factor() const
{
  return _factor;
}

void
Index_::setOffset(int o)
{
  _offset = o;
  _setIndex();
}

int
Index_::offset() const
{
  return _offset;
}

int
Index_::reverseValue(int val)
{
  int ret = 0;
  if(_factor != 0)
  {
    ret = (val - mappedConstant()) / _factor;
  }
  else
  {
    ret = mappedConstant();
  }
  return ret;
}

int
Index_::value(int val, int offset)
{
  int ret = 0;
  if(_factor != 0)
  {
    ret = _factor * val + operConstant();
  }
  else
  {
    ret = constant();
  }
  return ret;
}

int
Index_::mappedValue(int val, int offset)
{
  int ret = 0;
  if(_factor != 0)
  {
    ret = _factor * (val + offset) + mappedConstant();
  }
  else
  {
    ret = mappedConstant();
  }
  return ret;
}

int
Index_::modelicaValue(int val)
{
  int ret = 0;
  ret = _factor * (val + 1) + operConstant();
  return ret;
}

string
Index_::definition(string idx)
{
  stringstream buffer;
  string map;
  if(_map != NULL)
  {
    map = "__PAR_" + _parameter + "[" + _map->print(idx) + "]";
  }
  else
  {
    map = idx;
  }
  int cte = operConstant();
  if(_factor != 0)
  {
    if(_factor == 1)
    {
      if(cte == 0)
      {
        buffer << map;
      }
      else if(cte > 0)
      {
        buffer << map << "+" << cte;
      }
      else
      {
        buffer << map << cte;
      }
    }
    else
    {
      if(cte == 0)
      {
        buffer << _factor << "*" << map;
      }
      else if(cte > 0)
      {
        buffer << _factor << "*" << map << "+" << cte;
      }
      else
      {
        buffer << _factor << "*" << map << cte;
      }
    }
  }
  else
  {
    buffer << mappedConstant();
  }
  return buffer.str();

}

string
Index_::print(string sub, int offset, bool solver) const
    {
  string map;
  if(_map != NULL)
  {
    if(solver)
    {
      map = "__PAR_";
    }
    map += _parameter + "[" + _map->print(sub, offset) + "]";
  }
  else
  {
    map = sub;
  }
  ostringstream buffer;
  int cte = operConstant() + _offset + offset;
  if(_map != NULL)
  {
    int solverOffset = _offset;
    if(solver)
    {
      solverOffset = _offset - 1;
    }
    cte = operConstant() + solverOffset;
  }
  if(_factor != 0)
  {
    if(_factor == 1)
    {
      if(cte == 0)
      {
        buffer << map;
      }
      else if(cte > 0)
      {
        buffer << map << "+" << cte;
      }
      else
      {
        buffer << map << cte;
      }
    }
    else
    {
      if(cte == 0)
      {
        buffer << _factor << "*" << map;

      }
      else if(cte > 0)
      {
        buffer << _factor << "*" << map << "+" << cte;
      }
      else
      {
        buffer << _factor << "*" << map << cte;
      }
    }
  }
  else
  {
    buffer << mappedConstant();
  }
  return buffer.str();
}

string
Index_::printReverseDefinition(string variable, int offset)
{
  ostringstream buffer;
  int cte = operConstant() + offset;
  if(_map != NULL)
  {
    cte = operConstant() + 1;
  }
  string var;
  if(_map != NULL)
  {
    var = "__reverse__PAR_" + _parameter + "[" + _map->print(variable, offset)
        + "]";
  }
  else
  {
    var = variable;
  }
  if(_factor != 0)
  {
    if(_factor != 1)
    {
      if(cte > 0)
      {
        buffer << "(" << var << "-" << cte << ")" << "/" << _factor;
      }
      else if(cte < 0)
      {
        buffer << "(" << var << "+" << -cte << ")" << "/" << _factor;
      }
      else
      {
        buffer << var << "/" << _factor;
      }
    }
    else
    {
      if(cte > 0)
      {
        buffer << var << "-" << cte;
      }
      else if(cte < 0)
      {
        buffer << var << "+" << -cte;
      }
      else
      {
        buffer << var;
      }
    }
  }
  else
  {
    buffer << mappedConstant();
  }
  return buffer.str();
}

string
Index_::printReverse(string variable, int offset)
{
  ostringstream buffer;
  int cte = operConstant() + _offset + offset;
  if(_map != NULL)
  {
    cte = operConstant() + _offset + 1;
  }
  string var;
  if(_map != NULL)
  {
    var = "__reverse__PAR_" + _parameter + "[" + _map->print(variable, offset)
        + "]";
  }
  else
  {
    var = variable;
  }
  if(_factor != 0)
  {
    if(_factor != 1)
    {
      if(cte > 0)
      {
        buffer << "(" << var << "-" << cte << ")" << "/" << _factor;
      }
      else if(cte < 0)
      {
        buffer << "(" << var << "+" << -cte << ")" << "/" << _factor;
      }
      else
      {
        buffer << var << "/" << _factor;
      }
    }
    else
    {
      if(cte > 0)
      {
        buffer << var << "-" << cte;
      }
      else if(cte < 0)
      {
        buffer << var << "+" << -cte;
      }
      else
      {
        buffer << var;
      }
    }
  }
  else
  {
    buffer << mappedConstant();
  }
  return buffer.str();
}

bool
Index_::operator<(const Index_& other) const
    {
  Compare c;
  return c(this, &other);
}

int
Index_::begin() const
{
  return low() - 1;
}

int
Index_::end() const
{
  return hi() - 1;
}

int
Index_::mappedBegin() const
{
  return mappedConstant();
}

int
Index_::mappedEnd() const
{
  return mappedConstant() + _hi - _low;
}

int
Index_::reverseBegin() const
{
  return _low - 1;
}

int
Index_::reverseEnd() const
{
  return _hi - 1;
}

bool
Index_::isSet() const
{
  return _isSet;
}

bool
Index_::hasRange() const
{
  return _hasRange;
}

int
Index_::range() const
{
  return _hi - _low + 1;
}

bool
Index_::checkReverseValue(int val)
{
  int rev = reverseValue(val);
  return rev >= begin() && rev <= end();
}

int
Index_::lowValue() const
{
  return mappedConstant() + _factor * begin();
}

void
Index_::clear()
{
  _constant = 0;
  _factor = 0;
  _low = 0;
  _hi = 0;
  _offset = 0;
  _isSet = false;
  _array = false;
  _rangeOp = false;
  _hasRange = false;
}

int
Index_::hiValue() const
{
  return mappedConstant() + _factor * end();
}

Intersection
Index_::intersection(Index_ b) const
    {
  Intersection is;
  if(hasRange() && b.hasRange())
  {
    if(b.lowValue() >= lowValue() && b.hiValue() <= hiValue())
    {
      if(!equalExp(b) && !getIntersection(b))
      {
        is.setType(IDX_DISJOINT);
      }
      else
      {
        is.setType(IDX_SUBSET);
        is.setLow(b.low());
        is.setHi(b.hi());
      }
    }
    else if(lowValue() >= b.lowValue() && hiValue() <= b.hiValue())
    {
      if(!equalExp(b) && !getIntersection(b))
      {
        is.setType(IDX_DISJOINT);
      }
      else
      {
        is.setType(IDX_SUBSET_OF);
        is.setLow(low());
        is.setHi(hi());
      }
    }
    else if(b.lowValue() < lowValue() && lowValue() <= b.hiValue()
        && b.hiValue() < hiValue())
    {
      if(!equalExp(b) && !getIntersection(b))
      {
        is.setType(IDX_DISJOINT);
      }
      else
      {

        is.setType(IDX_ARB_AB);
        is.setLow(low());
        is.setHi(b.hi());
      }
    }
    else if(lowValue() < b.lowValue() && b.lowValue() <= hiValue()
        && hiValue() < b.hiValue())
    {
      if(!equalExp(b) && !getIntersection(b))
      {
        is.setType(IDX_DISJOINT);
      }
      else
      {
        is.setType(IDX_ARB_BA);
        is.setLow(b.low());
        is.setHi(hi());
      }
    }
    is.setRange(true);
  }
  else if(hasRange() && !b.hasRange())
  {
    if(b.mappedConstant() >= lowValue() && b.mappedConstant() <= hiValue())
    {
      is.setLow(low());
      is.setHi(hi());
      is.setType(IDX_CONSTANT_BA);
      is.setValue(b.mappedConstant() - lowValue());
    }
  }
  else if(!hasRange() && b.hasRange())
  {
    if(mappedConstant() >= b.lowValue() && mappedConstant() <= b.hiValue())
    {
      is.setLow(b.low());
      is.setHi(b.hi());
      is.setType(IDX_CONSTANT_AB);
      is.setValue(mappedConstant() - b.lowValue());
    }
  }
  else if(mappedConstant() == b.mappedConstant())
  {
    is.setType(IDX_EQUAL);
    is.setValue(constant());
  }
  return is;
}

void
Index_::setArray()
{
  _array = true;
}

bool
Index_::isArray() const
{
  return _array;
}

Intersection::Intersection() :
    _low(1), _hi(1), _type(-1), _value(0), _hasRange(false)
{
}

Intersection::~Intersection()
{
}

int
Intersection::begin()
{
  return _low - 1;
}

int
Intersection::end()
{
  return _hi - 1;
}

bool
Intersection::hasRange()
{
  return _hasRange;
}

void
Intersection::setRange(bool range)
{
  _hasRange = range;
}

bool
Intersection::equal(Intersection b)
{
  if(_type != b.type())
  {
    return false;
  }
  switch(_type)
  {
    case IDX_DISJOINT:
      return false;
    case IDX_SUBSET:
      case IDX_SUBSET_OF:
      case IDX_ARB_BA:
      case IDX_ARB_AB:
      return _low == b.low() && _hi == b.hi();
    default:
      return _value == b.value();
  }
  return false;
}

int
Intersection::low()
{

  return _low;
}

int
Intersection::hi()
{
  return _hi;
}

void
Intersection::setLow(int l)
{
  _low = l;
}

void
Intersection::setHi(int h)
{
  _hi = h;
}

void
Intersection::setType(int type)
{
  _type = type;
}

void
Intersection::setValue(int value)
{
  _value = value;
}

int
Intersection::type()
{
  return _type;
}

int
Intersection::value()
{
  return _value;
}

/*VariableInterval_::VariableInterval_ () :
 _index (), _name ()
 {
 }

 VariableInterval_::VariableInterval_ (Index_ index, string name) :
 _index (index), _name (name)
 {
 }

 VariableInterval_::~VariableInterval_ ()
 {
 }

 void
 VariableInterval_::setIndex (Index_ index)
 {
 _index = index;
 }

 Index_
 VariableInterval_::index ()
 {
 return _index;
 }

 void
 VariableInterval_::setName (string name)
 {
 _name = name;
 }

 string
 VariableInterval_::name ()
 {
 return _name;
 }*/

Index_
Index_::indexValue(int val)
{
  Index_ ret(*this);
  if(hasRange())
  {
    ret.setConstant(val);
    ret.setFactor(0);
    ret.setLow(1);
    ret.setHi(1);
  }
  return ret;
}

void
Index_::setMap(Index_ map, string variable)
{
  _map = new Index_(map);
  _parameter = variable;
}

void
Index_::setMap(Index_ map)
{
  _map = new Index_(map.map());
  _parameter = map.parameter();
}

bool
Index_::hasMap() const
{
  return _map != NULL;
}

/*bool
 VariableInterval_::isEmpty ()
 {
 return _name == "";
 }*/

string
Index_::variable(Index_ index, string var)
{
  string ret = var;
  if(factor() != index.factor() || operConstant() != index.operConstant())
  {
    Index_ retIndex = *this;
    retIndex.setFactor(index.factor() * factor());
    retIndex.setConstant(factor() * index.operConstant() + operConstant());
    ret = retIndex.definition(var);
  }
  return ret;
}

Index_
Index_::variableIndex(Index_ index)
{
  Index_ retIndex = *this;
  if(factor() != index.factor() || operConstant() != index.operConstant())
  {
    retIndex = applyVariableChange(index);
  }
  return retIndex;
}
Index_
Index_::applyVariableChange(Index_ index)
{
  Index_ retIndex = *this;
  retIndex.setFactor(index.factor() * factor());
  retIndex.setConstant(factor() * index.operConstant() + operConstant());
  return retIndex;
}

bool
Index_::variableChange(Index_ index)
{
  bool ret = false;
  if(factor() != index.factor() || operConstant() != index.operConstant())
  {
    ret = true;
  }
  return ret;
}

int
Intersection::modelicaValue()
{
  return _value + 1;
}

bool
Index_::operator ==(const Index_& other) const
    {
  return this->_constant == other._constant && this->_factor == other._factor
      && this->_hasRange == other._hasRange && this->_hi == other._hi
      && this->_index == other._index && this->_isSet == other._isSet
      && this->_low == other._low && this->_offset == other._offset
      && this->_map == other._map && this->_parameter == other._parameter
      && this->_rangeOp == other._rangeOp && this->_array == other._array;
}

int
Intersection::range()
{
  return end() - begin() + 1;
}
