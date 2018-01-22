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

#include "../ast/statement.h"
#include "statement.h"

#include <sstream>

#include "../ast/equation.h"
#include "../ast/expression.h"
#include "../util/ast_util.h"
#include "../util/error.h"
#include "../util/symbol_table.h"
#include "../util/type.h"
#include "../util/util.h"
#include "equation.h"
#include "expression.h"
#include "mmo_util.h"

MMO_Statement_::MMO_Statement_(AST_Statement stm, MMO_ModelData data) :
    _stm(stm), 
    _data(data), 
    _deps(NULL), 
    _lhs(NULL), 
    _initialCode(data->initialCode()), 
    _variables(), 
    _eventLhs(_data->lhs())
{
  if(_initialCode)
  {
    _setInitialCode(_stm);
  }
  else
  {
    _init();
  }
}

MMO_Statement_::MMO_Statement_(AST_Statement stm) :
    _stm(stm), 
    _data(NULL), 
    _deps(NULL), 
    _lhs(NULL), 
    _initialCode(false), 
    _variables()
{
}

MMO_Statement_::MMO_Statement_() :
    _stm(NULL), 
    _data(NULL), 
    _deps(NULL), 
    _lhs(NULL), 
    _initialCode(false), 
    _variables()
{
}

MMO_Statement_::~MMO_Statement_()
{
}

void
MMO_Statement_::_setInitialCode(AST_Statement stm)
{
  GenerateDeps_ de(_data);
  switch(stm->statementType())
  {
    case STIF:
      {
      de.foldTraverse(stm->getAsIf()->condition());
      AST_StatementListIterator it;
      AST_StatementList stl = stm->getAsIf()->statements();
      foreach(it,stl)
      {
        _setInitialCode(current_element(it));
      }
      stl = stm->getAsIf()->else_statements();
      foreach(it,stl)
      {
        _setInitialCode(current_element(it));
      }
      AST_Statement_ElseList eil = stm->getAsIf()->else_if();
      AST_Statement_ElseListIterator eilit;
      foreach(eilit,eil)
      {
        de.foldTraverse(current_element(eilit)->condition());
        stl = current_element(eilit)->statements();
        foreach(it,stl)
        {
          _setInitialCode(current_element(it));
        }
      }
    }
      break;
    case STASSING:
      {
      AST_Statement_Assign sa = stm->getAsAssign();
      de.foldTraverse(sa->exp());
    }
      break;
    case STFOR:
      {
      AST_Statement_For stf = stm->getAsFor();
      AST_StatementList sts = stf->statements();
      AST_StatementListIterator stit;
      foreach(stit,sts)
      {
        _setInitialCode(current_element(stit));
      }
    }
      break;
    default:
      break;
  }
}

/*! \brief MMO_Statement initialization routine.
 *
 * 	Description:
 * 		For statements in the algorithm section of a model, generate the dependencies of the 
 * 	left hand side which consist of the assignments of discrete and all the state variables
 * 	assigned in reinit statements. Also generate the dependencies for the right hand side of the 
 * 	statement containing all the state variables involved in the calculation.
 */

