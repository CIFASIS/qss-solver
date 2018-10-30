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

#include "convert_condition.h"

#include <sstream>

#include "../error.h"
#include "../error.h"
#include "../util.h"
#include "../../parser/parse.h"
#include "../../ast/ast_types.h"
#include "../../ast/ast_builder.h"
#include "../../ir/event.h"

namespace MicroModelica {
  using namespace IR;
  namespace Util {

    /* ConvertExpression Class */

    ConvertExpression::ConvertExpression(AST_Expression left, AST_Expression right, VarSymbolTable& symbols) :
      _left(left),
      _right(right),
      _symbols(symbols),
      _convert()
    {
      convert();
    }

    AST_Expression_ComponentReference 
    ConvertExpression::componentReference(AST_Expression exp)
    {
      if(exp->expressionType() == EXPDERIVATIVE)
      {
        AST_Expression_Derivative ed = exp->getAsDerivative();
        return AST_ListFirst(ed->arguments())->getAsComponentReference();
      }
      else if(exp->expressionType() == EXPCOMPREF)
      {
        return exp->getAsComponentReference();
      }
      return NULL;
    }

    Variable 
    ConvertExpression::variable(AST_Expression_ComponentReference exp)
    {  
      Option<Variable> var = _symbols[exp->name()];
      if(!var)
      {
        Error::instance().add(exp->lineNum(), EM_IR | EM_VARIABLE_NOT_FOUND, ER_Fatal, "%s", exp->name().c_str());
      }
      return var.get();
    }

    bool 
    ConvertExpression::scalarExpression(AST_Expression exp)
    {
      return (exp->expressionType() == EXPREAL || exp->expressionType() == EXPINTEGER);
    }

    double 
    ConvertExpression::scalarValue(AST_Expression exp)
    {
      if(exp->expressionType() == EXPINTEGER)
      {
        return exp->getAsInteger()->val();
      }
      else if(exp->expressionType() == EXPREAL)
      {
        return exp->getAsReal()->val();
      }
      return 0;
    }

