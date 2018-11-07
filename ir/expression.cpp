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

#include "../ast/expression.h"
#include "expression.h"

#include <iostream>
#include <sstream>
#include <utility>

#include "../ast/ast_builder.h"
#include "../util/error.h"
#include "../util/md_index.h"
#include "../util/symbol_table.h"
#include "../util/type.h"
#include "../util/util.h"
#include "annotation.h"
#include "equation.h"
#include "mmo_util.h"

/* MMO_Expression class. */

MMO_Expression_::MMO_Expression_(AST_Expression exp, MMO_ModelData data) :
    _exp(NULL), _data(data), _str(), _equationIndex(0), _expressionOrder(2), _deps(
    NULL), _gen(NULL), _printer(NULL), _ri(NULL)
{
  _ri = newMMO_ReplaceInterval(_data->symbols());
  if(_data->hasAnnotation())
  {
    _expressionOrder = _data->annotation()->polyCoeffs();
  }
  AST_Expression e = _ri->foldTraverse(exp);
  _exp = e;
  _gen = newGenerateDeps(_data);
  _deps = _gen->foldTraverse(exp);
  for(VariableInterval vi = _ri->first(); !_ri->end(); vi = _ri->next())
  {
    VarInfo v = _data->symbols()->lookup(vi.name());
    if(v->isState())
    {
      _deps->insert(vi.index(), DEP_STATE_VECTOR);
    }
    else if(v->isAlgebraic())
    {
      _deps->insert(vi.index(), DEP_ALGEBRAIC_VECTOR_DEF);
    }
    else if(v->isDiscrete())
    {
      _deps->insert(vi.index(), DEP_DISCRETE_VECTOR);
    }
  }
  if(_data->calculateAlgebraics())
  {
    map<Index, Index> states, discretes;
    _traverseAlgebraics(_deps, _data->lhs(), _deps, &states, &discretes,
        Index(), DEP_ALGEBRAIC_DEF, -1);
    _traverseAlgebraics(_deps, _data->lhs(), _deps, &states, &discretes,
        Index(), DEP_ALGEBRAIC_VECTOR_DEF, -1);
  }
  _printer = newMMO_PrintExp(_data->symbols(), _ri, data->packages());
}

MMO_Expression_::MMO_Expression_(AST_Expression exp) :
    _exp(exp), _data(NULL), _str(), _equationIndex(0), _expressionOrder(0), _deps(
    NULL), _gen(NULL), _printer(NULL), _ri(NULL)
{
}

MMO_Expression_::MMO_Expression_() :
    _exp(NULL), _data(NULL), _str(), _equationIndex(0), _expressionOrder(0), _deps(
    NULL), _gen(NULL), _printer(NULL), _ri(NULL)
{
}

MMO_Expression_::~MMO_Expression_()
{
  deleteGenerateDeps(_gen);
}

void
MMO_Expression_::_addVectorDeps(MMO_Equation eq, Index index)
{
  Dependencies deps = eq->exp()->deps();
  _deps->join(deps, DEP_STATE_VECTOR);
  _deps->join(deps, DEP_DISCRETE_VECTOR);
  _deps->join(deps, DEP_ALGEBRAIC_VECTOR);
  _addDeps(deps, DEP_STATE, index);
  _addDeps(deps, DEP_DISCRETE, index);
  _addDeps(deps, DEP_ALGEBRAIC, index);
}

void
MMO_Expression_::_addAlgebriacDeps(Index algIndex, MMO_Equation equation,
    Index equationIndex, Index derivativeIndex, Dependencies derivativeDeps,
    map<Index, Index> *states, map<Index, Index> *discretes,
    Index variableChange, int value)
{
  stringstream buffer;
  string indent;
  string variableString = equation->lhs().variable(algIndex, "i");
  Dependencies deps = equation->exp()->deps();
  map<Index, Index> &stateVariables = *states;
  Index eqIndex = equationIndex;
  for(Index *idx = deps->begin(DEP_STATE); !deps->end(DEP_STATE);
      idx = deps->next(DEP_STATE))
  {
    if(stateVariables.find(*idx) == stateVariables.end()
        || equation->lhs().variableChange(algIndex))
    {
      Index stateIndex = Index(
          idx->applyVariableChange(equation->lhs().variableIndex(algIndex)));
      stateVariables[*idx] = *idx;
      eqIndex.setOffset(_equationIndex++);
      if(idx->hasRange() && eqIndex.hasRange())
      {
        // X[i] = B[i];
        derivativeDeps->insert(stateIndex, eqIndex, DEP_ALGEBRAIC_STATE);
      }
      else if(!idx->hasRange() && eqIndex.hasRange())
      {
        // X[i] = B[N];
        derivativeDeps->insert(stateIndex, eqIndex, DEP_ALGEBRAIC_STATE);
      }
      else if(idx->hasRange() && !eqIndex.hasRange())
      {
        // X[N] = B[i];
        if(derivativeIndex.hasRange())
        {
          Index newEqIndex = eqIndex;
          newEqIndex.setRange();
          newEqIndex.setLow(derivativeIndex.low());
          newEqIndex.setHi(derivativeIndex.hi());
          derivativeDeps->insert(
              stateIndex.indexValue(stateIndex.value(algIndex.constant())),
              newEqIndex, DEP_ALGEBRAIC_STATE);
        }
        else
        {
          derivativeDeps->insert(
              stateIndex.indexValue(stateIndex.value(algIndex.constant())),
              eqIndex, DEP_ALGEBRAIC_STATE);
        }
      }
      else if(!idx->hasRange() && !eqIndex.hasRange())
      {
        // X[N] = B[N];
        if(derivativeIndex.hasRange() && !algIndex.isArray())
        {
          Index newEqIndex = eqIndex;
          newEqIndex.setRange();
          newEqIndex.setLow(derivativeIndex.low());
          newEqIndex.setHi(derivativeIndex.hi());
          derivativeDeps->insert(stateIndex, newEqIndex, DEP_ALGEBRAIC_STATE);
        }
        else
        {
          derivativeDeps->insert(stateIndex, eqIndex, DEP_ALGEBRAIC_STATE);
        }

      }
    }
  }
  map<Index, Index> &discreteVariables = *discretes;
  for(Index *idx = deps->begin(DEP_DISCRETE); !deps->end(DEP_DISCRETE); idx =
      deps->next(DEP_DISCRETE))
  {
    if(discreteVariables.find(*idx) == discreteVariables.end()
        || equation->lhs().variableChange(algIndex))
    {
      Index discreteIndex = Index(
          idx->applyVariableChange(equation->lhs().variableIndex(algIndex)));
      discreteVariables[*idx] = *idx;
      eqIndex.setOffset(_equationIndex++);
      if(idx->hasRange() && eqIndex.hasRange())
      {
        // X[i] = B[i];
        derivativeDeps->insert(discreteIndex, eqIndex, DEP_ALGEBRAIC_DISCRETE);
      }
      else if(!idx->hasRange() && eqIndex.hasRange())
      {
        // X[i] = B[N];
        derivativeDeps->insert(discreteIndex, eqIndex, DEP_ALGEBRAIC_DISCRETE);
      }
      else if(idx->hasRange() && !eqIndex.hasRange())
      {
        // X[N] = B[i];
        derivativeDeps->insert(
            discreteIndex.indexValue(
                discreteIndex.value(discreteIndex.constant())), eqIndex,
            DEP_ALGEBRAIC_DISCRETE);
      }
      else if(!idx->hasRange() && !eqIndex.hasRange())
      {
        // X[N] = B[N];
        derivativeDeps->insert(discreteIndex, eqIndex, DEP_ALGEBRAIC_DISCRETE);
      }
    }
  }
  for(Index *idx = deps->begin(DEP_STATE_VECTOR); !deps->end(DEP_STATE_VECTOR);
      idx = deps->next(DEP_STATE_VECTOR))
  {
    derivativeDeps->insert(*idx, DEP_STATE_VECTOR);
  }
  for(Index *idx = deps->begin(DEP_DISCRETE_VECTOR);
      !deps->end(DEP_DISCRETE_VECTOR); idx = deps->next(DEP_DISCRETE_VECTOR))
  {
    derivativeDeps->insert(*idx, DEP_DISCRETE_VECTOR);
  }
  _traverseAlgebraics(deps, derivativeIndex, derivativeDeps, states, discretes,
      variableChange, DEP_ALGEBRAIC_DEF, value);
  _traverseAlgebraics(deps, derivativeIndex, derivativeDeps, states, discretes,
      variableChange, DEP_ALGEBRAIC_VECTOR_DEF, value);
}

