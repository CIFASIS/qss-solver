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

#include <iostream>
#include <sstream>
#include <math.h>

#include "md_index.h"

MDIndex_::MDIndex_ (int dim) :
        _indexes (), _size (), _dimensions (dim)
{
    for (int i = 0; i < dim; i++)
    {
        Index_ idx;
        _indexes.push_back (idx);
    }
}

MDIndex_::MDIndex_ (int constant, int factor, int dim) :
        _indexes (), _size (), _dimensions (dim)
{
    Index_ idx (constant, factor);
    _indexes.push_back (idx);
    for (int i = 1; i < dim; i++)
    {
        Index_ id;
        _indexes.push_back (id);
    }
}

MDIndex_::MDIndex_ (int constant, int factor, int low, int high, int dim) :
        _indexes (), _size (), _dimensions (dim)
{
    Index_ idx (constant, factor, low, high);
    _indexes.push_back (idx);
    for (int i = 1; i < dim; i++)
    {
        Index_ id;
        _indexes.push_back (id);
    }
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
        ret = ret && _indexes[i].checkReverseValue (val);
    }
    return (ret);
}

void
MDIndex_::setLow (int l, int dim)
{
    _indexes[dim].setLow (l);
}

int
MDIndex_::low (int dim) const
{
    return (_indexes[dim].low ());
}

void
MDIndex_::setHi (int h, int dim)
{
    _indexes[dim].setHi (h);
}

int
MDIndex_::hi (int dim) const
{
    return (_indexes[dim].hi ());
}

void
MDIndex_::setConstant (int c, int dim)
{
    _indexes[dim].setConstant (c);
}

int
MDIndex_::mappedConstant (int dim) const
{
    return (_indexes[dim].mappedConstant ());
}

int
MDIndex_::operConstant (int dim) const
{
    return (_indexes[dim].operConstant ());
}

int
MDIndex_::constant (int dim) const
{
    return (_indexes[dim].constant ());
}

void
MDIndex_::setFactor (int f, int dim)
{
    _indexes[dim].setFactor (f);
}

int
MDIndex_::factor (int dim) const
{
    return (_indexes[dim].factor ());
}

void
MDIndex_::setOffset (int o)
{
    for (int i = 0; i < _dimensions; i++)
    {
        _indexes[i].setOffset (o);
    }
}

int
MDIndex_::offset () const
{
    return (_indexes[0].offset ());
}

int
MDIndex_::value (int val, int offset, int dim)
{
    return (_indexes[dim].value (val, offset));
}

MDIndex_
MDIndex_::indexValue (int val, int dim)
{
    MDIndex_ idx;
    Index_ id = _indexes[dim].indexValue (val);
    idx._indexes.push_back (id);
    idx._size.push_back (id.range ());
    return (idx);
}

int
MDIndex_::mappedValue (int val, int offset, int dim)
{
    return (_indexes[dim].mappedValue (val, offset));
}

int
MDIndex_::modelicaValue (int val, int dim)
{
    return (_indexes[dim].modelicaValue (val));
}

int
MDIndex_::reverseValue (int val, int dim)
{
    return (_indexes[dim].reverseValue (val));
}

int
MDIndex_::mappedBegin (int dim) const
{
    return (_indexes[dim].mappedBegin ());
}

int
MDIndex_::mappedEnd (int dim) const
{
    return (_indexes[dim].mappedEnd ());
}

int
MDIndex_::begin (int dim) const
{
    return (_indexes[dim].begin ());
}

int
MDIndex_::end (int dim) const
{
    return (_indexes[dim].end ());
}

int
MDIndex_::reverseBegin (int dim) const
{
    return (_indexes[dim].reverseBegin ());
}

int
MDIndex_::reverseEnd (int dim) const
{
    return (_indexes[dim].reverseEnd ());
}