    void 
    ConvertExpression::convert()
    {
      AST_Expression_ComponentReference eleft = componentReference(_left);
      if(eleft == NULL)
      {
        return;
      }
      string prefix = "";
      string postfix = "";
      if(_left->expressionType() == EXPDERIVATIVE)
      {
        prefix = "der(";
        postfix = ")";
      }
      if(_right->expressionType() == EXPBINOP)
      {
        AST_Expression_BinOp bo = _right->getAsBinOp();
        BinOpType type = bo->binopType();
        if(type == BINOPELADD || type == BINOPELSUB || type == BINOPELDIV 
            || type == BINOPELMULT || type == BINOPELEXP)
        {
          AST_Expression left = bo->left();
          AST_Expression right = bo->right();
          if(right->expressionType() == EXPCOMPREF && left->expressionType() == EXPCOMPREF)
          {
            AST_Expression_ComponentReference cleft = left->getAsComponentReference();
            AST_Expression_ComponentReference cright = right->getAsComponentReference();
            if(!cleft->hasIndexes() && !cright->hasIndexes())
            {
              Variable lvi = variable(cleft);
              Variable rvi = variable(cright);
              if(rvi.size() != lvi.size() && rvi.isArray() && lvi.isArray())
              {
                Error::instance().add(cright->lineNum(), EM_IR | EM_VARIABLE_NOT_FOUND,
                    ER_Fatal, "Different array size in element-wise operation");
              }
              Variable elvi = variable(eleft);
              if(lvi.isArray() && rvi.isArray())
              {
                if(rvi.size() != elvi.size())
                {
                  Error::instance().add(cright->lineNum(), EM_IR | EM_VARIABLE_NOT_FOUND,
                      ER_Fatal, "Different array size in element-wise operation");
                }
                Variable vi(newType_Integer(), TP_FOR, NULL, NULL, vector<int>(1, 1), false);
                _symbols["i"] = vi;
                stringstream buffer;
                buffer << "for i in 1:" << elvi.size() << " loop" << endl;
                buffer << prefix << eleft->name() << "[i]" << postfix << " = "
                    << cleft->name() << "[i] "
                    << Utils::instance().opString(type);
                buffer << cright->name() << "[i];" << endl;
                buffer << "end for" << endl;
                _convert = buffer.str();
              }
              else if(lvi.size() == 1 && rvi.size() > 1)
              {
                if(rvi.size() != elvi.size())
                {
                  Error::instance().add(cright->lineNum(), EM_IR | EM_VARIABLE_NOT_FOUND,
                      ER_Fatal, "Different array size in element-wise operation");
                }
                Variable vi(newType_Integer(), TP_FOR, NULL, NULL, vector<int>(1, 1), false);
                _symbols["i"] = vi;
                stringstream buffer;
                buffer << "for i in 1:" << elvi.size() << " loop" << endl;
                buffer << prefix << eleft->name() << "[i]" << postfix << " = "
                    << cleft->name()
                    << Utils::instance().opString(type);
                buffer << cright->name() << "[i];" << endl;
                buffer << "end for" << endl;
                _convert = buffer.str();
              }
              else if(lvi.size() > 1 && rvi.size() == 1)
              {
                if(lvi.size() != elvi.size())
                {
                  Error::instance().add(cleft->lineNum(), EM_IR | EM_VARIABLE_NOT_FOUND,
                      ER_Error, "Different array size in element-wise operation");
                }
                Variable vi(newType_Integer(), TP_FOR, NULL, NULL, vector<int>(1, 1), false);
                _symbols["i"] = vi;
                stringstream buffer;
                buffer << "for i in 1:" << elvi.size() << " loop" << endl;
                buffer << prefix << eleft->name() << "[i]" << postfix << " = "
                    << cleft->name() << "[i]"
                    << Utils::instance().opString(type);
                buffer << cright->name() << ";" << endl;
                buffer << "end for" << endl;
                _convert = buffer.str();
              }
            }
          }
          else if(left->expressionType() == EXPCOMPREF && scalarExpression(right))
          {
            AST_Expression_ComponentReference cleft = left->getAsComponentReference();
            if(!cleft->hasIndexes())
            {
              Variable lvi = variable(cleft);
              Variable elvi = variable(eleft);
              if(lvi.size() != elvi.size())
              {
                Error::instance().add(cleft->lineNum(), EM_IR | EM_VARIABLE_NOT_FOUND,
                    ER_Error, "Different array size in element-wise operation");
              }
              Variable vi(newType_Integer(), TP_FOR, NULL, NULL, vector<int>(1, 1), false);
              _symbols["i"] = vi;
              stringstream buffer;
              buffer << "for i in 1:" << elvi.size() << " loop" << endl;
              buffer << prefix << eleft->name() << "[i]" << postfix << " = "
                  << cleft->name() << "[i]"
                  << Utils::instance().opString(type);
              buffer << scalarValue(right) << ";" << endl;
              buffer << "end for" << endl;
              _convert = buffer.str();
            }
          }
          else if(right->expressionType() == EXPCOMPREF && scalarExpression(left))
          {
            AST_Expression_ComponentReference cright = right->getAsComponentReference();
            if(!cright->hasIndexes())
            {
              Variable lvi = variable(cright);
              Variable elvi = variable(eleft);
              if(lvi.size() != elvi.size())
              {
                Error::instance().add(cright->lineNum(), EM_IR | EM_VARIABLE_NOT_FOUND,
                    ER_Error, "Different array size in element-wise operation");
              }
              Variable vi(newType_Integer(), TP_FOR, NULL, NULL, vector<int>(1, 1), false);
              _symbols["i"] = vi;
              stringstream buffer;
              buffer << "for i in 1:" << elvi.size() << " loop" << endl;
              buffer << prefix << eleft->name() << "[i]" << postfix << " = "
                  << scalarValue(left)
                  << Utils::instance().opString(type);
              buffer << cright->name() << "[i]" << ";" << endl;
              buffer << "end for" << endl;
              _convert = buffer.str();
            }
          }
        }
      }
    }

    /* ConvertEquation Class */

    ConvertEquation::ConvertEquation(AST_Equation equation, VarSymbolTable& symbols) :
      _symbols(symbols)
    {
      _equation = convert(equation);
    }

    AST_Equation 
    ConvertEquation::convert(AST_Equation eq)
    {
      if(eq->equationType() == EQEQUALITY)
      {
        ReplaceInnerProduct rip(_symbols);
        AST_Expression l = eq->getAsEquality()->left();
        AST_Expression r = rip.apply(eq->getAsEquality()->right());
        string transform = ConvertExpression(l, r,_symbols).get();
        if(transform.empty())
        {
          return newAST_Equation_Equality(l, r);
        }
        int rValue;
        return parseEquation(transform, &rValue);
      }
      else if(eq->equationType() == EQFOR)
      {
        AST_Equation_For eqf = eq->getAsFor();
        AST_ForIndexList fil = eqf->forIndexList();
        AST_EquationList eqs = eqf->equationList();
        AST_EquationListIterator it;
        AST_EquationList tel = newAST_EquationList();
        foreach(it,eqs)
        {
          AST_ListAppend(tel, convert(current_element(it)));
        }
        return newAST_Equation_For(fil, tel);
      }
      return eq;
    }

    /* ConvertStatement Class */

    ConvertStatement::ConvertStatement(AST_Statement statement, VarSymbolTable& symbols) :
      _symbols(symbols)
    {
      _statement = convert(statement);
    }