void
MMO_Expression_::_traverseAlgebraics(Dependencies deps, Index derivativeIndex,
    Dependencies derivativeDeps, map<Index, Index> *states,
    map<Index, Index> *discretes, Index variableChange, DEP_Type type,
    int value)
{
  for(Index *idx = deps->begin(type); !deps->end(type); idx = deps->next(type))
  {

    list<MMO_Equation> algEqs = _data->algebraics()->equation(*idx);
    list<MMO_Equation>::iterator algEq;
    if(type == DEP_ALGEBRAIC_VECTOR_DEF)
    {
      derivativeDeps->insert(*idx, DEP_ALGEBRAIC_VECTOR);
    }
    int algValue = -1;
    for(algEq = algEqs.begin(); algEq != algEqs.end(); algEq++)
    {
      Index algebraicIdx = *idx;
      if(value >= 0 && idx->hasRange())
      {
        algebraicIdx = idx->indexValue(value);
      }
      if(!(*algEq)->exp()->deps()->autonomous())
      {
        _deps->setAutonomous(false);
      }
      Index variableCh;
      int f = (*algEq)->lhs().factor();
      int c = (*algEq)->lhs().operConstant();
      if(f != 0 && type == DEP_ALGEBRAIC_DEF)
      {
        if(f != 1 || c != 0)
        {
          variableCh.setFactor(f);
          variableCh.setConstant(-c);
          variableCh.setLow((*algEq)->lhs().low() * f + c);
          variableCh.setHi((*algEq)->lhs().hi() * f + c);
        }
      }
      Index algebraicIndex = (*algEq)->lhs();
      if(variableChange.isSet())
      {
        algebraicIndex = (*algEq)->lhs().applyVariableChange(variableChange);
        algebraicIdx = idx->applyVariableChange(variableChange);
        algebraicIdx.setLow(variableChange.low());
        algebraicIdx.setHi(variableChange.hi());
      }
      Intersection is = algebraicIndex.intersection(algebraicIdx);
      if(is.type() != IDX_DISJOINT)
      {
        Index algKey = algebraicIdx;
        Index equationIndex;
        equationIndex.setOffset(_equationIndex++);
        if(is.hasRange())
        {
          equationIndex.setRange();
          equationIndex.setLow(is.low());
          equationIndex.setHi(is.hi());
          algKey.setRange();
          algKey.setHi(equationIndex.hi());
          algKey.setLow(equationIndex.low());
        }
        else
        {
          equationIndex.setConstant(is.modelicaValue());
          if(is.type() == IDX_CONSTANT_BA)
          {
            algKey.setConstant(is.modelicaValue() + is.begin());
            algValue = is.modelicaValue();
          }
          else if(is.type() == IDX_CONSTANT_AB)
          {
            algKey.setConstant(is.modelicaValue() + is.begin());
            algValue = is.modelicaValue();
          }
          else
          {
            algKey.setConstant(is.modelicaValue());
          }
          algKey.setLow(1);
          algKey.setHi(1);
          algKey.setFactor(0);
        }
        if(type == DEP_ALGEBRAIC_DEF)
        {
          derivativeDeps->insert(algKey, DEP_ALGEBRAIC);
        }
        _addAlgebriacDeps(algKey, (*algEq), equationIndex, derivativeIndex,
            derivativeDeps, states, discretes, variableCh, algValue);
      }
    }
  }
}