void
MMO_Statement_::_init()
{
  _deps = newDependencies();
  _lhs = newDependencies();
  switch(_stm->statementType())
  {
    case STIF:
      {
      _insertDeps(_stm->getAsIf()->condition());
      AST_StatementListIterator it;
      AST_StatementList stl = _stm->getAsIf()->statements();
      foreach(it,stl)
      {
        _insertDeps(current_element(it));
      }
      stl = _stm->getAsIf()->else_statements();
      foreach(it,stl)
      {
        _insertDeps(current_element(it));
      }
      AST_Statement_ElseList eil = _stm->getAsIf()->else_if();
      AST_Statement_ElseListIterator eilit;
      foreach(eilit,eil)
      {
        _insertDeps(current_element(eilit)->condition());
        stl = current_element(eilit)->statements();
        foreach(it,stl)
        {
          _insertDeps(current_element(it));
        }
      }
    }
      break;
    case STASSING:
      {
      AST_Statement_Assign sa = _stm->getAsAssign();
      AST_Expression_ComponentReference cr = sa->lhs();
      string name = cr->name();
      if(sa->exp()->expressionType() == EXPCALLARG)
      {
        switch(Util::getInstance()->checkBuiltInFunctions(name))
        {
          case BIF_REINIT:
            {
            AST_Expression_CallArgs eca = sa->exp()->getAsCallArgs();
            if(eca->arguments()->size() > 2)
            {
              Error::getInstance()->add(eca->lineNum(), EM_IR | EM_WRONG_EXP,
                  ER_Error, "Expected 2 arguments, found %d",
                  eca->arguments()->size());
            }
            AST_Expression e = AST_ListFirst(eca->arguments());
            if(e->expressionType() != EXPCOMPREF)
            {
              Error::getInstance()->add(e->lineNum(),
              EM_IR | EM_WRONG_EXP,
                  ER_Error,
                  "Only variables references allowed, expressions not allowed.");
            }
            else
            {
              string sname = e->getAsComponentReference()->name();
              VarInfo vi = _data->symbols()->lookup(sname);
              if(vi == NULL)
              {
                Error::getInstance()->add(e->lineNum(),
                EM_IR | EM_VARIABLE_NOT_FOUND, ER_Error, "%s",
                    sname.c_str());
              }
              if(!vi->isState())
              {
                Error::getInstance()->add(e->lineNum(), EM_IR | EM_WRONG_EXP,
                    ER_Error, "Only state variables allowed.");
              }
              else
              {
                _lhs->insert(_getIndex(e->getAsComponentReference(), vi),
                    DEP_STATE);
                AST_ExpressionListIterator eli;
                AST_ExpressionList el = eca->arguments();
                int c = 0;
                foreach(eli,el)
                {
                  if(c == 0)
                  {
                    c++;
                    continue;
                  }
                  e = current_element(eli);
                }
                _insertDeps(e);
              }
            }
          }
            break;
          case BIF_TERMINATE:
            {
            AST_Expression_CallArgs eca = sa->exp()->getAsCallArgs();
            if(eca->arguments()->size() > 1)
            {
              Error::getInstance()->add(eca->lineNum(), EM_IR | EM_WRONG_EXP,
                  ER_Error, "Expected 1 arguments, found %d",
                  eca->arguments()->size());
            }
          }
            break;
          case BIF_SUM:
            case BIF_PRODUCT:
            case BIF_MIN:
            case BIF_MAX:
            case BIF_INNER_PRODUCT:
            Error::getInstance()->add(_stm->lineNum(),
            EM_IR | EM_WRONG_EXP,
                ER_Error, "Reduction expression in statement.");
            break;
          default:
            Error::getInstance()->add(_stm->lineNum(),
            EM_IR | EM_FUNCTION_NOT_FOUND,
                ER_Error, "Init statement %s", name.c_str());
        }
      }
      else
      {
        VarInfo vi = _data->symbols()->lookup(name);
        if(vi == NULL)
        {
          Error::getInstance()->add(_stm->lineNum(),
          EM_IR | EM_VARIABLE_NOT_FOUND,
              ER_Error, "%s", name.c_str());
          return;
        }
        if(!vi->isDiscrete() && _data->whenStatement())
        {
          Error::getInstance()->add(_stm->lineNum(),
          EM_IR | EM_VARIABLE_NOT_FOUND,
              ER_Error, "%s is not a discrete variable.", name.c_str());
        }
        Index idx = _getIndex(cr, vi);
        _lhs->insert(idx, DEP_DISCRETE);
        _insertDeps(sa->exp());
      }
    }
      break;
    case STFOR:
      {
      Range range;
      AST_Statement_For stf = _stm->getAsFor();
      range.get(stf, _data->symbols());
      AST_ForIndexList fil = stf->forIndexList();
      AST_ForIndexListIterator filit;
      foreach (filit, fil)
      {
        AST_ForIndex fi = current_element(filit);
        _data->symbols()->insert(*fi->variable(),
            newVarInfo(newType_Integer(), TP_FOR, NULL, NULL, vector<int>(1, 1),
                false));
      }
      AST_StatementList sts = stf->statements();
      AST_StatementListIterator stit;
      foreach(stit,sts)
      {
        _insertDeps(current_element(stit), range);
      }
    }
      break;
    default:
      break;
  }
}