    AST_StatementList 
    ConvertStatement::convert(AST_StatementList sts)
    {
      AST_StatementList stt = newAST_StatementList();
      AST_StatementListIterator stit;
      foreach(stit,sts)
      {
        AST_ListAppend(stt, convert(current_element(stit)));
      }
      return stt;
    }

    AST_Statement_ElseList 
    ConvertStatement::convert(AST_Statement_ElseList stel)
    {
      AST_Statement_ElseList stelt = newAST_Statement_ElseList();
      AST_Statement_ElseListIterator stelit;
      foreach(stelit,stel)
      {
        AST_Statement_Else ceif = current_element(stelit);
        AST_StatementList newElseIfSts = newAST_StatementList();
        AST_StatementList elseIfSts = ceif->statements();
        AST_StatementListIterator elseIfStIt;
        foreach(elseIfStIt,elseIfSts)
        {
          AST_ListAppend(newElseIfSts, convert(current_element(elseIfStIt)));
        }
        AST_Statement_Else newStElse = newAST_Statement_Else(ceif->condition(), newElseIfSts);
        AST_ListAppend(stelt, newStElse);
      }
      return stelt;
    }

    AST_Statement
    ConvertStatement::convert(AST_Statement st)
    {
      if(st->statementType() == STASSING)
      {
        ReplaceInnerProduct rip(_symbols);
        AST_Expression l = st->getAsAssign()->lhs();
        AST_Expression r = rip.apply(st->getAsAssign()->exp());
        string transform = ConvertExpression(l, r,_symbols).get();
        if(transform.empty())
        {
          return newAST_Statement_Assign(st->getAsAssign()->lhs(), r);
        }
        int rValue;
        return parseStatement(transform, &rValue);
      }
      else if(st->statementType() == STFOR)
      {
        AST_Statement_For stf = st->getAsFor();
        AST_ForIndexList fil = stf->forIndexList();
        AST_StatementList sts = stf->statements();
        AST_StatementList stt = convert(sts);
        AST_Statement retFor = newAST_Statement_For(fil, stt);
        return retFor;
      }
      else if(st->statementType() == STIF)
      {
        AST_Statement_If sti = st->getAsIf();
        AST_StatementList stt = convert(sti->statements());
        AST_StatementList stet = convert(sti->else_statements());
        AST_Statement_ElseList stel = sti->else_if();
        AST_Statement_ElseList stelt = convert(stel);
        AST_Statement retIf = newAST_Statement_If(sti->condition(), stt, stelt,
            stet);
        return retIf;
      }
      else if(st->statementType() == STWHEN)
      {
        AST_Statement_When stWhen = st->getAsWhen();
        AST_StatementList stList = convert(stWhen->statements());
        AST_Statement_ElseList stElseList = convert(stWhen->else_when());
        AST_Statement retWhen = newAST_Statement_When(stWhen->condition(), stList, stElseList, stWhen->comment());
        return retWhen;
      }
      return st;
    }


    /* ConvertCondition class */

    ConvertCondition::ConvertCondition() :
        _zc(EVENT::Zero), 
        _zcRelation(EVENT::GT)
    {
    }

    AST_Expression
    ConvertCondition::foldTraverseElement(AST_Expression l, AST_Expression r, BinOpType bot)
    {
      switch(bot)
      {
        case BINOPLOWER:
        {
          AST_Expression bo = newAST_Expression_BinOp(l, newAST_Expression_OutputExpressions(newAST_ExpressionList(r)), BINOPSUB);
          _zc = EVENT::Negative;
          _zcRelation = EVENT::LT;
          return bo;
        }
        case BINOPLOWEREQ:
        {
          AST_Expression bo = newAST_Expression_BinOp(l, newAST_Expression_OutputExpressions(newAST_ExpressionList(r)), BINOPSUB);
          _zc = EVENT::Negative;
          _zcRelation = EVENT::LE;
          return bo;
        }
        case BINOPGREATER:
          {
          AST_Expression bo = newAST_Expression_BinOp(l, newAST_Expression_OutputExpressions(newAST_ExpressionList(r)), BINOPSUB);
          _zc = EVENT::Positive;
          _zcRelation = EVENT::GT;
          return bo;
        }
        case BINOPGREATEREQ:
          {
          AST_Expression bo = newAST_Expression_BinOp(l, newAST_Expression_OutputExpressions(newAST_ExpressionList(r)), BINOPSUB);
          _zc = EVENT::Positive;
          _zcRelation = EVENT::GE;
          return bo;
        }
        default:
          break;
      }
      AST_Expression bo = newAST_Expression_BinOp(l, r, bot);
      return bo;
    }
  }
}