void
MMO_Expression_::_addDeps(Dependencies deps, DEP_Type type, Index index)
{
  DEP_Type vectorType = DEP_STATE_VECTOR;
  if(type == DEP_DISCRETE)
  {
    vectorType = DEP_DISCRETE_VECTOR;
  }
  else if(type == DEP_ALGEBRAIC)
  {
    vectorType = DEP_ALGEBRAIC_VECTOR;
  }
  for(Index *idx = deps->begin(type); !deps->end(type); idx = deps->next(type))
  {
    Index dIdx(*idx);
    if(dIdx.hasRange())
    {
      // TODO: Agregar las dependencias vectoriales con el rango correcto cuando no
      // estan definidas en el mismo rango que la discreta.
      Index vi(index);
      vi.setOffset(dIdx.offset());
      _deps->insert(vi, vectorType);
    }
    else
    {
      _deps->insert(dIdx, type);
    }
  }
}

AST_Expression
MMO_Expression_::exp()
{
  return _exp;
}

Dependencies
MMO_Expression_::deps()
{
  return _deps;
}

string
MMO_Expression_::print()
{
  string ret;
  return ret;
}

list<string>
MMO_Expression_::getVariables()
{
  return _printer->getVariables();
}

list<string>
MMO_Expression_::getCode()
{
  return _printer->getCode();
}

string
MMO_Expression_::print(string idx, int offset, int order, int constant,
    int expressionOrder, int forOffset)
{
  _printer->setEnvironment(idx, offset, order, constant, _expressionOrder,
      forOffset);
  return _printer->foldTraverse(_exp);
}

string
MMO_Expression_::indexes(string idx, int offset)
{
  ExpIndexes_ ie(_data->symbols());
  ie.setEnvironment(idx, offset);
  return ie.foldTraverse(_exp);
}

bool
MMO_Expression_::hasIndexes()
{
  return !indexes("i").empty();
}

string
MMO_Expression_::findVar(AST_Expression exp)
{
  return _ri->find(exp);
}

MMO_Expression
newMMO_Expression(AST_Expression exp, MMO_ModelData data)
{
  return new MMO_Expression_(exp, data);
}

MMO_Expression
newMMO_Expression(AST_Expression exp)
{
  return new MMO_Expression_(exp);
}

MMO_Expression
newMMO_Expression()
{
  return new MMO_Expression_();
}

void
deleteMMO_Expression(MMO_Expression m)
{
  delete m;
}

/* MMO_EvalInitExp class */

MMO_EvalInitExp_::MMO_EvalInitExp_(VarSymbolTable vt) :
    _vt(vt)
{
}

MMO_EvalInitExp_::~MMO_EvalInitExp_()
{
}

int
MMO_EvalInitExp_::foldTraverseElement(AST_Expression exp)
{
  int ret = 0;
  switch(exp->expressionType())
  {
    case EXPCOMPREF:
      {
      AST_Expression_ComponentReference cr = exp->getAsComponentReference();
      VarInfo vi = _vt->lookup(cr->name());
      if(vi == NULL)
      {
        Error::getInstance()->add(exp->lineNum(),
        EM_IR | EM_VARIABLE_NOT_FOUND,
            ER_Error, "%s", cr->name().c_str());
        return ret;
      }
      if(!vi->isConstant())
      {
        Error::getInstance()->add(exp->lineNum(), EM_IR | EM_INIT_EXP, ER_Error,
            "Only constants allowed inside initial expressions. %s",
            cr->name().c_str());
        return ret;
      }
      return vi->value();
    }
    case EXPBOOLEAN:
      {
      AST_Expression_Boolean eb = exp->getAsBoolean();
      if(eb->value())
      {
        return 1;
      }
      else
      {
        return 0;
      }
    }
    case EXPBOOLEANNOT:
      {
      AST_Expression_BooleanNot ebn = exp->getAsBooleanNot();
      int res = foldTraverse(ebn->exp());
      if(res == 0)
      {
        return 1;
      }
      else
      {
        return 0;
      }
    }
    case EXPREAL:
      Error::getInstance()->add(0, EM_IR | EM_INIT_EXP, ER_Warning,
          "Implicit conversion from Real to Integer, in initial expression.");
      return exp->getAsReal()->val();
    case EXPINTEGER:
      return exp->getAsInteger()->val();
    default:
      Error::getInstance()->add(0,
      EM_IR | EM_INIT_EXP,
          ER_Warning,
          "Initial expression not recognized, returning zero as default value.");
      break;
  }
  return ret;
}

int
MMO_EvalInitExp_::foldTraverseElementUMinus(AST_Expression exp)
{
  return -1 * foldTraverse(exp->getAsUMinus()->exp());
}

int
MMO_EvalInitExp_::foldTraverseElement(int l, int r, BinOpType bot)
{
  switch(bot)
  {
    case BINOPOR:
      if(l != 0 || r != 0)
      {
        return 1;
      }
      else
      {
        return 0;
      }
    case BINOPAND:
      if(l != 0 && r != 0)
      {
        return 1;
      }
      else
      {
        return 0;
      }
    case BINOPLOWER:
      if(l < r)
      {
        return 1;
      }
      else
      {
        return 0;
      }
    case BINOPLOWEREQ:
      if(l <= r)
      {
        return 1;
      }
      else
      {
        return 0;
      }
    case BINOPGREATER:
      if(l > r)
      {
        return 1;
      }
      else
      {
        return 0;
      }
    case BINOPGREATEREQ:
      if(l >= r)
      {
        return 1;
      }
      else
      {
        return 0;
      }
    case BINOPCOMPNE:
      if(l != r)
      {
        return 1;
      }
      else
      {
        return 0;
      }
    case BINOPCOMPEQ:
      if(l == r)
      {
        return 1;
      }
      else
      {
        return 0;
      }
    case BINOPADD:
      return l + r;
    case BINOPSUB:
      return l - r;
    case BINOPDIV:
      if(r != 0)
      {
        return l / r;
      }
      else
      {
        Error::getInstance()->add(0,
        EM_IR | EM_INIT_EXP,
            ER_Warning,
            "Initial expression zero division, returning zero as default value.");
      }
      break;
    case BINOPMULT:
      return l * r;
    default:
      break;
  }
  return 0;
}

