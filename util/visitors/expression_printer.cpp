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

#include "expression_printer.h"

#include <sstream>

#include "../../ast/ast_builder.h"
#include "../../ir/helpers.h"
#include "../util.h"
#include "../error.h"

namespace MicroModelica {
  using namespace IR;
  namespace Util {

    ExpressionPrinter::ExpressionPrinter(const VarSymbolTable& symbols) :
      _symbols(symbols),
      _code()
    {
    }

    string 
    ExpressionPrinter::foldTraverseElement(AST_Expression exp)
    {
      stringstream buffer;
      
      switch(exp->expressionType())
      {
        case EXPBOOLEAN:
          buffer << (exp->getAsBoolean()->value() ? 1 : 0); break;
        case EXPBOOLEANNOT:
          buffer << "!" << apply(exp->getAsBooleanNot()->exp()); break;
        case EXPBRACE: break;
        case EXPCALL:
        {
          AST_Expression_Call call = exp->getAsCall();
          CompiledFunctionTable fs = Utils::instance().compiledFunctions();
          Option<CompiledFunction> f = fs[*call->name()];
          if(!f)
          {
            Error::instance().add(exp->lineNum(), EM_IR | EM_VARIABLE_NOT_FOUND, ER_Error, "%s", call->name()->c_str());
            break;
          }
          Utils::instance().setSymbols(_symbols);
          f->setArguments(call->arguments());
          buffer << f.get();
          _code = f->code();
          break;
        }
        case EXPCALLARG: 
        {
          AST_Expression_CallArgs call = exp->getAsCallArgs();
          AST_ExpressionList el = call->arguments();
          AST_ExpressionListIterator it;
          buffer << "(";
          foreach(it, el)
          {
            buffer << apply(current_element(it));
          }
          buffer << ")";
          break;
        }
        case EXPCOMPREF:
        {
          AST_Expression_ComponentReference cr = exp->getAsComponentReference();
          Option<Variable> var = _symbols[cr->name()];
          if(!var)
          {
            Error::instance().add(exp->lineNum(), EM_IR | EM_VARIABLE_NOT_FOUND, ER_Error, "%s", cr->name().c_str());
            break;
          }
          buffer << var.get();
          if(cr->hasIndexes())
          {
            AST_ExpressionList indexes = cr->firstIndex();
            AST_ExpressionListIterator it;
            int size = indexes->size(), i = 0;
            buffer << "(";
            foreach(it, indexes)
            {
              buffer << apply(current_element(it)) << (++i < size ? "," : "");
            }
            buffer << ")";
          }
          break;
        }
        case EXPDERIVATIVE:
        {
          AST_Expression_Derivative der = exp->getAsDerivative();
          buffer << apply(AST_ListFirst(der->arguments()));
          break;
        }
        case EXPINTEGER:
          buffer << exp->getAsInteger()->val();
          break;
        case EXPOUTPUT:
        {
          AST_Expression_Output out = exp->getAsOutput();
          AST_ExpressionListIterator it;
          int size = out->expressionList()->size(), i = 0;
          buffer << "(";
          foreach (it,out->expressionList())
          {
            buffer << apply(current_element(it));
            buffer << (++i == size ? "" : ",");
          }
          buffer << ")";
          break;
        }
        case EXPREAL:
          buffer << exp->getAsReal()->val();
          break;
        case EXPSTRING:
          buffer << exp->getAsString()->str();
          break;
        default:
          return "";
      }
      return buffer.str();
    }

    string 
    ExpressionPrinter::foldTraverseElement(string l, string r, BinOpType bot)
    {
      stringstream buffer;
      switch(bot)
      {
        case BINOPOR: buffer << l << "||" << r; break;
        case BINOPAND: buffer << l << "&&" << r; break;
        case BINOPLOWER: buffer << l << "<" << r; break; 
        case BINOPLOWEREQ: buffer << l << "<=" << r; break;
        case BINOPGREATER: buffer << l << ">" << r; break; 
        case BINOPGREATEREQ: buffer << l << ">=" << r; break;
        case BINOPCOMPNE: buffer << l << "!=" << r; break;
        case BINOPCOMPEQ: buffer << l << "==" << r; break; 
        case BINOPADD: buffer << l << "+" << r; break; 
        case BINOPELADD: buffer << l << "IMPLEMENT" << r; break;
        case BINOPSUB: buffer << l << "-" << r; break;  
        case BINOPELSUB: buffer << l << "IMPLEMENT" << r; break; 
        case BINOPDIV: buffer << l << "/" << r; break;  
        case BINOPELDIV: buffer << l << "IMPLEMENT" << r; break; 
        case BINOPMULT: buffer << l << "*" << r; break;  
        case BINOPELMULT: buffer << l << "IMPLEMENT" << r; break; 
        case BINOPEXP: buffer << l << "^" << r; break;     
        case BINOPELEXP: buffer << l << "IMPLEMENT" << r; break;    
        default:
          return "NOT IMPLEMENTED";
      }
      return buffer.str();
    }
     
    string 
    ExpressionPrinter::foldTraverseElementUMinus(AST_Expression exp)
    {
      stringstream buffer;
      buffer << "-" << apply(exp->getAsUMinus()->exp());
      return buffer.str();
    }
  }
}
