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

#include "ginac_interface.h"

#include <assert.h>
#include <ginac/add.h>
#include <ginac/inifcns.h>
#include <ginac/mul.h>
#include <ginac/operators.h>
#include <ginac/power.h>
#include <ginac/print.h>
#include <ginac/registrar.h>
#include <ginac/relational.h>
#include <ginac/wildcard.h>
#include <string>
#include <iostream>
#include <list>
#include <sstream>
#include <utility>

#include "../ast/ast_builder.h"
#include "../ast/expression.h"
#include "../ir/expression.h"
#include "../parser/parse.h"
#include "symbol_table.h"
#include "util.h"

using namespace GiNaC;

REGISTER_FUNCTION(der3, dummy())

static ex
der2_derivative(const ex & x, const ex &y, unsigned diff_param)
{
  return der3(x);
}

REGISTER_FUNCTION(der2, derivative_func(der2_derivative))

static ex
der_derivative(const ex & x, const ex &y, unsigned diff_param)
{
  return der2(x, y);
}

REGISTER_FUNCTION(der, derivative_func(der_derivative))

REGISTER_FUNCTION(pre, dummy())

void
my_print_power_dflt(const power & p, const print_dflt & c, unsigned level)
{
  // if the parent operator has the same or a higher precedence
  // we need parentheses around the power
  if(p.op(1).match(-1))
  {
    c.s << "(1/(" << p.op(0) << "))";
  }
  else
  {
    c.s << "(" << p.op(0) << "^" << p.op(1) << ")";
  }
}

void
my_print_mul_dflt(const mul & m, const print_dflt & c, unsigned level)
{
  // get the precedence of the 'power' class
  unsigned power_prec = m.precedence();
  if(level >= power_prec)
    c.s << '(';
  if(m.op(1).match(pow(wild(), -1)))
  {
    c.s << "(" << m.op(0) << "/" << m.op(1).op(0) << ")";
  }
  else if(m.op(0).match(pow(wild(), -1)))
  {
    c.s << "(" << m.op(1) << "/" << m.op(0).op(0) << ")";
  }
  else
  {
    c.s << m.op(0) << "*" << m.op(1);
  }
  if(level >= power_prec)
    c.s << ')';
}
void
my_print_add_dflt(const add & s, const print_dflt & c, unsigned level)
{
  // get the precedence of the 'power' class
  unsigned power_prec = s.precedence();
  if(level >= power_prec)
    c.s << '(';
  if(s.op(0).match(-wild()) && !s.op(1).match(-wild()))
  {
    c.s << s.op(1) << "-" << s.op(0).op(0);
  }
  else
  {
    c.s << s.op(0) << "+" << s.op(1);
  }
  if(level >= power_prec)
    c.s << ')';
}

ConvertToGiNaC::ConvertToGiNaC(VarSymbolTable varEnv, bool forDerivation,
    MMO_Expression exp) :
    _forDerivation(forDerivation), _varEnv(varEnv), _replaceDer(true), _generateIndexes(
        false), _exp(exp)
{
}

ex
ConvertToGiNaC::convert(AST_Expression e, bool replaceDer, bool generateIndexes)
{
  _replaceDer = replaceDer;
  _generateIndexes = generateIndexes;
  ex p = foldTraverse(e);
  return p;
}

static ex
var_derivative(const ex & x, const ex & y, unsigned diff_param)
{
  return der(x, y);
}
REGISTER_FUNCTION(var, derivative_func(var_derivative))

GiNaC::symbol&
ConvertToGiNaC::first()
{
  return getSymbol("");
}

GiNaC::symbol&
ConvertToGiNaC::next()
{
  return getSymbol("");
}

bool
ConvertToGiNaC::end()
{
  return true;
}

map<string, GiNaC::symbol>
ConvertToGiNaC::directory()
{
  return _directory;
}

string
ConvertToGiNaC::identifier(string str)
{
  std::size_t found = str.find("[");
  if(found == std::string::npos)
  {
    return str;
  }
  else
  {
    std::size_t end = str.find("]");
    string ret = str;
    ret.erase(found, end);
    return ret;
  }
  return "";
}