MMO_EvalInitExp
newMMO_EvalInitExp(VarSymbolTable vt)
{
  return new MMO_EvalInitExp_(vt);
}

void
deleteMMO_EvalInitExp(MMO_EvalInitExp m)
{
  delete m;
}

/* MMO_ConvertCondition class */

MMO_ConvertCondition_::MMO_ConvertCondition_(MMO_ModelData data) :
    _data(data), _zc(0), _zcRelation(0)
{
}

MMO_ConvertCondition_::~MMO_ConvertCondition_()
{
}

AST_Expression
MMO_ConvertCondition_::foldTraverseElement(AST_Expression exp)
{
  return exp;
}

AST_Expression
MMO_ConvertCondition_::foldTraverseElementUMinus(AST_Expression exp)
{
  return foldTraverse(exp->getAsUMinus()->exp());
}

AST_Expression
MMO_ConvertCondition_::foldTraverseElement(AST_Expression l, AST_Expression r,
    BinOpType bot)
{
  switch(bot)
  {
    case BINOPLOWER:
      {
      AST_Expression bo = newAST_Expression_BinOp(l,
          newAST_Expression_OutputExpressions(newAST_ExpressionList(r)),
          BINOPSUB);
      _zc = -1;
      _zcRelation = 0;
      return bo;
    }
    case BINOPLOWEREQ:
      {
      AST_Expression bo = newAST_Expression_BinOp(l,
          newAST_Expression_OutputExpressions(newAST_ExpressionList(r)),
          BINOPSUB);
      _zc = -1;
      _zcRelation = 1;
      return bo;
    }
    case BINOPGREATER:
      {
      AST_Expression bo = newAST_Expression_BinOp(l,
          newAST_Expression_OutputExpressions(newAST_ExpressionList(r)),
          BINOPSUB);
      _zc = 1;
      _zcRelation = 2;
      return bo;
    }
    case BINOPGREATEREQ:
      {
      AST_Expression bo = newAST_Expression_BinOp(l,
          newAST_Expression_OutputExpressions(newAST_ExpressionList(r)),
          BINOPSUB);
      _zc = 1;
      _zcRelation = 3;
      return bo;
    }
    default:
      break;
  }
  AST_Expression bo = newAST_Expression_BinOp(l, r, bot);
  return bo;
}

int
MMO_ConvertCondition_::zeroCrossing()
{
  return _zc;
}

MMO_ConvertCondition
newMMO_ConvertCondition(MMO_ModelData data)
{
  return new MMO_ConvertCondition_(data);
}

void
deleteMMO_ConvertCondition(MMO_ConvertCondition m)
{
  delete m;
}

/* MMO_PrintExp class. */

MMO_PrintExp_::MMO_PrintExp_(VarSymbolTable vt, MMO_ReplaceInterval ri,
    MMO_PackageTable pt) :
    _vt(vt), _idx(), _offset(0), _order(0), _constant(-1), _functions(), _variables(), _expressionOrder(
        1), _ri(ri), _pt(pt), _forOffset()
{
}

MMO_PrintExp_::~MMO_PrintExp_()
{
}

void
MMO_PrintExp_::setEnvironment(string idx, int offset, int order, int constant,
    int expressionOrder, int forOffset)
{
  _idx = idx;
  _offset = offset;
  _order = order;
  _constant = constant;
  _expressionOrder = expressionOrder;
  _forOffset = forOffset;
}

void
MMO_PrintExp_::_getIndexList(AST_Expression_ComponentReference cr, Index index,
    list<Index> *idxs)
{
  AST_ExpressionListList ell = cr->indexes();
  AST_ExpressionListListIterator ellit;
  ExpressionIndex_ ei(_vt);
  foreach(ellit,ell)
  {
    AST_ExpressionList el = current_element(ellit);
    AST_ExpressionListIterator elit;
    foreach(elit,el)
    {
      Index iet = ei.index(current_element(elit));
      iet.setOffset(index.offset());
      iet.setArray();
      idxs->push_back(iet);
    }
  }
}

list<string>
MMO_PrintExp_::getVariables()
{
  list<string> ret;
  for(map<string, string>::iterator it = _variables.begin();
      it != _variables.end(); it++)
  {
    ret.push_back(it->second);
  }
  return ret;
}

list<string>
MMO_PrintExp_::getCode()
{
  list<string> ret;
  for(map<string, list<string> >::iterator it = _functions.begin();
      it != _functions.end(); it++)
  {
    ret.insert(ret.end(), it->second.begin(), it->second.end());
  }
  return ret;
}

bool
MMO_PrintExp_::_findExpression(AST_ExpressionList el, AST_Expression exp)
{
  AST_ExpressionListIterator eli;
  foreach(eli,el)
  {
    EqualExp ee(_vt);
    if(ee.equalTraverse(current_element(eli), exp))
    {
      return true;
    }
  }
  return false;
}