string
MDIndex_::print (string sub, int offset, bool solver) const
{
    stringstream idxStr, ret;
    for (int i = 0; i < _dimensions; i++)
    {
        idxStr << sub;
        if (solver)
        {
            idxStr << i;
        }
        if (i > 0)
        {
            ret << " + ";
        }
        ret << _indexes[i].print (idxStr.str(), offset, solver);
        if (i < _dimensions - 1)
        {
            ret << " * " << pow(_size[i],(_dimensions - i - 1));
        }
        idxStr.str ("");
    }
    return (ret.str ());
}

string
MDIndex_::printReverse (string variable, int offset)
{
    stringstream idxStr, ret;
    for (int i = 0; i < _dimensions; i++)
    {
        idxStr << variable << i;
        if (i > 0)
        {
            ret << " + ";
        }
        ret << _indexes[i].printReverse(idxStr.str(), offset);
        idxStr.str("");
    }
    return (ret.str());
}

bool
MDIndex_::operator < (const MDIndex_& other) const
{
    for (int i = 0; i < _dimensions; i++)
    {
        if (_indexes[i] < other._indexes[i])
        {
            return (true);
        }
        else if (other._indexes[i] < _indexes[i])
        {
            return (false);
        }
    }
    return (false);
}

bool
MDIndex_::isSet (int dim) const
{
    for (int i = 0; i < _dimensions; i++)
    {
        if (_indexes[i].isSet ())
        {
            return (true);
        }
    }
    return (false);
}

bool
MDIndex_::hasRange () const
{
    for (int i = 0; i < _dimensions; i++)
    {
        if (_indexes[i].hasRange ())
        {
            return (true);
        }
    }
    return (false);
}

bool
MDIndex_::hasRange (int dim) const
{
    return (_indexes[dim].hasRange ());
}

void
MDIndex_::setRange (int dim)
{
    _indexes[dim].setRange ();
}

int
MDIndex_::range (int dim) const
{
    return (_indexes[dim].range ());
}

Intersection
MDIndex_::intersection (MDIndex_ b, int dim) const
{
    return (_indexes[dim].intersection (b._indexes[dim]));
}

int
MDIndex_::lowValue (int dim) const
{
    return (_indexes[dim].lowValue ());
}

int
MDIndex_::hiValue (int dim) const
{
    return (_indexes[dim].hiValue ());
}

void
MDIndex_::setArray ()
{
    for (int i = 0; i < _dimensions; i++)
    {
        _indexes[i].setArray ();
    }
}

bool
MDIndex_::isArray () const
{
    return (_indexes[0].isArray ());
}

void
MDIndex_::clear (int dim)
{
    for (int i = 0; i < _dimensions; i++)
    {
        _indexes[i].clear ();
    }
}

void
MDIndex_::setMap (MDIndex_ map, string variable, int dim)
{
    _indexes[dim].setMap (map._indexes[dim], variable);
}

bool
MDIndex_::hasMap (int dim) const
{
    for (int i = 0; i < _dimensions; i++)
    {
        if (_indexes[i].hasMap ())
        {
            return (true);
        }
    }
    return (false);
}

string
MDIndex_::parameter (int dim) const
{
    return (_indexes[dim].parameter ());
}

MDIndex_
MDIndex_::map (int dim) const
{
    MDIndex_ idx;
    idx._indexes.push_back (_indexes[dim].map ());
    idx._size.push_back (idx._indexes[0].range ());
    return (idx);
}

void
MDIndex_::setMap (MDIndex_ map, int dim)
{
    _indexes[dim].setMap (map._indexes[dim]);
}

bool
MDIndex_::hasRangeOp (int dim)
{
    return (_indexes[dim].hasRangeOp ());
}

void
MDIndex_::setRangeOp (bool rangeOp, int dim)
{
    _indexes[dim].setRangeOp (rangeOp);
}

string
MDIndex_::definition (string idx, int dim)
{
    return (_indexes[dim].definition (idx));
}

string
MDIndex_::variable (MDIndex_ index, string var, int dim)
{
    return (_indexes[dim].variable (index._indexes[dim], var));
}

