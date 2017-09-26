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

#include "type_check.h"

#include <iostream>
#include <list>
#include <string>

#include "../ast/ast_builder.h"
#include "../ast/expression.h"
#include "symbol_table.h"

#define T(s) tyEnv->lookup(s)

TypeCheck_::TypeCheck_(TypeSymbolTable t, VarSymbolTable v) :
    tyEnv(t), varEnv(v)
{
}
;

bool
TypeCheck_::check_equal(Type t1, Type t2)
{
  if(*t1 == T("Integer"))
  {
    if(*t2 == T("Integer") or *t2 == T("Real"))
      return true;
    else
      return false;
  }
  else if(*t1 == T("Real"))
  {
    if(*t2 == T("Integer") or *t2 == T("Real"))
      return true;
    else
      return false;
  }
  else
  {
    return *t1 == *t2;
  }

}

Type
TypeCheck_::check_binop(AST_Expression l, AST_Expression r, BinOpType op)
{
  Type t1 = check_expression(l);
  Type t2 = check_expression(r);
  switch(op)
  {
    case BINOPLOWER:
      case BINOPLOWEREQ:
      case BINOPGREATER:
      case BINOPGREATEREQ:
      case BINOPCOMPNE:
      case BINOPCOMPEQ:
      if(check_equal(t1, t2) and !check_equal(t1, T("Boolean")))
        return T("Boolean");
      else
        throw "Type Error (0)";
    case BINOPDIV:
      case BINOPELDIV:
      case BINOPMULT:
      case BINOPELMULT:
      case BINOPADD:
      case BINOPELADD:
      case BINOPSUB:
      case BINOPELSUB:
      case BINOPEXP:
      case BINOPELEXP:
      if(check_equal(t1, t2)
          and (check_equal(t1, T("Real")) or check_equal(t1, T("Integer"))))
        return t1;
      else
        throw "Type Error (1)";
    case BINOPAND:
      case BINOPOR:
      if(check_equal(t1, t2) and check_equal(t1, T("Boolean")))
        return t1;
      throw "Type Error (2)";
  }
  throw "Error! (TypeCheck_::check_binop)";
}

Type
TypeCheck_::check_expression(AST_Expression e)
{
  Type ct, t1, t2, t;
  switch(e->expressionType())
  {
    case EXPBINOP:
      {
      AST_Expression_BinOp b = e->getAsBinOp();
      return check_binop(b->left(), b->right(), b->binopType());
    }
    case EXPUMINUS:
      {
      AST_Expression_UMinus b = e->getAsUMinus();
      t = check_expression(b->exp());
      if(check_equal(t, T("Integer")) or check_equal(t, T("Real")))
        return t;
      throw "Type Error (3)";
    }

    case EXPOUTPUT:
      {
      AST_Expression_Output b = e->getAsOutput();
      return check_expression(b->expressionList()->front());
    }

    case EXPIF:
      {
      AST_Expression_If b = e->getAsIf();
      ct = check_expression(b->condition());
      t1 = check_expression(b->then());
      t2 = check_expression(b->else_exp()); // Falta el elseIF
      if(!check_equal(ct, T("Boolean")))
        throw "Type Error (4)";
      if(!check_equal(t1, t2))
        throw "Type Error (5)";
      return t1;

    }

    case EXPCALL:
      {
      // Añadir las funciones en la listaaaa de variables
      AST_Expression_Call c = e->getAsCall();
      if( toStr(c->name()) == "sample")
        return T("Boolean");
      if( toStr(c->name()) == "pre")
        return check_expression(c->arguments()->front());
      return T("Real");
    }

    case EXPCOMPREF:
      {
      AST_Expression_ComponentReference b = e->getAsComponentReference();

      VarInfo tt = varEnv->lookup(toStr(b->names()->front()));

      if(tt == NULL)
      {
        cerr << "Var:" << b->names()->front() << ":";
        throw "Variable no existe (8)";
      }
      if(b->indexes()->front()->size() == 0)
        return tt->type();
      else
      {
        Type t = tt->type();
        AST_ExpressionListIterator exit;
        foreach(exit , b->indexes()->front() )
          if(t->getType() == TYARRAY)
            t = t->getAsArray()->arrayOf();
          else
            throw "Type Error (7)";
        return t;
      }

      break;
    }
    case EXPDERIVATIVE:
      return T("Real");
    case EXPBOOLEAN:
      return T("Boolean");
    case EXPSTRING:
      return T("String");
    case EXPREAL:
      return T("Real");
    case EXPINTEGER:
      return T("Integer");
    case EXPBOOLEANNOT:
      {
      AST_Expression_BooleanNot b = e->getAsBooleanNot();
      t = check_expression(b->exp());
      if(!check_equal(t, T("Boolean")))
        throw "Type Error (6)";
      return t;
    }
    default:
      throw "No implrementado aun! (check_expression)";

  }
}