string
MMO_PrintExp_::foldTraverseElement(AST_Expression exp)
{
  stringstream ret(stringstream::out);
  switch(exp->expressionType())
  {
    case EXPCOMPREF:
      {
      AST_Expression_ComponentReference cr = exp->getAsComponentReference();
      VarInfo vi = _vt->lookup(cr->name());
      if(vi == NULL)
      {
        Error::getInstance()->add(exp->lineNum(),
        EM_IR | EM_VARIABLE_NOT_FOUND,
            ER_Error, "%s", cr->name().c_str());
        return ret.str();
      }
      list<Index> idxs;
      if(cr->hasIndexes())
      {
        _getIndexList(cr, vi->index(), &idxs);
      }
      ret << _vt->print(vi, _idx, _offset, _order, idxs, _constant, _forOffset);
    }
      break;
    case EXPCALLARG:
      {
      AST_ExpressionList el = exp->getAsCallArgs()->arguments();
      AST_ExpressionListIterator it;
      ret << "(";
      int l = el->size(), i = 1;
      foreach(it,el)
      {
        ret << foldTraverse(current_element(it));
        if(i++ < l)
        {
          ret << ",";
        }
      }
      ret << ")";
    }
      break;
    case EXPCALL:
      {
      string name = *(exp->getAsCall()->name());
      BIF_NAMES fn = Util::getInstance()->checkBuiltInReductionFunctions(name);
      if(fn != BIF_NONE)
      {
        string map = _ri->find(exp);
        if(_functions.find(map) == _functions.end())
        {
          BIF *bif = Util::getInstance()->builtInReductionFunctions(fn);
          bif->setSymbolTable(_vt);
          bif->setExpressionOrder(_expressionOrder);
          list<VariableInterval> vars = _ri->variables(exp);
          string vn = Util::getInstance()->newVarName("j", _vt);
          list<string> code = bif->generateCode(map, vn, vars,
              _expressionOrder);
          if(code.empty())
          {
            Error::getInstance()->add(exp->lineNum(),
            EM_IR | EM_VARIABLE_NOT_FOUND,
                ER_Error, "");
            return ret.str();
          }
          _functions[map] = code;
          _variables["int " + vn + ";"] = "int " + vn + ";";
          stringstream v;
          v << "double " << map << "[" << _expressionOrder << "];";
          _variables[v.str()] = v.str();
        }
        ret << map << "[" << _order << "]";
      }
      else
      {
        AST_ExpressionList el = exp->getAsCall()->arguments();
        AST_ExpressionListIterator it;
        BIF_NAMES bif = Util::getInstance()->checkBuiltInFunctions(name);
        if(bif == BIF_PRE)
        {
          ret << foldTraverse(AST_ListFirst(el));
          break;
        }
        else if(bif == BIF_NONE)
        {
          string pre = _pt->functionPrefix(name);
          if(pre.empty())
          {
            ret << "__";
          }
          else
          {
            ret << pre;
          }
        }
        else if(bif == BIF_ABS)
        {
          name = "fabs";
        }
        ret << name << "(";
        int l = el->size(), i = 1;
        foreach(it,el)
        {
          ret << foldTraverse(current_element(it));
          if(i++ < l)
          {
            ret << ",";
          }
        }
        ret << ")";
      }
    }
      break;
    case EXPBOOLEAN:
      if(exp->getAsBoolean()->value())
      {
        ret << "1.0";
      }
      else
      {
        ret << "0.0";
      }
      break;
    case EXPREAL:
      {
      double d = exp->getAsReal()->val();
      ret.precision(24);
      ret << scientific;
      ret << d;
    }
      break;
    case EXPINTEGER:
      ret << exp->getAsInteger()->val() << ".0";
      break;
    case EXPBOOLEANNOT:
      ret << "!" << foldTraverse(exp->getAsBooleanNot()->exp());
      break;
    case EXPDERIVATIVE:
      {
      AST_Expression_Derivative ed = exp->getAsDerivative();
      AST_Expression e = AST_ListFirst(ed->arguments());
      string r;
      if(e->expressionType() == EXPDERIVATIVE)
      {
        AST_Expression ede = AST_ListFirst(e->getAsDerivative()->arguments());
        if(ede->expressionType() == EXPDERIVATIVE)
        {
          _order = 3;
          r = foldTraverse(AST_ListFirst(ede->getAsDerivative()->arguments()));
        }
        else
        {
          _order = 2;
          r = foldTraverse(AST_ListFirst(e->getAsDerivative()->arguments()));
        }
      }
      else
      {
        _order = 1;
        r = foldTraverse(e);
      }
      _order = 0;
      return r;
    }
      break;
    case EXPOUTPUT:
      {
      AST_Expression_Output o = exp->getAsOutput();
      ret << "(" << foldTraverse(AST_ListFirst(o->expressionList())) << ")";
    }
      break;
    case EXPNAMEDARGUMENT:
      {
      AST_Expression_NamedArgument na = exp->getAsNamedArgument();
      ret << foldTraverse(na->expression());
    }
      break;
    default:
      break;
  }
  return ret.str();
}

string
MMO_PrintExp_::_printIndex(Index idx, list<Index> idxs, string sub, int offset,
    int constant)
{
  stringstream buffer;
  /* if (idxs.empty ())
   {
   if (constant >= 0 && idx.isArray ())
   {
   buffer << "[" << idx.mappedValue (constant) << "]";
   }
   else
   {
   buffer << "[" << idx.print (sub, offset) << "]";
   }
   }
   else
   {
   for (list<Index>::iterator it = idxs.begin (); it != idxs.end (); it++)
   {
   buffer << "[";
   if (_vste == VST_OUTPUT)
   {
   if (constant >= 0 && it->isArray ())
   {
   buffer << it->mappedValue (constant) << "]";
   }
   else if (sub.empty ())
   {
   buffer << it->operConstant () << "]";
   }
   else
   {
   buffer << sub << "]";
   }
   }
   else
   {
   if (constant >= 0)
   {
   buffer << it->mappedValue (constant) << "]";
   }
   else
   {
   buffer << it->print (sub, offset) << "]";
   }
   }
   }
   }*/
  return buffer.str();
}

string
MMO_PrintExp_::foldTraverseElementUMinus(AST_Expression exp)
{
  stringstream ret(stringstream::out);
  ret << "-" << foldTraverse(exp->getAsUMinus()->exp());
  return ret.str();
}

string
MMO_PrintExp_::foldTraverseElement(string l, string r, BinOpType bot)
{
  stringstream ret;
  if(bot == BINOPEXP)
  {
    ret << "pow(" << l << "," << r << ")";
  }
  else
  {
    ret << l << Util::getInstance()->opString(bot) << r;
  }
  return ret.str();
}

MMO_PrintExp
newMMO_PrintExp(VarSymbolTable vt, MMO_ReplaceInterval ri, MMO_PackageTable pt)
{
  return new MMO_PrintExp_(vt, ri, pt);
}

