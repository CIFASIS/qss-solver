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

#include "mmo_decl.h"

#include "mmo_visitor.h"

MMODecl::MMODecl () :
        _value (), _type (DEC_CONSTANT), _init ()
{
}

MMODecl::MMODecl (string id, double value, MMODeclType type)
{
    _id = id;
    _exp = "";
    _type = type;
    _value = value;
    _init = false;
}

MMODecl::MMODecl (string id, string exp, MMODeclType type, bool conditional)
{
    _id = id;
    _exp = exp;
    _type = type;
    if (type == DEC_ASSIGNMENT && conditional)
    {
        _type = DEC_CONDITIONAL_ASSIGNMENT;
    }
    _value = 0;
    _init = false;
}

MMODecl::MMODecl (string id, MMODeclType type)
{
    if (type == DEC_ZC_RELATION || type == DEC_CONDITION)
    {
        _id = "";
        _exp = id;
    }
    else
    {
        _id = id;
        _exp = "";
    }
    _type = type;
    _value = 0;
    _init = false;
}

MMODecl::~MMODecl ()
{
}

void
MMODecl::accept (MMOVisitor *visitor)
{
    visitor->visit (this);
    visitor->leave (this);
}

string
MMODecl::getId ()
{
    return (_id);
}

void
MMODecl::id (string i)
{
    _id = i;
}

void
MMODecl::setType (MMODeclType type)
{
    _type = type;
}

string
MMODecl::getExp ()
{
    return (_exp);
}

void
MMODecl::exp (string i)
{
    _exp = i;
}

void
MMODecl::value (double i)
{
    _value = i;
}

double
MMODecl::getValue ()
{
    return (_value);
}

bool
MMODecl::hasExp ()
{
    return (_exp != "");
}

bool
MMODecl::hasValue ()
{
    return (_exp == "");
}

bool
MMODecl::isAlgebraicEquation ()
{
    return (_type == DEC_ALGEBRAIC_EQUATION);
}

bool
MMODecl::isInitialAssignment ()
{
    return (_type == DEC_INITIAL_ASSIGNMENT);
}

bool
MMODecl::isAssignment ()
{
    return (_type == DEC_ASSIGNMENT);
}

bool
MMODecl::isZeroCrossing ()
{
    return (_type == DEC_ZC_RELATION);
}

bool
MMODecl::isOppositeZeroCrossing ()
{
    return (_type == DEC_ZC_OPPOSITE_RELATION);
}
bool
MMODecl::isDerivative ()
{
    return (_type == DEC_DERIVATIVE);
}

bool
MMODecl::isParameter ()
{
    return (_type == DEC_PARAMETER);
}

bool
MMODecl::isConstant ()
{
    return (_type == DEC_CONSTANT);
}

bool
MMODecl::isState ()
{
    return (_type == DEC_STATE);
}

bool
MMODecl::isDiscrete ()
{
    return (_type == DEC_DISCRETE);
}

bool
MMODecl::isAlgebraic ()
{
    return (_type == DEC_ALGEBRAIC);
}

bool
MMODecl::isCondition ()
{
    return (_type == DEC_CONDITION);
}

bool
MMODecl::isFunctionInput ()
{
    return (_type == DEC_FUNCTION_INPUT);
}

bool
MMODecl::isFunctionOutput ()
{
    return (_type == DEC_FUNCTION_OUTPUT);
}

bool
MMODecl::isFunctionDefinition ()
{
    return (_type == DEC_FUNCTION_DEFINITION);
}

bool
MMODecl::isFunctionFormula ()
{
    return (_type == DEC_FUNCTION_FORMULA);
}

bool
MMODecl::isReinit ()
{
    return (_type == DEC_REINIT);
}

bool
MMODecl::isImport ()
{
    return (_type == DEC_IMPORT);
}

bool
MMODecl::isImplicit ()
{
    return (_type == DEC_IMPLICIT_EQUATION);
}

bool
MMODecl::isConditionalAssignment ()
{
    return (_type == DEC_CONDITIONAL_ASSIGNMENT);
}