ex
ConvertToGiNaC::foldTraverseElement(ex l, ex r, BinOpType b)
{
  switch(b)
  {
    case BINOPADD:
      return l + r;
    case BINOPSUB:
      return l - r;
    case BINOPMULT:
      return l * r;
    case BINOPDIV:
      return l / r;
    case BINOPEXP:
      return pow(l, r);
    default:
      cerr << "BinOp not converted to GiNaC " << b << endl;
      return ex(0);
  }
}

symbol&
ConvertToGiNaC::getSymbol(AST_Expression_Derivative der)
{
  AST_Expression_ComponentReference cr =
      der->arguments()->front()->getAsComponentReference();
  string s = cr->name();
  s.insert(0, "__der_");
  AST_Expression_ComponentReference new_cr =
      AST_Expression_ComponentReference_Add(
          newAST_Expression_ComponentReference(), newAST_String(s),
          newAST_ExpressionList());
  return getSymbol(new_cr);
}

symbol&
ConvertToGiNaC::getSymbol(AST_Expression_ComponentReference cr)
{
  string s = cr->name();
  if(_generateIndexes)
  {
    ExpIndexes_ ei(_varEnv);
    ei.setEnvironment("i", true);
    s += ei.foldTraverse(cr);
  }
  map<string, symbol>::iterator i = _directory.find(s);
  if(i != _directory.end())
  {
    return i->second;
  }
  else
  {
    symbol c = symbol(s);
    return _directory.insert(make_pair(s, c)).first->second;
  }
}

symbol&
ConvertToGiNaC::getSymbol(string cr)
{
  map<string, symbol>::iterator i = _directory.find(cr);
  if(i != _directory.end())
  {
    return i->second;
  }
  else
  {
    symbol c = symbol(cr);
    return _directory.insert(make_pair(cr, c)).first->second;
  }
}

symbol&
ConvertToGiNaC::getTime()
{
  string s = "time";
  map<string, symbol>::iterator i = _directory.find(s);
  if(i != _directory.end())
    return i->second;
  return _directory.insert(make_pair(s, symbol(s))).first->second;
}

ex
ConvertToGiNaC::foldTraverseElementUMinus(AST_Expression e)
{
  return -convert(e->getAsUMinus()->exp(), _replaceDer, _generateIndexes);
}