void
deleteMMO_PrintExp(MMO_PrintExp m)
{
  delete m;
}

////////////////////////////////////////////////////////////////////////////////////////////

MMO_PartitionInterval_::MMO_PartitionInterval_(VarSymbolTable vt) :
    _vt(vt)
{
}

MMO_PartitionInterval_::~MMO_PartitionInterval_()
{
}

list<int>
MMO_PartitionInterval_::foldTraverseElement(AST_Expression exp)
{
  list<int> part;
  switch(exp->expressionType())
  {
    case EXPCOMPREF:
      {
      AST_Expression_ComponentReference cr = exp->getAsComponentReference();
      VarInfo vi = _vt->lookup(cr->name());
      if(vi == NULL)
      {
        Error::getInstance()->add(exp->lineNum(),
        EM_IR | EM_VARIABLE_NOT_FOUND,
            ER_Fatal, "Partition interval: %s", cr->name().c_str());
      }
      if(cr->hasIndexes())
      {
        AST_ExpressionList indexes = AST_ListFirst(cr->indexes());
        AST_Expression eidx = AST_ListFirst(indexes);
        ExpressionType et = eidx->expressionType();
        MMO_EvalInitExp_ eie(_vt);
        if(et == EXPRANGE)
        {
          AST_ExpressionList rangeExps = eidx->getAsRange()->expressionList();
          AST_ExpressionListIterator it;
          int count = 0;
          int range[3];
          foreach(it,rangeExps)
          {
            range[count++] = eie.foldTraverse(current_element(it));
          }
          if(count == 2)
          {
            range[2] = range[1];
            range[1] = 1;
          }
          int p;
          for(p = range[0]; p < range[2]; p += range[1])
            part.push_back(vi->index().offset()+p);
          return part;
        }
        else if(et == EXPCOLON)
        {
          int p;
          for(p = 0; p < vi->size(); p++)
            part.push_back(vi->index().offset()+p);
          return part;
        }
        else // Expression must be a constant. 
        {
          int p = eie.foldTraverse(eidx);
          part.push_back(vi->index().offset()+p);
        }
      }
      else if(vi->isArray())
      {
        int p;
        for(p = 0; p < vi->size(); p++)
          part.push_back(vi->index().offset()+p);
        return part;
      }
      else 
      {
        part.push_back(vi->index().offset());
        return part;
      }
    }
    default:
      return part;
  }
  return part;
}

list<int>
MMO_PartitionInterval_::foldTraverseElementUMinus(AST_Expression exp)
{
  return list<int>();
}

list<int>
MMO_PartitionInterval_::foldTraverseElement(list<int> l, list<int> r, BinOpType bot)
{
  return list<int>();
}

MMO_PartitionInterval
newMMO_PartitionInterval(VarSymbolTable vt)
{
  return new MMO_PartitionInterval_(vt);
}

void
deleteMMO_PartitionInterval(MMO_PartitionInterval m)
{
  delete m;
}

//////////////////////////////////////////////////////////////////////////////////////////

MMO_ReplaceInterval_::MMO_ReplaceInterval_(VarSymbolTable vt) :
    _vt(vt), _indexes(0), _replacedVars(), _replacedExpsVars(), _replacedExps(), _currentVar()
{
}

MMO_ReplaceInterval_::~MMO_ReplaceInterval_()
{
}

int
MMO_ReplaceInterval_::indexes()
{
  return _replacedVars.size();
}

VariableInterval
MMO_ReplaceInterval_::first()
{
  _indexes = 0;
  list<VariableInterval>::iterator it = _replacedVars.begin();
  if(it != _replacedVars.end())
  {
    return *it;
  }
  return VariableInterval();
}

VariableInterval
MMO_ReplaceInterval_::next()
{
  _indexes++;
  list<VariableInterval>::iterator it = _replacedVars.begin();
  if(_indexes > _replacedVars.size())
  {
    return VariableInterval();
  }
  for(unsigned int j = 0; j < _indexes; j++)
  {
    it++;
  }
  if(it == _replacedVars.end())
  {
    return VariableInterval();
  }
  return *it;
}

bool
MMO_ReplaceInterval_::end()
{
  if(_replacedVars.empty())
  {
    return true;
  }
  return _indexes == _replacedVars.size();
}

AST_Expression
MMO_ReplaceInterval_::_indexExp(int constant, int factor)
{
  _vt->insert("i",
      newVarInfo(newType_Integer(), TP_FOR, NULL, NULL, vector<int>(1, 1),
          false));
  AST_Expression_ComponentReference idx =
      newAST_Expression_ComponentReference();
  AST_Expression_ComponentReference_Add(idx, newAST_String("i"),
      newAST_ExpressionList());
  if(constant == 0 && factor == 1)
  {
    return idx;
  }
  else if(constant == 0 && factor != 1)
  {
    AST_Expression ret = newAST_Expression_BinOp(
        newAST_Expression_Integer(factor), idx, BINOPMULT);
    return ret;
  }
  else if(constant != 0 && factor == 1)
  {
    AST_Expression ret = newAST_Expression_BinOp(
        newAST_Expression_Integer(constant), idx, BINOPADD);
    return ret;
  }
  else if(constant != 0 && factor != 1)
  {
    AST_Expression ret = newAST_Expression_BinOp(
        newAST_Expression_BinOp(newAST_Expression_Integer(factor), idx,
            BINOPMULT),
        newAST_Expression_Integer(constant), BINOPADD);
    //newAST_Expression_Integer (constant - factor + 1), BINOPADD);
    return ret;
  }
  return NULL;
}

void
MMO_ReplaceInterval_::_setIndex(int constant, int factor, int range, VarInfo vi)
{
  Index idx;
  idx.setLow(1);
  idx.setHi(range);
  idx.setFactor(factor);
  idx.setConstant(constant);
  idx.setOffset(vi->index().offset());
  idx.setArray();
  idx.setRange();
  VariableInterval v(idx, vi->name());
  _replacedVars.push_back(v);
  if(!_currentVar.empty())
  {
    if(_replacedExpsVars.find(_currentVar) != _replacedExpsVars.end())
    {
      _replacedExpsVars[_currentVar].push_back(v);
    }
    else
    {
      list<VariableInterval> lvi;
      lvi.push_back(v);
      _replacedExpsVars[_currentVar] = lvi;
    }
  }
}