Index
MMO_Statement_::_getIndex(AST_Expression_ComponentReference cr, VarInfo vi)
{
  Index idx;
  if(cr->hasIndexes())
  {
    AST_ExpressionListList ell = cr->indexes();
    AST_ExpressionListListIterator ellit;
    ExpressionIndex_ ei(_data->symbols());
    foreach(ellit,ell)
    {
      AST_ExpressionList el = current_element(ellit);
      AST_ExpressionListIterator elit;
      int c = 0;
      idx.setDimension(el->size());
      foreach(elit,el)
      {
        ei.setIndex(&idx, vi->index());
        idx.setIndex(ei.index(current_element(elit)), c);
        Index lhs = _data->lhs();
        idx.setLow(lhs.low(), c);
        idx.setHi(lhs.hi(), c);
        c++;
      }
    }
    idx.setArray();
    idx.setOffset(vi->index().offset());
  }
  else
  {
    idx = vi->index();
  }
  return idx;
}

/*! \brief Inserts the dependencies from an AST_Expression into the statements dependencies.
 *
 *  \param exp: AST_Expression to be processed. 
 *
 * 	Description:
 * 		Creates a new MMO_Expression without the inverse matrix generation
 * 		and join the dependencies of the expression with the dependencies of
 * 		the MMO_Statement.
 *
 */

void
MMO_Statement_::_insertDeps(AST_Expression exp)
{
  Index tmp = _data->lhs();
  _data->setCalculateAlgegraics(true);
  _data->setLHS(_eventLhs);
  MMO_Expression e = newMMO_Expression(exp, _data);
  _data->setCalculateAlgegraics(false);
  _data->setLHS(tmp);
  _deps->join(e->deps());
  deleteMMO_Expression(e);
}

void
MMO_Statement_::_insertVectorDeps(Dependencies deps, Dependencies in,
    DEP_Type type, DEP_Type insert, Range range)
{
  for(Index *idx = deps->begin(type); !deps->end(type); idx = deps->next(type))
  {
    if(idx->factor() != 0)
    {
      Index vector(*idx);
      vector.setDimension(range.dimension());
      vector.setRange(range);
      in->insert(vector, insert);
    }
    else
    {
      in->insert(*idx, type);
    }
  }
}

/*! \brief Inserts the dependencies of an AST_Statement. 
 *
 * 	\param stm: AST_Statement to be processed.
 */

void
MMO_Statement_::_insertDeps(AST_Statement stm, Range range)
{
  MMO_Statement ls = newMMO_Statement(stm, _data);
  if(!range.empty())
  {
    Dependencies deps = ls->deps();
    _insertVectorDeps(deps, _deps, DEP_STATE, DEP_STATE_VECTOR, range);
    _insertVectorDeps(deps, _deps, DEP_DISCRETE, DEP_DISCRETE_VECTOR, range);
    _insertVectorDeps(deps, _deps, DEP_ALGEBRAIC, DEP_ALGEBRAIC_VECTOR, range);
    deps = ls->lhs();
    _insertVectorDeps(deps, _lhs, DEP_STATE, DEP_STATE_VECTOR, range);
    _insertVectorDeps(deps, _lhs, DEP_DISCRETE, DEP_DISCRETE_VECTOR, range);
    _insertVectorDeps(deps, _lhs, DEP_ALGEBRAIC, DEP_ALGEBRAIC_VECTOR, range);

  }
  else
  {
    _deps->join(ls->deps());
    _lhs->join(ls->lhs());
  }
  deleteMMO_Statement(ls);
}

Dependencies
MMO_Statement_::deps()
{
  return _deps;
}

Dependencies
MMO_Statement_::lhs()
{
  return _lhs;
}

string
MMO_Statement_::print()
{
  string ret;
  return ret;
}