MDIndex_
MDIndex_::variableIndex (MDIndex_ index, int dim)
{
    MDIndex_ idx;
    idx._indexes.push_back (_indexes[dim].variableIndex (index._indexes[dim]));
    idx._size.push_back (idx._indexes[0].range ());
    return (idx);
}

MDIndex_
MDIndex_::applyVariableChange (MDIndex_ index, int dim)
{
    MDIndex_ idx;
    idx._indexes.push_back (_indexes[dim].applyVariableChange (index._indexes[dim]));
    idx._size.push_back (idx._indexes[0].range ());
    return (idx);
}

bool
MDIndex_::variableChange (MDIndex_ index, int dim)
{
    return (_indexes[dim].variableChange (index._indexes[dim]));
}

string
MDIndex_::printReverseDefinition (string variable, int offset, int dim)
{
    return (_indexes[dim].printReverseDefinition (variable, offset));
}

bool
MDIndex_::operator == (const MDIndex_& other) const
{
    for (int i = 0; i < _dimensions; i++)
    {
        if (!(_indexes[i] == other._indexes[i]))
        {
            return (false);
        }
    }
    return (true);
}

bool
MDIndex_::equalExp (const MDIndex_& other) const
{
    for (int i = 0; i < _dimensions; i++)
    {
        if (!(_indexes[i].equalExp (other._indexes[i])))
        {
            return (false);
        }
    }
    return (true);
}

bool
MDIndex_::getIntersection (const MDIndex_& other) const
{
    for (int i = 0; i < _dimensions; i++)
    {
        if (!(_indexes[i].getIntersection (other._indexes[i])))
        {
            return (true);
        }
    }
    return (false);
}

bool
MDIndex_::odd (int dim) const
{
    return (_indexes[dim].odd ());
}

bool
MDIndex_::even (int dim) const
{
    return (_indexes[dim].even ());
}

void
MDIndex_::addIndex ()
{
    Index_ idx;
    _indexes.push_back (idx);
    _size.push_back (1);
    _dimensions++;
}

void
MDIndex_::addIndex (int constant, int factor)
{
    Index_ idx (constant, factor);
    _indexes.push_back (idx);
    _size.push_back (idx.range ());
    _dimensions++;
}

void
MDIndex_::setLow (vector<int> l)
{
    int size = l.size ();
    for (int i; i < size; i++)
    {
        _indexes[i].setLow(l[i]);
    }
}

void
MDIndex_::setHi (vector<int> h)
{
    int size = h.size ();
    for (int i; i < size; i++)
    {
        _indexes[i].setHi(h[i]);
    }
}

void
MDIndex_::addIndex (int constant, int factor, int low, int high)
{
    Index_ idx (constant, factor, low, high);
    _indexes.push_back (idx);
    _size.push_back (idx.range ());
    _dimensions++;
}

int
MDIndex_::dimension ()
{
    return (_dimensions);
}

void
MDIndex_::setDimension (int d)
{
    if (_dimensions < d)
    {
        int ub = d - _dimensions;
        for (int i = 0; i < ub; i++)
        {
            _indexes.push_back(Index_ ());
            _size.push_back(1);
        }
        _dimensions = d;
    }
}

void
MDIndex_::setIndex (MDIndex_ idx, int od, int size, int dd)
{
    _indexes[od] = idx._indexes[dd];
    _size[od] = size;
}

MDVariableInterval_::MDVariableInterval_ () : _index(), _name()
{
}

MDVariableInterval_::MDVariableInterval_ (MDIndex_ index, string name) : _index(index), _name(name)
{
}

MDVariableInterval_::~MDVariableInterval_ ()
{
}

MDIndex_
MDVariableInterval_::index ()
{
    return (_index);
}

string
MDVariableInterval_::name ()
{
    return (_name);
}

void
MDVariableInterval_::setIndex (MDIndex_ index)
{
    _index = index;
}

void
MDVariableInterval_::setName (string name)
{
    _name = name;
}

bool
MDVariableInterval_::isEmpty ()
{
    return (_name == "");
}