AST_Expression
MMO_ReplaceInterval_::foldTraverseElement(AST_Expression exp)
{
  switch(exp->expressionType())
  {
    case EXPCOMPREF:
      {
      AST_Expression_ComponentReference cr = exp->getAsComponentReference();
      VarInfo vi = _vt->lookup(cr->name());
      AST_Expression_ComponentReference ret =
          newAST_Expression_ComponentReference();
      if(vi == NULL)
      {
        Error::getInstance()->add(exp->lineNum(),
        EM_IR | EM_VARIABLE_NOT_FOUND,
            ER_Fatal, "Replace interval: %s", cr->name().c_str());
      }
      if(cr->hasIndexes())
      {
        AST_ExpressionList indexes = AST_ListFirst(cr->indexes());
        AST_Expression eidx = AST_ListFirst(indexes);
        ExpressionType et = eidx->expressionType();
        if(et == EXPRANGE)
        {
          AST_ExpressionList rangeExps = eidx->getAsRange()->expressionList();
          AST_ExpressionListIterator it;
          MMO_EvalInitExp_ eie(_vt);
          int count = 0;
          int range[3];
          foreach(it,rangeExps)
          {
            range[count++] = eie.foldTraverse(current_element(it));
          }
          if(count == 2)
          {
            _setIndex(range[0] - vi->index().low() - 1, 1,
                range[1] - range[0] + 1, vi);
            ret->append(newAST_String(cr->name()),
                newAST_SimpleList(_indexExp(range[0] - vi->index().low() - 1)));
          }
          else
          {
            if(range[1] > 0)
            {
              _setIndex(range[0] - vi->index().low() - 1, range[1],
                  (range[2] - range[0]) / range[1] + 1, vi);
            }
            else
            {
              Error::getInstance()->add(exp->lineNum(), EM_IR | EM_FOR_DEF,
                  ER_Fatal, "Range interval not valid");
            }
            ret->append(newAST_String(cr->name()),
                newAST_SimpleList(
                    _indexExp(range[0] - vi->index().low() - 1, range[1])));
          }
          return ret;
        }
        else if(et == EXPCOLON)
        {
          _setIndex(0, 1, vi->size(), vi);
          ret->append(newAST_String(cr->name()),
              newAST_SimpleList(_indexExp()));
          return ret;
        }
      }
      else if(vi->isArray())
      {
        _setIndex(0, 1, vi->size(), vi);
        ret->append(newAST_String(cr->name()), newAST_SimpleList(_indexExp()));
        return ret;
      }
      return exp;
    }
    case EXPCALL:
      {
      string name = *(exp->getAsCall()->name());
      AST_ExpressionList nel = newAST_ExpressionList();
      switch(Util::getInstance()->checkBuiltInFunctions(name))
      {
        case BIF_SUM:
          case BIF_PRODUCT:
          case BIF_MIN:
          case BIF_MAX:
          if(AST_Length(exp->getAsCall()->arguments()) != 1)
          {
            Error::getInstance()->add(exp->lineNum(),
            EM_IR | EM_VARIABLE_NOT_FOUND, ER_Error,
                "%s, expected 1 argument, found %d",
                name.c_str(), AST_Length(exp->getAsCall()->arguments()));
            return newAST_Expression_Call(newAST_String(name), NULL, nel);
          }
          break;
        case BIF_INNER_PRODUCT:
          if(AST_Length(exp->getAsCall()->arguments()) != 2)
          {
            Error::getInstance()->add(exp->lineNum(),
            EM_IR | EM_VARIABLE_NOT_FOUND, ER_Error,
                "%s, expected 2 arguments, found %d",
                name.c_str(), AST_Length(exp->getAsCall()->arguments()));
            return newAST_Expression_Call(newAST_String(name), NULL, nel);
          }
          break;
        default:
          break;
      }
      AST_ExpressionList el = exp->getAsCall()->arguments();
      string varName = Util::getInstance()->newVarName("tmp", _vt);
      _currentVar = varName;
      AST_ExpressionListIterator it;
      foreach(it,el)
      {
        AST_ListAppend(nel, foldTraverse(current_element(it)));
      }
      AST_Expression ret = newAST_Expression_Call(newAST_String(name), NULL,
          nel);
      _replacedExps[varName] = ret;
      if(_replacedExpsVars.find(_currentVar) != _replacedExpsVars.end())
      {
        VarInfo vi = newVarInfo(newType_Real(), 0, NULL, NULL,
            vector<int>(1, 0), false);
        vi->setBuiltIn();
        vi->setName(varName);
        _vt->insert(varName, vi);
      }
      _currentVar = "";
      return ret;
    }
    case EXPCALLARG:
      {
      AST_ExpressionList el = exp->getAsCallArgs()->arguments();
      AST_ExpressionListIterator it;
      AST_ExpressionList nel = newAST_ExpressionList();
      foreach(it,el)
      {
        AST_ListAppend(nel, foldTraverse(current_element(it)));
      }
      return newAST_Expression_FunctionCallArgs(nel);
    }
    case EXPBRACE:
      {
      AST_ExpressionList el = exp->getAsBrace()->arguments();
      AST_ExpressionListIterator it;
      AST_ExpressionList nel = newAST_ExpressionList();
      foreach(it,el)
      {
        AST_ListAppend(nel, foldTraverse(current_element(it)));
      }
      return newAST_Expression_Brace(nel);
    }
    case EXPDERIVATIVE:
      {
      AST_ExpressionList el = exp->getAsDerivative()->arguments();
      AST_ExpressionListIterator it;
      AST_ExpressionList nel = newAST_ExpressionList();
      foreach(it,el)
      {
        AST_ListAppend(nel, foldTraverse(current_element(it)));
      }
      return newAST_Expression_Derivative(nel);
    }
    case EXPOUTPUT:
      {
      AST_ExpressionList el = exp->getAsOutput()->expressionList();
      AST_ExpressionListIterator eli;
      AST_ExpressionList ret = newAST_ExpressionList();
      foreach(eli,el)
      {
        AST_ListAppend(ret, foldTraverse(current_element(eli)));
      }
      return newAST_Expression_OutputExpressions(ret);
    }
    case EXPNAMEDARGUMENT:
      {
      AST_Expression_NamedArgument e = exp->getAsNamedArgument();
      return (newAST_Expression_NamedArgument(e->name(),
          foldTraverse(e->expression())));
    }
    default:
      return exp;
  }
  return NULL;
}