void
MMO_Statement_::_getIndexList(AST_Expression_ComponentReference cr, Index index,
    list<Index> *idxs)
{
  AST_ExpressionListList ell = cr->indexes();
  AST_ExpressionListListIterator ellit;
  ExpressionIndex_ ei(_data->symbols());
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

void
MMO_Statement_::_printAssignment(const string& name,
    AST_Expression_ComponentReference cr, AST_Expression e,
    const string& indent, const string& idx,
    int offset, int order, int forOffset, list<string>& ret)
{
  list<Index> idxs;
  VarSymbolTable vt = _data->symbols();
  VarInfo vi = vt->lookup(name);
  if(cr->hasIndexes())
  {
    _getIndexList(cr, vi->index(), &idxs);
  }
  _data->setCalculateAlgegraics(!_initialCode);
  Index tmp = _data->lhs();
  _data->setLHS(_eventLhs);
  MMO_Equation eq = newMMO_Equation(e, _data);
  _data->setCalculateAlgegraics(false);
  _data->setLHS(tmp);
  bool mod = vi->hasEachModifier();
  vi->setEachModifier(false);
  string index = vt->print(vi, idx, offset, 0, idxs, -1, forOffset);
  list<string> code = eq->print(indent, index, idx, true, _data->algebraics(),
      EQ_HANDLER, order, false, offset, false, forOffset);
  vi->setEachModifier(mod);
  stringstream reverse;
  if(vi->isParameter() && (vi->type()->getType() == TYINTEGER))
  {
    list<Index>::iterator it = idxs.begin();
    reverse << "__reverse__PAR_" << vi->name() << "[" << index << "-1] = "
        << it->print(idx, offset) << "+1;";
  }
  list<string> vars = eq->getVariables();
  _variables.insert(_variables.end(), vars.begin(), vars.end());
  ret.insert(ret.end(), code.begin(), code.end());
  ret.push_back(reverse.str());
  deleteMMO_Equation(eq);
}

void
MMO_Statement_::_printList(AST_StatementListIterator it, AST_StatementList stl,
    const string& indent, const string& idx, int offset, int order,
    int forOffset, list<string>& ret)
{
  foreach(it,stl)
  {
    bool ic = _data->initialCode();
    _data->setInitialCode(_initialCode);
    MMO_Statement s = newMMO_Statement(current_element(it), _data);
    list<string> stms = s->print(indent, idx, offset, order, forOffset);
    list<string> vars = s->getVariables();
    _variables.insert(_variables.end(), vars.begin(), vars.end());
    ret.insert(ret.end(), stms.begin(), stms.end());
    deleteMMO_Statement(s);
    _data->setInitialCode(ic);
  }
}

void
MMO_Statement_::_printIfExpression(AST_Expression e, string lhs,
    const string& indent, const string& idx, int order, int offset,
    int forOffset,
    list<string>& ret, list<string>& code)
{
  Index tmp = _data->lhs();
  _data->setCalculateAlgegraics(!_initialCode);
  _data->setLHS(_eventLhs);
  MMO_Equation exp = newMMO_Equation(e, _data);
  _data->setCalculateAlgegraics(false);
  _data->setLHS(tmp);
  exp->print(indent, lhs, idx, true, _data->algebraics(), EQ_HANDLER_IF, order,
      false, offset, false, forOffset);
  list<string> vars = exp->getVariables();
  _variables.insert(_variables.end(), vars.begin(), vars.end());
  list<string> def = exp->getEquation();
  list<string> c = exp->getCode();
  code.insert(code.end(), c.begin(), c.end());
  c = exp->getAlgebraics();
  code.insert(code.end(), c.begin(), c.end());
  ret.insert(ret.end(), def.begin(), def.end());
  deleteMMO_Equation(exp);
}

list<string>
MMO_Statement_::print(string indent, string idx, int offset, int order,
    int forOffset)
{
  list<string> ret;
  stringstream buffer;
  switch(_stm->statementType())
  {
    case STIF:
      {
      list<string> code;
      list<string> stms;
      AST_Expression e = _stm->getAsIf()->condition();
      _printIfExpression(e, "if", indent, idx, order, offset, forOffset, stms,
          code);
      stms.push_back("{");
      AST_StatementListIterator it;
      AST_StatementList stl = _stm->getAsIf()->statements();
      _printList(it, stl, indent, idx, offset, order, forOffset, stms);
      stms.push_back("}");
      AST_Statement_ElseList eil = _stm->getAsIf()->else_if();
      AST_Statement_ElseListIterator eilit;
      foreach(eilit,eil)
      {
        _printIfExpression(current_element(eilit)->condition(), "else if",
            indent, idx, order, offset, forOffset, stms, code);
        stms.push_back("{");
        stl = current_element(eilit)->statements();
        _printList(it, stl, indent, idx, offset, order, forOffset, stms);
        stms.push_back("}");
      }
      stl = _stm->getAsIf()->else_statements();
      if(stl->size())
      {
        stms.push_back("else");
        stms.push_back("{");
      }
      _printList(it, stl, indent, idx, offset, order, forOffset, stms);
      if(stl->size())
      {
        stms.push_back("}");
      }
      ret.insert(ret.end(), code.begin(), code.end());
      ret.insert(ret.end(), stms.begin(), stms.end());
    }
      break;
    case STASSING:
      {
      AST_Statement_Assign sa = _stm->getAsAssign();
      AST_Expression_ComponentReference cr = sa->lhs();
      string name = cr->name();
      if(sa->exp()->expressionType() == EXPCALLARG)
      {
        switch(Util::getInstance()->checkBuiltInFunctions(name))
        {
          case BIF_REINIT:
            {
            AST_Expression_CallArgs eca = sa->exp()->getAsCallArgs();
            AST_Expression e;
            int c = 0;
            AST_ExpressionListIterator eli;
            foreach(eli,eca->arguments())
            {
              e = current_element(eli);
              AST_Expression_ComponentReference cRef;
              if(c == 0)
              {
                cRef = e->getAsComponentReference();
              }
              else
              {
                _printAssignment(cRef->name(), cRef, e, indent, idx, offset,
                    order, forOffset, ret);
              }
              c++;
            }
            buffer.str("");
          }
            break;
          case BIF_TERMINATE:
            {
            ret.push_back("CMD_terminate();");
          }
            break;
          default:
            Error::getInstance()->add(_stm->lineNum(),
            EM_IR | EM_FUNCTION_NOT_FOUND,
                ER_Error, "Print statement %s", name.c_str());
        }
      }
      else
      {
        _printAssignment(name, cr, sa->exp(), indent, idx, offset, order,
            forOffset, ret);
      }
    }
      break;
    case STFOR:
      {
      Range range;
      AST_Statement_For stf = _stm->getAsFor();
      range.get(stf, _data->symbols());
      int size = range.dimension();
      AST_StatementList sts = stf->statements();
      AST_StatementListIterator it;
      string varName = "i0";
      for(int i = 0; i < size; i++)
      {
        range.setLocalVariable(varName, i);
        buffer << "for(" << varName << " = " << range.begin(i) - 1 << "; "
            << varName << " <= " << range.end(i) - 1 << "; " << varName
            << "++)";
        ret.push_back(buffer.str());
        buffer.str("");
        ret.push_back("{");
      }
      foreach(it,sts)
      {
        _data->setInitialCode(_initialCode);
        _data->setRange(range);
        MMO_Statement_ s(current_element(it), _data);
        list<string> stms = s.print(indent, "i", offset, order, -offset + 1);
        _data->setInitialCode(false);
        list<string> vars = s.getVariables();
        _variables.insert(_variables.end(), vars.begin(), vars.end());
        ret.insert(ret.end(), stms.begin(), stms.end());
      }
      for(int i = 0; i < size; i++)
      {
        ret.push_back("}");
      }
    }
      break;
    default:
      break;
  }
  return ret;
}

list<string>
MMO_Statement_::getVariables()
{
  return _variables;
}

MMO_Statement
newMMO_Statement(AST_Statement stm, MMO_ModelData data)
{
  return new MMO_Statement_(stm, data);
}

MMO_Statement
newMMO_Statement(AST_Statement stm)
{
  return new MMO_Statement_(stm);
}

MMO_Statement
newMMO_Statement()
{
  return new MMO_Statement_();
}

void
deleteMMO_Statement(MMO_Statement m)
{
  delete m;
}
