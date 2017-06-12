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

#include "md_index.h"

MDIndex_::MDIndex_ () :
        _indexes (), _size (), _dimensions (1)
{
    Index_ idx;
    _indexes.push_back (idx);
}

MDIndex_::MDIndex_ (int constant, int factor) :
        _indexes (), _size (), _dimensions (1)
{
    Index_ idx (constant, factor);
    _indexes.push_back (idx);
}

MDIndex_::MDIndex_ (int constant, int factor, int low, int high) :
        _indexes (), _size (), _dimensions (1)
{
    Index_ idx (constant, factor, low, high);
    _indexes.push_back (idx);
}

MDIndex_::~MDIndex_ ()
{
}

bool
MDIndex_::checkReverseValue (int val, int dim)
{
    bool ret = true;
    for (int i = 0; i < _dimensions; i++)
    {
        ret = ret && _indexes[i].checkReverseValue(val);
    }
    return (ret);
}

void
MDIndex_::setLow (int l, int dim)
{
    _indexes[dim].setLow(l);
}

int
MDIndex_::low (int dim) const
{
    return (_indexes[dim].low());
}

void
MDIndex_::setHi (int h, int dim)
{
    _indexes[dim].setHi(h);
}

int
MDIndex_::hi (int dim) const
{
    return (_indexes[dim].hi());
}

void
MDIndex_::setConstant (int c, int dim)
{
    _indexes[dim].setConstant(c);
}

int
MDIndex_::mappedConstant (int dim) const
{
    return (_indexes[dim].mappedConstant());
}

int
MDIndex_::operConstant (int dim) const
{
    return (_indexes[dim].operConstant());
}

int
MDIndex_::constant (int dim) const
{
    return (_indexes[dim].constant());
}

void
MDIndex_::setFactor (int f, int dim)
{
    _indexes[dim].setFactor(f);
}

int
MDIndex_::factor (int dim) const
{
    return (_indexes[dim].factor());
}

void
MDIndex_::setOffset (int o, int dim)
{
}

int
MDIndex_::offset (int dim) const
{
    return (0);
}

int
MDIndex_::value (int val, int offset, int dim)
{
    return (0);
}

MDIndex_
MDIndex_::indexValue (int val, int dim)
{
    return (MDIndex_ ());
}

int
MDIndex_::mappedValue (int val, int offset, int dim)
{
    return (0);
}

int
MDIndex_::modelicaValue (int val, int dim)
{
    return (0);
}

int
MDIndex_::reverseValue (int val, int dim)
{
    return (0);
}

int
MDIndex_::mappedBegin (int dim) const
{
    return (0);
}

int
MDIndex_::mappedEnd (int dim) const
{
    return (0);
}

int
MDIndex_::begin (int dim) const
{
    return (0);
}

int
MDIndex_::end (int dim) const
{
    return (0);
}

int
MDIndex_::reverseBegin (int dim) const
{
    return (0);
}

int
MDIndex_::reverseEnd (int dim) const
{
    return (0);
}

string
MDIndex_::print (string sub, int offset, bool modelica) const
{
    return ("");
}

string
MDIndex_::printReverse (string variable, int offset)
{
    return ("");
}

bool
MDIndex_::operator < (const MDIndex_& other) const
{
    return (true);
}

bool
MDIndex_::isSet (int dim) const
{
    return ("");
}

bool
MDIndex_::hasRange (int dim) const
{
    return ("");
}

void
MDIndex_::setRange (int dim)
{
}

int
MDIndex_::range (int dim) const
{
    return (0);
}

Intersection
MDIndex_::intersection (MDIndex_ b) const
{
    return (Intersection ());
}

int
MDIndex_::lowValue (int dim) const
{
    return (0);
}

int
MDIndex_::hiValue (int dim) const
{
    return (0);
}

void
MDIndex_::setArray (int dim)
{
}

bool
MDIndex_::isArray (int dim) const
{
    return (true);
}

void
MDIndex_::clear (int dim)
{
}

void
MDIndex_::setMap (MDIndex_ map, string variable, int dim)
{
}

bool
MDIndex_::hasMap (int dim) const
{
    return (true);
}

string
MDIndex_::parameter (int dim) const
{
    return ("");
}

MDIndex_
MDIndex_::map (int dim) const
{
    return (MDIndex_ ());
}

void
MDIndex_::setMap (MDIndex_ map, int dim)
{
}

bool
MDIndex_::hasRangeOp (int dim)
{
    return (true);
}

void
MDIndex_::setRangeOp (bool rangeOp, int dim)
{
}

string
MDIndex_::definition (string idx, int dim)
{
    return ("");
}

string
MDIndex_::variable (MDIndex_ index, string var, int dim)
{
    return ("");
}

MDIndex_
MDIndex_::variableIndex (MDIndex_ index, int dim)
{
    return (MDIndex_ ());
}

MDIndex_
MDIndex_::applyVariableChange (MDIndex_ index, int dim)
{
    return (MDIndex_ ());
}

bool
MDIndex_::variableChange (MDIndex_ index, int dim)
{
    return (true);
}

string
MDIndex_::printReverseDefinition (string variable, int offset, int dim)
{
    return ("");
}

bool
MDIndex_::operator == (const MDIndex_& other) const
{
    return (true);
}

bool
MDIndex_::equalExp (const MDIndex_& other) const
{
    return (true);
}

bool
MDIndex_::getIntersection (const MDIndex_& other) const
{
    return (true);
}

bool
MDIndex_::odd (int dim) const
{
    return (true);
}

bool
MDIndex_::even (int dim) const
{
    return (true);
}

void
MDIndex_::addIndex ()
{
    Index_ idx;
    _indexes.push_back(idx);
    _size.push_back(1);
    _dimensions++;
}

void
MDIndex_::addIndex (int constant, int factor)
{
    Index_ idx(constant, factor);
    _indexes.push_back(idx);
    _size.push_back(idx.range());
    _dimensions++;
}

void
MDIndex_::addIndex (int constant, int factor, int low, int high)
{
    Index_ idx(constant, factor, low, high);
    _indexes.push_back(idx);
    _size.push_back(idx.range());
    _dimensions++;
}

MDVariableInterval_::MDVariableInterval_ ()
{
}

MDVariableInterval_::MDVariableInterval_ (MDIndex_ index, string name)
{
}

MDVariableInterval_::~MDVariableInterval_ ()
{
}

MDIndex_
MDVariableInterval_::index ()
{
    return (MDIndex_());
}

string
MDVariableInterval_::name ()
{
    return ("");
}

void
MDVariableInterval_::setIndex (MDIndex_ index)
{
}

void
MDVariableInterval_::setName (string name)
{
}

bool
MDVariableInterval_::isEmpty ()
{
    return (true);
}