AST_Expression
MMO_ReplaceInterval_::foldTraverseElementUMinus(AST_Expression exp)
{
  return newAST_Expression_UnaryMinus(foldTraverse(exp->getAsUMinus()->exp()));
}

AST_Expression
MMO_ReplaceInterval_::foldTraverseElement(AST_Expression l, AST_Expression r,
    BinOpType bot)
{
  return newAST_Expression_BinOp(l, r, bot);
}

bool
MMO_ReplaceInterval_::fixedInterval(string var)
{
  if(!_replacedVars.size())
  {
    return false;
  }
  int range = _replacedExpsVars[var].front().index().range();
  for(list<VariableInterval>::iterator it = _replacedExpsVars[var].begin();
      it != _replacedExpsVars[var].end(); it++)
  {
    if(range != it->index().range())
    {
      return false;
    }
  }
  return true;
}

string
MMO_ReplaceInterval_::find(AST_Expression exp)
{
  for(map<string, AST_Expression>::iterator it = _replacedExps.begin();
      it != _replacedExps.end(); it++)
  {
    EqualExp eq(_vt);
    if(eq.equalTraverse(it->second, exp))
    {
      return it->first;
    }
  }
  return string();
}

list<VariableInterval>
MMO_ReplaceInterval_::variables(AST_Expression exp)
{
  string var = find(exp);
  return _replacedExpsVars[var];
}

MMO_ReplaceInterval
newMMO_ReplaceInterval(VarSymbolTable vt)
{
  return new MMO_ReplaceInterval_(vt);
}

void
deleteMMO_ReplaceInterval(MMO_ReplaceInterval m)
{
  delete m;
}

MMO_ReplaceInnerProduct::MMO_ReplaceInnerProduct(VarSymbolTable vt) :
    _vt(vt)
{
}

MMO_ReplaceInnerProduct::~MMO_ReplaceInnerProduct()
{
}

AST_Expression
MMO_ReplaceInnerProduct::foldTraverseElement(AST_Expression exp)
{
  switch(exp->expressionType())
  {
    case EXPCALL:
      {
      string name = *(exp->getAsCall()->name());
      AST_ExpressionList nel = newAST_ExpressionList();
      AST_ExpressionList el = exp->getAsCall()->arguments();
      AST_ExpressionListIterator it;
      foreach(it,el)
      {
        AST_ListAppend(nel, foldTraverse(current_element(it)));
      }
      AST_Expression ret = newAST_Expression_Call(newAST_String(name), NULL,
          nel);
      return ret;
    }
    case EXPCALLARG:
      {
      AST_ExpressionList el = exp->getAsCallArgs()->arguments();
      AST_ExpressionListIterator it;
      AST_ExpressionList nel = newAST_ExpressionList();
      foreach(it,el)
      {
        AST_ListAppend(nel, foldTraverse(current_element(it)));
      }
      return newAST_Expression_FunctionCallArgs(nel);
    }
    case EXPBRACE:
      {
      AST_ExpressionList el = exp->getAsBrace()->arguments();
      AST_ExpressionListIterator it;
      AST_ExpressionList nel = newAST_ExpressionList();
      foreach(it,el)
      {
        AST_ListAppend(nel, foldTraverse(current_element(it)));
      }
      return newAST_Expression_Brace(nel);
    }
    case EXPOUTPUT:
      {
      AST_ExpressionList el = exp->getAsOutput()->expressionList();
      AST_ExpressionListIterator eli;
      AST_ExpressionList ret = newAST_ExpressionList();
      foreach(eli,el)
      {
        AST_ListAppend(ret, foldTraverse(current_element(eli)));
      }
      return newAST_Expression_OutputExpressions(ret);
    }
    default:
      return exp;
  }
  return NULL;
}

AST_Expression
MMO_ReplaceInnerProduct::foldTraverseElement(AST_Expression l, AST_Expression r,
    BinOpType bot)
{
  if(bot == BINOPMULT)
  {
    if(_controlArray(l) && _controlArray(r))
    {
      string name = "__INNER_PRODUCT";
      AST_ExpressionList nel = newAST_ExpressionList();
      AST_ListAppend(nel, l);
      AST_ListAppend(nel, r);
      AST_Expression ret = newAST_Expression_Call(newAST_String(name),
      NULL,
          nel);
      return ret;
    }
  }
  return newAST_Expression_BinOp(l, r, bot);
}

AST_Expression
MMO_ReplaceInnerProduct::foldTraverseElementUMinus(AST_Expression exp)
{
  return exp;
}

bool
MMO_ReplaceInnerProduct::_controlArray(AST_Expression exp)
{
  if(exp->expressionType() == EXPCOMPREF)
  {
    AST_Expression_ComponentReference cr = exp->getAsComponentReference();
    VarInfo vi = _vt->lookup(cr->name());
    if(vi == NULL)
    {
      Error::getInstance()->add(exp->lineNum(),
      EM_IR | EM_VARIABLE_NOT_FOUND,
          ER_Fatal, "Control Array: %s", cr->name().c_str());
    }
    if(!cr->hasIndexes() && vi->isArray())
    {
      return true;
    }
  }
  return false;
}

int
MMO_ConvertCondition_::zeroCrossingRelation()
{
  return _zcRelation;
}
