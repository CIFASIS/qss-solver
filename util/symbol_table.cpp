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

#include "symbol_table.h"

#include <sstream>
#include <math.h>

#include "../ast/expression.h"
#include "../ir/equation.h"
#include "../ir/expression.h"
#include "dependencies.h"
#include "util.h"

/* VarInfo class. */

VarInfo_::VarInfo_ (Type t, AST_TypePrefix tp, AST_Modification m, AST_Comment c) :
        _state (false), _discrete (false), _t (t), _tp (tp), _m (m), _comm (c), _builtin (false), _index (), _size (), _value (0), _algebraic (
                false), _exp (
        NULL), _hasStart (false), _hasEach (false), _hasAssigment (false), _name (), _isArray (false)
{
    _processModification ();
}
;

VarInfo_::VarInfo_ (Type t, AST_TypePrefix tp, AST_Modification m, vector<int> s, bool array) :
        _state (false), _discrete (false), _t (t), _tp (tp), _m (m), _comm (NULL), _builtin (false), _index (), _size (s), _value (0), _algebraic (
                false), _exp (
        NULL), _hasStart (false), _hasEach (false), _hasAssigment (false), _name (), _isArray (array)
{
    _processModification ();
}
;

/*! \brief Process the argument modification to determine the variable modifiers if any.
 *
 * 	\note{We don't look for errors here, if there's an error in the code, it should be detected in 
 * 	an earlier stage (the model checker and the intermediate code generation.}
 */

