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

VarInfo_::VarInfo_(Type t, AST_TypePrefix tp, AST_Modification m, AST_Comment c) :
    _state(false), _discrete(false), _t(t), _tp(tp), _m(m), _comm(c), _builtin(
        false), _index(), _size(), _value(0), _algebraic(
        false), _exp(
    NULL), _hasStart(false), _hasEach(false), _hasAssigment(false), _name(), _isArray(
        false)
{
  _processModification();
}
;

VarInfo_::VarInfo_(Type t, AST_TypePrefix tp, AST_Modification m, vector<int> s,
    bool array) :
    _state(false), _discrete(false), _t(t), _tp(tp), _m(m), _comm(NULL), _builtin(
        false), _index(), _size(s), _value(0), _algebraic(
        false), _exp(
    NULL), _hasStart(false), _hasEach(false), _hasAssigment(false), _name(), _isArray(
        array)
{
  _processModification();
}
;

/*! \brief Process the argument modification to determine the variable modifiers if any.
 *
 * 	\note{We don't look for errors here, if there's an error in the code, it should be detected in 
 * 	an earlier stage (the model checker and the intermediate code generation.}
 */

void
VarInfo_::_processModification()
{
  _hasAssigment = false;
  _hasEach = false;
  _hasStart = false;
  if(_m != NULL)
  {
    ModificationType t = _m->modificationType();
    if(t == MODEQUAL)
    {
      _hasAssigment = true;
      _exp = _m->getAsEqual()->exp();
    }
    else if(t == MODCLASS)
    {
      AST_ArgumentList al = _m->getAsClass()->arguments();
      AST_ArgumentListIterator it;
      AST_Expression asgExp = _m->getAsClass()->exp();
      if(asgExp->expressionType() != EXPNULL)
      {
        _hasAssigment = true;
        _exp = asgExp;
      }
      else
      {
        if(!isParameter())
        {
          foreach(it,al)
          {
            if(current_element(it)->argumentType() == AR_MODIFICATION)
            {
              AST_Argument_Modification am =
              current_element(it)->getAsModification();
              if(am->modification()->modificationType() == MODEQUAL)
              {
                _exp = am->modification()->getAsEqual()->exp();
                if(current_element(it)->hasEach())
                {
                  _hasEach = true;
                }
                else if(am->name()->compare("start") == 0)
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
VarInfo_::isState()
{
  return _state;
}

void
VarInfo_::setState()
{
  _state = true;
  _unsetAssignment();
}

void
VarInfo_::setDiscrete()
{
  _discrete = true;
  _unsetAssignment();
}

Type
VarInfo_::type()
{
  return _t;
}

bool
VarInfo_::isTime()
{
  return _name.compare("time") == 0;
}

bool
VarInfo_::isAlgebraic()
{
  return _algebraic;
}

void
VarInfo_::setAlgebraic()
{
  _algebraic = true;
  _unsetAssignment();
  _unsetStartEach();
}

bool
VarInfo_::hasIndex()
{
  return _index.isSet();
}

void
VarInfo_::setIndex(Index idx)
{
  _index = idx;
}

Index
VarInfo_::index()
{
  return _index;
}

void
VarInfo_::setValue(int val)
{
  _value = val;
}

int
VarInfo_::value()
{
  return _value;
}

int
VarInfo_::size()
{
  vector<int>::iterator it;
  int total = 1;
  for(it = _size.begin(); it != _size.end(); it++)
  {
    total *= *it;
  }
  return total;
}

bool
VarInfo_::hasAssignment()
{
  return _hasAssigment;
}

bool
VarInfo_::hasStartModifier()
{
  return _hasStart;
}

bool
VarInfo_::hasEachModifier()
{
  return _hasEach;
}

AST_Expression
VarInfo_::exp()
{
  return _exp;
}

ostream &
operator<<(ostream &ret, const VarInfo_ &e)
{
  if(e.isParameter())
    ret << "parameter ";
  if(e.isDiscrete())
    ret << "discrete ";
  if(e.isConstant())
    ret << "constant ";
  if(e.isInput())
    ret << "input ";
  if(e.isOutput())
    ret << "output ";
  ret << e._t;
  return ret;
}

bool
VarInfo_::isUnknown()
{
  return _unknown;
}

void
VarInfo_::setUnknown()
{
  _unknown = true;
}

string
VarInfo_::name()
{
  return _name;
}

void
VarInfo_::setName(string n)
{
  _name = n;
}

bool
VarInfo_::isArray()
{
  return _isArray;
}

VarInfo
newVarInfo(Type t, AST_TypePrefix tp, AST_Modification m, AST_Comment c,
    vector<int> s, bool array)
{
  if(s.empty())
  {
    return new VarInfo_(t, tp, m, c);
  }
  else
  {
    return new VarInfo_(t, tp, m, s, array);
  }
}

/* TypeSymbolTable_ class.*/

TypeSymbolTable_::TypeSymbolTable_()
{
  insert("String", new Type_String_());
  insert("Real", new Type_Real_());
  insert("Integer", new Type_Integer_());
  insert("Boolean", new Type_Boolean_());
}

TypeSymbolTable
newTypeSymbolTable()
{
  return new TypeSymbolTable_;
}

/* VarSymbolTable_ class */

VarSymbolTable_::VarSymbolTable_() :
    _vste(), _coeffs(1), _parameters()
{
}

void
VarSymbolTable_::initialize(TypeSymbolTable ty)
{
  VarInfo v = newVarInfo(ty->lookup("Real"), 0, NULL, NULL, vector<int>(1, 0),
      false);
  v->setBuiltIn();
  v->setName("time");
  insert("time", v);
}

VST_Environment
VarSymbolTable_::printEnvironment()
{
  return _vste;
}

void
VarSymbolTable_::setPrintEnvironment(VST_Environment vse)
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
VarSymbolTable_::print(VarInfo vi, string idx, int offset, int order,
    list<Index> idxs, int constant, int forOffset)
{
  stringstream ret;
  string idxStr;
  if(vi->isForType())
  {
    int fo = offset + forOffset;
    if(fo != 0)
    {
      ret << "(";
    }
    // TODO: Remove this when the indexes are printed using ranges.
    ret << idx << "0"; // Temporal hack source
    if(fo > 0)
    {
      ret << "+" << fo << ")";
    }
    else if(fo < 0)
    {
      ret << fo << ")";
    }
    return ret.str();
  }
  if(vi->hasEachModifier() && (_vste == VST_INIT || _vste == VST_CLASSIC_INIT))
  {
    stringstream tmp;
    tmp << "[" << idx;
    if(!vi->isParameter())
    {
      int cte = vi->index().mappedConstant();
      if(cte)
      {
        tmp.str("");
        tmp << "[(" << idx << "+" << cte << ")";
      }
    }
    if(vi->isState() || vi->isAlgebraic())
    {
      tmp << " * " << _coeffs;
    }
    tmp << "]";
    idxStr = tmp.str();
  }
  else if(!vi->isConstant() && !vi->isParameter())
  {
    idxStr = printIndex(vi->index(), idxs, idx, offset, constant, vi, order);
  }
  else if(vi->isParameter() && !idxs.empty())
  {
    idxStr = printIndex(Index(), idxs, idx, offset, constant, vi, order);
  }
  switch(_vste)
  {
    case VST_CLASSIC_INIT:
      if(vi->isState())
      {
        ret << "modelData->x" << idxStr;
      }
      else if(vi->isDiscrete())
      {
        ret << "modelData->d" << idxStr;
      }
      else if(vi->isAlgebraic())
      {
        ret << "modelData->alg" << idxStr;
      }
      else if(vi->isParameter())
      {
        ret << "__PAR_";
        ret << vi->name();
        ret << idxStr;
      }
      else if(vi->isConstant())
      {
        ret << vi->value();
      }
      break;
    case VST_INIT:
      if(vi->isState())
      {
        ret << "modelData->x" << idxStr;
      }
      else if(vi->isDiscrete())
      {
        ret << "modelData->d" << idxStr;
      }
      else if(vi->isAlgebraic())
      {
        ret << "modelData->alg" << idxStr;
      }
      else if(vi->isParameter())
      {
        ret << "__PAR_";
        ret << vi->name();
        ret << idxStr;
      }
      else if(vi->isConstant())
      {
        ret << vi->value();
      }
      break;
    case VST_MODEL_FUNCTIONS:
    case VST_MODEL_OUTPUT_FUNCTIONS:
      if(vi->isState())
      {
        ret << "x" << idxStr;
        if(order == 2)
        {
          ret << "*2";
        }
        else if(order == 3)
        {
          ret << "*6";
        }
      }
      else if(vi->isDiscrete())
      {
        ret << "d" << idxStr;
      }
      else if(vi->isAlgebraic() && _vste)
      {
        ret << "alg" << idxStr;
      }
      else if(vi->isParameter())
      {
        ret << "__PAR_";
        ret << vi->name();
        ret << idxStr;
      }
      else if(vi->isConstant())
      {
        ret << vi->value();
      }
      break;
    case VST_CLASSIC_MODEL_FUNCTIONS:
      if(vi->isState())
      {
        ret << "x" << idxStr;
      }
      else if(vi->isDiscrete())
      {
        ret << "d" << idxStr;
      }
      else if(vi->isAlgebraic() && _vste)
      {
        ret << "alg" << idxStr;
      }
      else if(vi->isParameter())
      {
        ret << "__PAR_";
        ret << vi->name();
        ret << idxStr;
      }
      else if(vi->isConstant())
      {
        ret << vi->value();
      }
      break;
    case VST_FUNCTION:
      if(vi->isConstant())
      {
        ret << vi->value();
      }
      else
      {
        if(vi->isOutput())
        {
          ret << "*";
        }
        ret << vi->name();
        if(vi->isArray())
        {
          ret << idxStr;
        }
      }
      break;
    case VST_OUTPUT:
      ret << vi->name();
      if(vi->isArray())
      {
        ret << idxStr;
      }
      break;
  }
  if(vi->builtIn())
  {
    switch(Util::getInstance()->checkBuiltInVariables(vi->name()))
    {
      case BIV_TIME:
        ret << "t";
        break;
      case BIV_SUM:
      case BIV_PRODUCT:
      case BIV_MIN:
      case BIV_MAX:
      case BIV_INNER_PRODUCT:
        ret << vi->name() << "[" << order << "]";
        break;
      default:
        break;
    }
  }
  return ret.str();
}

string
VarSymbolTable_::printIndex(Index idx, list<Index> idxs, string sub, int offset,
    int constant, VarInfo vi, int order)
{
  stringstream buffer;
  stringstream varOffset;
  bool printOffset = false;
  if(vi->isState() || vi->isAlgebraic())
  {
    varOffset << " * " << _coeffs;
    if(order > 0)
    {
      varOffset << " + " << order;
    }
    printOffset = true;
  }
  if(idxs.empty())
  {
    if(constant >= 0 && idx.isArray())
    {
      if(printOffset)
      {
        buffer << "[" << idx.mappedValue(constant) * _coeffs + order << "]";
      }
      else
      {
        buffer << "[" << idx.mappedValue(constant) << "]";
      }
    }
    else
    {
      if(idx.factor() == 0 && (vi->isState() || vi->isAlgebraic()))
      {
        buffer << "[" << idx.mappedConstant() * _coeffs + order << "]";
      }
      else
      {
        buffer << "[(" << idx.print(sub, offset) << ")" << varOffset.str()
            << "]";
      }
    }
  }
  else
  {
    buffer << "[";
    int idxIt = 0;
    stringstream s;
    for(list<Index>::iterator it = idxs.begin(); it != idxs.end(); it++)
    {
      if(idxIt > 0)
      {
        buffer << " + ";
      }
      s << sub;
      // TODO: Remove index string formation from here.
      if(sub.find("%") == string::npos)
      {
        s << idxIt;
      }
      if(_vste == VST_OUTPUT)
      {
        if(constant >= 0 && it->isArray())
        {
          buffer << it->mappedValue(constant);
        }
        else if(sub.empty())
        {
          buffer << it->operConstant();
        }
        else
        {
          buffer << s.str();
        }
      }
      else
      {
        if(constant >= 0)
        {
          if(printOffset)
          {
            buffer << it->mappedValue(constant) * _coeffs + order;
          }
          else
          {
            buffer << it->mappedValue(constant);
          }
        }
        else
        {
          if(it->factor() == 0 && (vi->isState() || vi->isAlgebraic()))
          {
            buffer << it->mappedConstant() * _coeffs + order;
          }
          else
          {
            buffer << "(" << it->print(s.str(), offset, false, true) << ")"
                << varOffset.str();
          }
        }
      }
      if(idxIt < (vi->dimensions() - 1))
      {
        buffer << " * " << pow(vi->size(idxIt), vi->dimensions() - idxIt - 1);
      }
      idxIt++;
      s.str("");
    }
    buffer << "]";
  }
  return buffer.str();
}

VarSymbolTable
newVarSymbolTable()
{
  return new VarSymbolTable_;
}

void
VarInfo_::setEachModifier(bool each)
{
  _hasEach = each;
}

void
VarInfo_::_unsetAssignment()
{
  _hasAssigment = false;
}

int
VarInfo_::size(int dim)
{
  return _size[dim];
}

int
VarInfo_::dimensions()
{
  return _size.size();
}

void
VarInfo_::_unsetStartEach()
{
  _hasEach = false;
  _hasStart = false;
}

void
VarInfo_::setParameter()
{
  _tp = TP_PARAMETER;
  _unsetStartEach();
}

void
VarSymbolTable_::setPolyCoeffs(int coeffs)
{
  _coeffs = coeffs;
}

void
VarSymbolTable_::insert(VarName n, VarInfo vi)
{
  SymbolTable<VarName, VarInfo>::insert(n, vi);
  if(vi->isParameter())
  {
    _parameters.push_back(vi);
  }
}

list<VarInfo>
VarSymbolTable_::parameters()
{
  return _parameters;
}

string
VarSymbolTable_::getTypePrefix()
{
  if(_vste == VST_CLASSIC_INIT || _vste == VST_INIT)
  {
    return "modelData->";
  }
  return "";
}