ex
ConvertToGiNaC::foldTraverseElement(AST_Expression e)
{
  switch(e->expressionType())
  {
    case EXPREAL:
      return ex(e->getAsReal()->val());
    case EXPINTEGER:
      return ex(e->getAsInteger()->val());
    case EXPCOMPREF:
      {
      if(!_forDerivation)
      {
        return getSymbol(e->getAsComponentReference());
      }
      else
      {
        VarInfo v = _varEnv->lookup(e->getAsComponentReference()->name());
        if(v->isParameter() || v->isDiscrete() || v->isConstant()
            || v->isForType())
          return getSymbol(e->getAsComponentReference());
        else if(v->builtIn() && !v->name().compare("time"))
          return getTime();
        else
          return var(getSymbol(e->getAsComponentReference()), getTime());
      }
    }
    case EXPDERIVATIVE:
      if(_replaceDer)
      {
        return getSymbol(e->getAsDerivative());
      }
      else
      {
        AST_Expression ed = e->getAsDerivative()->arguments()->front();
        switch(ed->expressionType())
        {
          case EXPCOMPREF:
            {
            AST_Expression_ComponentReference cr =
                ed->getAsComponentReference();
            return der(getSymbol(cr), getTime());
          }
          default:
            cerr << "Derivate argument " << ed << " not converted to GiNaC"
                << endl;
            return ex(0);
        }
      }
      break;
    case EXPCALL:
      {
      AST_Expression_Call c = e->getAsCall();
      if(toStr(c->name()) == "sin")
      {
        return (sin(
            convert(AST_ListFirst(c->arguments()), _replaceDer,
                _generateIndexes)));
      }
      else if(toStr(c->name()) == "cos")
      {
        return (cos(
            convert(AST_ListFirst(c->arguments()), _replaceDer,
                _generateIndexes)));
      }
      else if(toStr(c->name()) == "log")
      {
        return (log(
            convert(AST_ListFirst(c->arguments()), _replaceDer,
                _generateIndexes)));
      }
      else if(toStr(c->name()) == "pre")
      {
        return (pre(
            convert(AST_ListFirst(c->arguments()), _replaceDer,
                _generateIndexes)));
      }
      else if(toStr(c->name()) == "der2")
      {
        AST_Expression ed = AST_ListFirst(c->arguments());
        switch(ed->expressionType())
        {
          case EXPCOMPREF:
            {
            AST_Expression_ComponentReference cr =
                ed->getAsComponentReference();
            return der2(getSymbol(cr), getTime());
          }
          default:
            cerr << "Derivate argument " << ed << " not converted to GiNaC"
                << endl;
            return ex(0);
        }
      }
      else if(toStr(c->name()) == "der3")
      {
        return (der3(
            convert(AST_ListFirst(c->arguments()), _replaceDer,
                _generateIndexes)));
      }
      else if(toStr(c->name()) == "sum")
      {
        if(_exp != NULL)
        {
          string varName = _exp->findVar(e);
          Util::getInstance()->addBuiltInVariables(varName, BIV_SUM);
          return var(getSymbol(varName), getTime());
        }
        else
        {
          return ex(0);
        }
      }
      else if(toStr(c->name()) == "product")
      {
        if(_exp != NULL)
        {
          string varName = _exp->findVar(e);
          Util::getInstance()->addBuiltInVariables(varName, BIV_PRODUCT);
          return var(getSymbol(varName), getTime());
        }
        else
        {
          return ex(0);
        }
      }
      else if(toStr(c->name()) == "min")
      {
        if(_exp != NULL)
        {
          string varName = _exp->findVar(e);
          Util::getInstance()->addBuiltInVariables(varName, BIV_MIN);
          return var(getSymbol(varName), getTime());
        }
        else
        {
          return ex(0);
        }
      }
      else if(toStr(c->name()) == "max")
      {
        if(_exp != NULL)
        {
          string varName = _exp->findVar(e);
          Util::getInstance()->addBuiltInVariables(varName, BIV_MAX);
          return var(getSymbol(varName), getTime());
        }
        else
        {
          return ex(0);
        }
      }
      else if(toStr(c->name()) == "__INNER_PRODUCT")
      {
        if(_exp != NULL)
        {
          string varName = _exp->findVar(e);
          Util::getInstance()->addBuiltInVariables(varName, BIV_INNER_PRODUCT);
          return var(getSymbol(varName), getTime());
        }
        else
        {
          return ex(0);
        }
      }
      else if(toStr(c->name()) == "pow")
      {
        AST_ExpressionList el = c->arguments();
        AST_Expression f = AST_ListFirst(el);
        AST_Expression s = AST_ListElement(el, 2);
        return (pow(convert(f, _replaceDer, _generateIndexes),
            convert(s, _replaceDer, _generateIndexes)));
      }
      else if(toStr(c->name()) == "sqrt")
      {
        AST_ExpressionList el = c->arguments();
        AST_Expression f = AST_ListFirst(el);
        return sqrt(convert(f, _replaceDer, _generateIndexes));
      }
      else if(toStr(c->name()) == "exp")
      {
        return (exp(
            convert(AST_ListFirst(c->arguments()), _replaceDer,
                _generateIndexes)));
      }
      else if(Util::getInstance()->checkGKLinkFunctions(toStr(c->name())))
      {
        return ex(0);
      }
      else
      {
        cerr << "Function call : " << c->name()->c_str()
            << " not converted to GiNaC" << endl;
        return ex(0);
      }
    }
      break;
    case EXPOUTPUT:
      {
      AST_Expression el = AST_ListFirst(e->getAsOutput()->expressionList());
      return ex((foldTraverse(el)));
    }
    default:
      cerr << "Expression: " << e << " not converted to GiNaC" << endl;
      return ex(0);
  }
}

AST_Expression
ConvertToExpression::convert(ex exp)
{
  stringstream s(ios_base::out), der_s(ios_base::out);
  int r;
  set_print_func<power, print_dflt> (my_print_power_dflt);
  // set_print_func<mul,print_dflt>(my_print_mul_dflt);
  // set_print_func<add,print_dflt>(my_print_add_dflt);
  s << exp;
  AST_Expression e;
  if(s.str().find("__der_") == 0)
  {
    string ss = s.str().erase(0, 6);
    der_s << "der(" << ss << ")";
    e = parseExpression(der_s.str().c_str(), &r);
  }
  else
  {
    e = parseExpression(s.str().c_str(), &r);
  }
  assert(e!=NULL && r==0);
  return e;
}