void
VarInfo_::_processModification ()
{
    _hasAssigment = false;
    _hasEach = false;
    _hasStart = false;
    if (_m != NULL)
    {
        ModificationType t = _m->modificationType ();
        if (t == MODEQUAL)
        {
            _hasAssigment = true;
            _exp = _m->getAsEqual ()->exp ();
        }
        else if (t == MODCLASS)
        {
            AST_ArgumentList al = _m->getAsClass ()->arguments ();
            AST_ArgumentListIterator it;
            AST_Expression asgExp = _m->getAsClass ()->exp ();
            if (asgExp->expressionType () != EXPNULL)
            {
                _hasAssigment = true;
                _exp = asgExp;
            }
            else
            {
                if (!isParameter ())
                {
                    foreach(it,al)
                    {
                        if (current_element(it)->argumentType () == AR_MODIFICATION)
                        {
                            AST_Argument_Modification am =
                            current_element(it)->getAsModification ();
                            if (am->modification ()->modificationType () == MODEQUAL)
                            {
                                _exp = am->modification ()->getAsEqual ()->exp ();
                                if (current_element(it)->hasEach ())
                                {
                                    _hasEach = true;
                                }
                                else if (am->name ()->compare ("start") == 0)
                                {
                                    _hasStart = true;
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

bool
VarInfo_::isState ()
{
    return (_state);
}

void
VarInfo_::setState ()
{
    _state = true;
    _unsetAssignment ();
}

void
VarInfo_::setDiscrete ()
{
    _discrete = true;
    _unsetAssignment ();
}

Type
VarInfo_::type ()
{
    return (_t);
}

bool
VarInfo_::isTime ()
{
    return (_name.compare ("time") == 0);
}

bool
VarInfo_::isAlgebraic ()
{
    return (_algebraic);
}

void
VarInfo_::setAlgebraic ()
{
    _algebraic = true;
    _unsetAssignment ();
    _unsetStartEach ();
}

bool
VarInfo_::hasIndex ()
{
    return (_index.isSet ());
}

void
VarInfo_::setIndex (Index idx)
{
    _index = idx;
}

Index
VarInfo_::index ()
{
    return (_index);
}

void
VarInfo_::setValue (int val)
{
    _value = val;
}

int
VarInfo_::value ()
{
    return (_value);
}

int
VarInfo_::size ()
{
    vector<int>::iterator it;
    int total = 1;
    for (it = _size.begin(); it != _size.end(); it++)
    {
        total *= *it;
    }
    return (total);
}

bool
VarInfo_::hasAssignment ()
{
    return (_hasAssigment);
}

bool
VarInfo_::hasStartModifier ()
{
    return (_hasStart);
}

bool
VarInfo_::hasEachModifier ()
{
    return (_hasEach);
}

AST_Expression
VarInfo_::exp ()
{
    return (_exp);
}

ostream &
operator<< (ostream &ret, const VarInfo_ &e)
{
    if (e.isParameter ())
        ret << "parameter ";
    if (e.isDiscrete ())
        ret << "discrete ";
    if (e.isConstant ())
        ret << "constant ";
    if (e.isInput ())
        ret << "input ";
    if (e.isOutput ())
        ret << "output ";
    ret << e._t;
    return (ret);
}

bool
VarInfo_::isUnknown ()
{
    return (_unknown);
}

void
VarInfo_::setUnknown ()
{
    _unknown = true;
}

string
VarInfo_::name ()
{
    return (_name);
}

void
VarInfo_::setName (string n)
{
    _name = n;
}

bool
VarInfo_::isArray ()
{
    return (_isArray);
}

VarInfo
newVarInfo (Type t, AST_TypePrefix tp, AST_Modification m, AST_Comment c, vector<int> s, bool array)
{
    if (s.empty ())
    {
        return (new VarInfo_ (t, tp, m, c));
    }
    else
    {
        return (new VarInfo_ (t, tp, m, s, array));
    }
}

/* TypeSymbolTable_ class.*/

TypeSymbolTable_::TypeSymbolTable_ ()
{
    insert ("String", new Type_String_ ());
    insert ("Real", new Type_Real_ ());
    insert ("Integer", new Type_Integer_ ());
    insert ("Boolean", new Type_Boolean_ ());
}

TypeSymbolTable
newTypeSymbolTable ()
{
    return (new TypeSymbolTable_);
}

/* VarSymbolTable_ class */

VarSymbolTable_::VarSymbolTable_ () :
        _vste (), _coeffs (1), _parameters ()
{
}

void
VarSymbolTable_::initialize (TypeSymbolTable ty)
{
    VarInfo v = newVarInfo (ty->lookup ("Real"), 0, NULL, NULL, vector<int> (1,0), false);
    v->setBuiltIn ();
    v->setName ("time");
    insert ("time", v);
}

VST_Environment
VarSymbolTable_::printEnvironment ()
{
    return (_vste);
}

void
VarSymbolTable_::setPrintEnvironment (VST_Environment vse)
{
    _vste = vse;
}

/*! \brief Print the symbol name and the corresponding index.
 *
 * 	\param vi: Variable info to be printed.
 * 	\param idx: String that that represents the index variable, if needed.
 * 	\param offset: Offset used to print the index, if needed. 
 * 	\param order: order used in the state and algebraic variables arrays.
 *	\return String with the proper variable representation.
 *
 * 	Description:
 * 		Prints the variable name according to the environment provided:
 * 		-# VST_INIT: returns _localData->x[idx][0] or _localData->d[idx], for the parameters return the parameter name prefixed with an underscore.
 * 		-# VST_MODEL_FUNCTIONS: _x[idx][order] or _d[0], for the algebraics variables always return _algvars[idx][order], parameters are printed in the same way as before.  
 * 		-# VST_FUNCTION: returns the variable name as defined in the model.
 *		-# The constants are printed since they are replaced everywhere in the model.
 *
 * 	\note{Algebraics variables are only printed in models}
 */

string
VarSymbolTable_::print (VarInfo vi, string idx, int offset, int order, list<Index> idxs, int constant, int forOffset)
{
    stringstream ret;
    string idxStr;
    if (vi->isForType ())
    {
        int fo = offset + forOffset;
        if (fo != 0)
        {
            ret << "(";
        }
        ret << idx << "0"; // Temporal hack source
        if (fo > 0)
        {
            ret << "+" << fo << ")";
        }
        else if (fo < 0)
        {
            ret << fo << ")";
        }
        return (ret.str ());
    }
    if (vi->hasEachModifier () && (_vste == VST_INIT || _vste == VST_CLASSIC_INIT))
    {
        stringstream tmp;
        tmp << "[" << idx;
        if (!vi->isParameter ())
        {
            int cte = vi->index ().mappedConstant ();
            if (cte)
            {
                tmp.str ("");
                tmp << "[(" << idx << "+" << cte << ")";
            }
        }
        if (vi->isState () || vi->isAlgebraic ())
        {
            tmp << " * " << _coeffs;
        }
        tmp << "]";
        idxStr = tmp.str ();
    }
    else if (!vi->isConstant () && !vi->isParameter ())
    {
        idxStr = printIndex (vi->index (), idxs, idx, offset, constant, vi, order);
    }
    else if (vi->isParameter () && !idxs.empty ())
    {
        idxStr = printIndex (Index (), idxs, idx, offset, constant, vi, order);
    }
    switch (_vste)
    {
        case VST_CLASSIC_INIT:
            if (vi->isState ())
            {
                ret << "modelData->x" << idxStr;
            }
            else if (vi->isDiscrete ())
            {
                ret << "modelData->d" << idxStr;
            }
            else if (vi->isAlgebraic ())
            {
                ret << "modelData->alg" << idxStr;
            }
            else if (vi->isParameter ())
            {
                ret << "__PAR_";
                ret << vi->name ();
                ret << idxStr;
            }
            else if (vi->isConstant ())
            {
                ret << vi->value ();
            }
            break;
        case VST_INIT:
            if (vi->isState ())
            {
                ret << "modelData->x" << idxStr;
            }
            else if (vi->isDiscrete ())
            {
                ret << "modelData->d" << idxStr;
            }
            else if (vi->isAlgebraic ())
            {
                ret << "modelData->alg" << idxStr;
            }
            else if (vi->isParameter ())
            {
                ret << "__PAR_";
                ret << vi->name ();
                ret << idxStr;
            }
            else if (vi->isConstant ())
            {
                ret << vi->value ();
            }
            break;
        case VST_MODEL_FUNCTIONS:
        case VST_MODEL_OUTPUT_FUNCTIONS:
            if (vi->isState ())
            {
                ret << "x" << idxStr;
                if (order == 2)
                {
                    ret << "*2";
                }
                else if (order == 3)
                {
                    ret << "*6";
                }
            }
            else if (vi->isDiscrete ())
            {
                ret << "d" << idxStr;
            }
            else if (vi->isAlgebraic () && _vste)
            {
                ret << "alg" << idxStr;
            }
            else if (vi->isParameter ())
            {
                ret << "__PAR_";
                ret << vi->name ();
                ret << idxStr;
            }
            else if (vi->isConstant ())
            {
                ret << vi->value ();
            }
            break;
        case VST_CLASSIC_MODEL_FUNCTIONS:
            if (vi->isState ())
            {
                ret << "x" << idxStr;
            }
            else if (vi->isDiscrete ())
            {
                ret << "d" << idxStr;
            }
            else if (vi->isAlgebraic () && _vste)
            {
                ret << "alg" << idxStr;
            }
            else if (vi->isParameter ())
            {
                ret << "__PAR_";
                ret << vi->name ();
                ret << idxStr;
            }
            else if (vi->isConstant ())
            {
                ret << vi->value ();
            }
            break;
        case VST_FUNCTION:
            if (vi->isConstant ())
            {
                ret << vi->value ();
            }
            else
            {
                ret << vi->name ();
                if (vi->isArray ())
                {
                    ret << idxStr;
                }
            }
            break;
        case VST_OUTPUT:
            ret << vi->name ();
            if (vi->isArray ())
            {
                ret << idxStr;
            }
            break;
    }
    if (vi->builtIn ())
    {
        switch (Util::getInstance ()->checkBuiltInVariables (vi->name ()))
        {
            case BIV_TIME:
                ret << "t";
                break;
            case BIV_SUM:
            case BIV_PRODUCT:
            case BIV_MIN:
            case BIV_MAX:
            case BIV_INNER_PRODUCT:
                ret << vi->name () << "[" << order << "]";
                break;
            default:
                break;
        }
    }
    return (ret.str ());
}

string
VarSymbolTable_::printIndex (Index idx, list<Index> idxs, string sub, int offset, int constant, VarInfo vi, int order)
{
    stringstream buffer;
    stringstream varOffset;
    bool printOffset = false;
    if (vi->isState () || vi->isAlgebraic ())
    {
        varOffset << " * " << _coeffs;
        if (order > 0)
        {
            varOffset << " + " << order;
        }
        printOffset = true;
    }
    if (idxs.empty ())
    {
        if (constant >= 0 && idx.isArray ())
        {
            if (printOffset)
            {
                buffer << "[" << idx.mappedValue (constant) * _coeffs + order << "]";
            }
            else
            {
                buffer << "[" << idx.mappedValue (constant) << "]";
            }
        }
        else
        {
            if (idx.factor () == 0 && (vi->isState () || vi->isAlgebraic ()))
            {
                buffer << "[" << idx.mappedConstant () * _coeffs + order << "]";
            }
            else
            {
                buffer << "[(" << idx.print (sub, offset) << ")" << varOffset.str () << "]";
            }
        }
    }
    else
    {
        buffer << "[";
        int idxIt = 0;
        stringstream s;
        for (list<Index>::iterator it = idxs.begin (); it != idxs.end (); it++)
        {
            if (idxIt > 0)
            {
                buffer << " + ";
            }
            s << sub << idxIt;
            if (_vste == VST_OUTPUT)
            {
                if (constant >= 0 && it->isArray ())
                {
                    buffer << it->mappedValue (constant);
                }
                else if (sub.empty ())
                {
                    buffer << it->operConstant ();
                }
                else
                {
                    buffer << s.str ();
                }
            }
            else
            {
                if (constant >= 0)
                {
                    if (printOffset)
                    {
                        buffer << it->mappedValue (constant) * _coeffs + order;
                    }
                    else
                    {
                        buffer << it->mappedValue (constant);
                    }
                }
                else
                {
                    if (it->factor () == 0 && (vi->isState () || vi->isAlgebraic ()))
                    {
                        buffer << it->mappedConstant () * _coeffs + order;
                    }
                    else
                    {
                        buffer << "(" << it->print (s.str (), offset, false, true) << ")" << varOffset.str ();
                    }
                }
            }
            if (idxIt < (vi->dimensions () - 1))
            {
                buffer << " * " << pow (vi->size (idxIt), vi->dimensions() - idxIt - 1);
            }
            idxIt++;
            s.str ("");
        }
        buffer << "]";
    }
    return (buffer.str ());
}

VarSymbolTable
newVarSymbolTable ()
{
    return (new VarSymbolTable_);
}

/* MMO_EquationTable_ class */

MMO_EquationTable_::MMO_EquationTable_ ()
{
}

list<MMO_Equation>
MMO_EquationTable_::equation (Index index)
{
    list<MMO_Equation> ret;
    MMO_EquationTable eqs = newMMO_EquationTable ();
    MMO_Equation eq = _equation (index);
    if (eq != NULL)
    {
        ret.push_back (eq);
        return (ret);
    }
    else if (eq == NULL && !index.isArray ())
    {
        return (ret);
    }
    else if (eq == NULL)
    {
        for (int j = index.low (); j <= index.hi (); j++)
        {
            Index simAlg = index.indexValue (j);
            MMO_Equation simEq = _equation (simAlg);
            if (simEq != NULL)
            {
                eqs->insert (simEq->lhs (), simEq);
            }
        }
    }
    for (MMO_Equation it = eqs->begin (); !eqs->end (); it = eqs->next ())
    {
        ret.push_back (it);
    }
    deleteMMO_EquationTable (eqs);
    return (ret);
}

Index
MMO_EquationTable_::equationIndex (Index index)
{
    int s = SymbolTable<Index, MMO_Equation>::count ();
    for (int i = 0; i < s; i++)
    {
        MMO_Equation eq = SymbolTable<Index, MMO_Equation>::val (i);
        if (eq->lhs ().hasRange ())
        {
            if (eq->lhs ().intersection (index).type () != IDX_DISJOINT)
            {
                return (SymbolTable<Index, MMO_Equation>::key (i));
            }
        }
    }
    return (Index ());
}

bool
MMO_EquationTable_::equations (Index index)
{
    int s = SymbolTable<Index, MMO_Equation>::count ();
    int count = 0;
    for (int i = 0; i < s; i++)
    {
        MMO_Equation eq = SymbolTable<Index, MMO_Equation>::val (i);
        if (eq->lhs ().offset () == index.offset ())
        {
            count++;
            if (count > 1)
            {
                return (true);
            }
        }
    }
    return (count > 1);
}

bool
MMO_EquationTable_::findGenericDependencies (int val)
{
    int s = IndexTable<MMO_Equation>::count ();
    for (int i = 0; i < s; i++)
    {
        Index idx = IndexTable<MMO_Equation>::key (i);
        if (idx.factor () != 0)
        {
            Dependencies d = IndexTable<MMO_Equation>::val (i)->exp ()->deps ();
            for (Index *dIdx = d->begin (DEP_STATE); !d->end (DEP_STATE); dIdx = d->next (DEP_STATE))
            {
                if (dIdx->factor () != 0)
                {
                    if (dIdx->checkReverseValue (val))
                    {
                        return (true);
                    }
                }
                else
                {
                    if (dIdx->mappedConstant () == val)
                    {
                        return (true);
                    }
                }
            }
            for (d->begin (DEP_ALGEBRAIC_STATE); !d->end (DEP_ALGEBRAIC_STATE); d->next (DEP_ALGEBRAIC_STATE))
            {
                Index state = d->key (DEP_ALGEBRAIC_STATE);
                if (state.factor () != 0)
                {
                    if (state.checkReverseValue (val))
                    {
                        return (true);
                    }
                }
                if (state.mappedConstant () == val)
                {
                    return (true);
                }
            }
        }
    }
    return (false);
}

MMO_EquationTable
newMMO_EquationTable ()
{
    return (new MMO_EquationTable_ ());
}

void
deleteMMO_EquationTable (MMO_EquationTable m)
{
    delete m;
}

/* MMO_PackageTable_ class */

MMO_PackageTable_::MMO_PackageTable_ ()
{
}

string
MMO_PackageTable_::functionPrefix (string name)
{
    for (MMO_PackageData pd = ListTable_<MMO_PackageData>::begin (); !ListTable_<MMO_PackageData>::end (); pd = ListTable_<MMO_PackageData>::next ())
    {
        MMO_FunctionDefinitionTable fdt = pd->functions ();
        for (MMO_FunctionDefinition fd = fdt->begin (); !fdt->end (); fd = fdt->next ())
        {
            string qualifiedName = pd->prefix () + name;
            if (fd->name ().compare (name) == 0 || fd->name ().compare (qualifiedName) == 0)
            {
                return (pd->prefix ());
            }
        }
    }
    return ("");
}

MMO_PackageTable
newMMO_PackageTable ()
{
    return (new MMO_PackageTable_ ());
}

void
deleteMMO_PackageTable (MMO_PackageTable m)
{
    delete m;
}

/* MMO_FunctionTable_ class */

MMO_FunctionTable_::MMO_FunctionTable_ ()
{
}

MMO_FunctionTable
newMMO_FunctionTable ()
{
    return (new MMO_FunctionTable_ ());
}

void
deleteMMO_FunctionTable (MMO_FunctionTable m)
{
    delete m;
}

/* MMO_EventTable_ class */

MMO_EventTable_::MMO_EventTable_ ()
{
}

MMO_EventTable
newMMO_EventTable ()
{
    return (new MMO_EventTable_ ());
}

void
deleteMMO_EventTable (MMO_EventTable m)
{
    delete m;
}

/* MMO_DependenciesTable_ class */

MMO_DependenciesTable_::MMO_DependenciesTable_ ()
{
}

bool
MMO_DependenciesTable_::find (int val)
{
    int s = IndexTable<Dependencies>::count ();
    for (int i = 0; i < s; i++)
    {
        Index idx = IndexTable<Dependencies>::key (i);
        if (idx.factor () != 0)
        {
            Dependencies d = IndexTable<Dependencies>::val (i);
            for (Index *dIdx = d->begin (DEP_STATE); !d->end (DEP_STATE); dIdx = d->next (DEP_STATE))
            {
                if (dIdx->factor () != 0)
                {
                    if (dIdx->checkReverseValue (val))
                    {
                        return (true);
                    }
                }
            }
        }
    }
    return (false);
}

MMO_DependenciesTable
newMMO_DependenciesTable ()
{
    return (new MMO_DependenciesTable_ ());
}

void
deleteMMO_DependenciesTable (MMO_DependenciesTable m)
{
    delete m;
}

/* MMO_ArgumentTable_ class */

MMO_ArgumentsTable_::MMO_ArgumentsTable_ ()
{
}

MMO_ArgumentsTable
newMMO_ArgumentsTable ()
{
    return (new MMO_ArgumentsTable_ ());
}

void
deleteMMO_ArgumentsTable (MMO_ArgumentsTable m)
{
    delete m;
}

/* MMO_StatementTable class. */

MMO_StatementTable_::MMO_StatementTable_ ()
{
}

MMO_StatementTable_::~MMO_StatementTable_ ()
{
}

MMO_StatementTable
newMMO_StatementTable ()
{
    return (new MMO_StatementTable_ ());
}

void
deleteMMO_StatementTable (MMO_StatementTable m)
{
    delete m;
}

/* MMO_ImportTable class. */

MMO_ImportTable_::MMO_ImportTable_ ()
{
}

MMO_ImportTable_::~MMO_ImportTable_ ()
{
}

MMO_ImportTable
newMMO_ImportTable ()
{
    return (new MMO_ImportTable_ ());
}

void
deleteMMO_ImportTable (MMO_ImportTable m)
{
    delete m;
}

/* MMO_FunctionDefinitionTable class. */

MMO_FunctionDefinitionTable_::MMO_FunctionDefinitionTable_ ()
{
}

MMO_FunctionDefinitionTable_::~MMO_FunctionDefinitionTable_ ()
{
}

MMO_FunctionDefinitionTable
newMMO_FunctionDefinitionTable ()
{
    return (new MMO_FunctionDefinitionTable_ ());
}

void
deleteMMO_FunctionDefinitionTable (MMO_FunctionDefinitionTable m)
{
    delete m;
}

/* MMO_IndexTable class.*/

MMO_IndexTable_::MMO_IndexTable_ ()
{
}

MMO_IndexTable
newMMO_IndexTable ()
{
    return (new MMO_IndexTable_ ());
}

void
deleteMMO_IndexTable (MMO_IndexTable m)
{
    delete m;
}

/* MMO_SymbolRefTable class. */

MMO_SymbolRefTable_::MMO_SymbolRefTable_ ()
{
}

MMO_SymbolRefTable_::~MMO_SymbolRefTable_ ()
{
}

MMO_SymbolRefTable
newMMO_SymbolRefTable ()
{
    return (new MMO_SymbolRefTable_ ());
}

void
deleteMMO_SymbolRefTable (MMO_SymbolRefTable m)
{
    delete m;
}

MMO_Equation
MMO_EquationTable_::_equation (Index index)
{
    int s = SymbolTable<Index, MMO_Equation>::count ();
    for (int i = 0; i < s; i++)
    {
        MMO_Equation eq = SymbolTable<Index, MMO_Equation>::val (i);
        if (eq->lhs ().intersection (index).type () != IDX_DISJOINT && eq->lhs ().range () >= index.range ())
        {
            return (eq);
        }
    }
    return (NULL);
}

void
VarInfo_::setEachModifier (bool each)
{
    _hasEach = each;
}

void
VarInfo_::_unsetAssignment ()
{
    _hasAssigment = false;
}

int
VarInfo_::size (int dim)
{
    return (_size[dim]);
}

int
VarInfo_::dimensions ()
{
    return (_size.size());
}

void
VarInfo_::_unsetStartEach ()
{
    _hasEach = false;
    _hasStart = false;
}

void
VarInfo_::setParameter ()
{
    _tp = TP_PARAMETER;
    _unsetStartEach ();
}

void
VarSymbolTable_::setPolyCoeffs (int coeffs)
{
    _coeffs = coeffs;
}

void
VarSymbolTable_::insert (VarName n, VarInfo vi)
{
    SymbolTable<VarName, VarInfo>::insert (n, vi);
    if (vi->isParameter ())
    {
        _parameters.push_back (vi);
    }
}

list<VarInfo>
VarSymbolTable_::parameters ()
{
    return (_parameters);
}

string
VarSymbolTable_::getTypePrefix ()
{
    if (_vste == VST_CLASSIC_INIT || _vste == VST_INIT)
    {
        return ("modelData->");
    }
    return ("");
}
