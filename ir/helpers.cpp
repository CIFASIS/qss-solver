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

#include <sstream>

#include "helpers.h"
#include "built_in_functions.h"
#include "expression.h"
#include "equation.h"
#include "../util/error.h"

namespace MicroModelica {
  using namespace Util;
  namespace IR {
   
    /* ExternalFunction Class Implementation */

    ExternalFunction::ExternalFunction(string lvalue, string name, AST_ExpressionList args, const VarSymbolTable& symbols) :
      _lvalue(lvalue),
      _name(name),
      _args(args),
      _symbols(symbols)
    {
    }
    
    std::ostream& operator<<(std::ostream& out, const ExternalFunction& e)
    {
      list<string> ret;
      stringstream buffer;
      if(!e._lvalue.empty())
      {
        buffer << e._lvalue << " = ";
      }
      buffer << e._name << "(";
      if(e._args != NULL)
      {
        AST_ExpressionListIterator it;
        unsigned int count = 0;
        foreach(it,e._args)
        {
          Expression ex(current_element(it), e._symbols);
          buffer << ex;
          if(++count < e._args->size())
          {
            buffer << ",";
          }
        }
      }
      buffer << ");";
      out << buffer.str();
      return out;
    }
    
    /* CompiledFunction Class Implementation */

    CompiledFunction::CompiledFunction() :
      _name(), 
      _prototype(), 
      _includeDirectory(), 
      _libraryDirectory(), 
      _libraries()
    {
    }


    CompiledFunction::CompiledFunction(string name, string includeDir, string libraryDir, SymbolTable& libraries, string prefix) :
      _name(prefix+name), 
      _prototype(), 
      _includeDirectory(includeDir), 
      _libraryDirectory(libraryDir), 
      _libraries(libraries)
    {
    }

    std::ostream& operator<<(std::ostream& out, const CompiledFunction& cf)
    {
      out << cf.print();
      return out;
    }

    string 
    CompiledFunction::print() const 
    {
      stringstream buffer;
      Option<BuiltInFunctionPrinter> fp = BuiltInFunction::instance().reductionFunctions(_name);
      if(fp)
      {
        buffer << fp.get();
      }
      else 
      {
        buffer << _name << "(";
        AST_ExpressionListIterator it;
        int size = _arguments->size(), i = 0;
        foreach(it,_arguments)
        {
          i++;
          Expression ex(current_element(it), Utils::instance().symbols());
          buffer << ex;
          buffer << (i < size ? "," : "");
        }
        buffer << ")";
      }
      return buffer.str();
    }

    string 
    CompiledFunction::code()
    {
      Option<BuiltInFunctionPrinter> fp = BuiltInFunction::instance().reductionFunctions(_name);
      if(fp)
      {
        return fp->code();
      }
      return "";
    }

    /* CompiledPackage Class Implementation */
    
    CompiledPackage::CompiledPackage()
    {
    }
    
    CompiledPackage::CompiledPackage(string name) :
        _name(name), 
        _cft(), 
        _objects()
    {
    }

    CompiledPackage::CompiledPackage(string name, CompiledFunctionTable cft, ImportTable objects) :
        _name(name), 
        _cft(cft), 
        _objects(objects)
    {
    }

    string
    CompiledPackage::name()
    {
      return _name;
    }

    string
    CompiledPackage::prefix()
    {
      return "__" + _name + "__";
    }

    CompiledFunctionTable 
    CompiledPackage::definitions()
    {
      return _cft;
    }

    ImportTable
    CompiledPackage::objects()
    {
      return _objects;
    }

    SymbolTable 
    CompiledPackage::linkLibraries()
    {
      SymbolTable ret;
      CompiledFunctionTable::iterator it;
      for(CompiledFunction cf = _cft.begin(it); !_cft.end(it); cf = _cft.next(it))
      {
        if(cf.hasLibraries())
        {
          ret.merge(cf.libraries());
        }
      }
      return ret;
    }

    SymbolTable 
    CompiledPackage::libraryDirectories()
    {
      SymbolTable ret;
      CompiledFunctionTable::iterator it;
      for(CompiledFunction cf = _cft.begin(it); !_cft.end(it); cf = _cft.next(it))
      {
        if(cf.hasLibraryDirectory())
        {
          string ld = cf.libraryDirectory();
          ret.insert(ld,ld);
        }
      }
      return ret;
    }

    SymbolTable 
    CompiledPackage::includeDirectories()
    {
      SymbolTable ret;
      CompiledFunctionTable::iterator it;
      for(CompiledFunction cf = _cft.begin(it); !_cft.end(it); cf = _cft.next(it))
      {
        if(cf.hasIncludeDirectory())
        {
          string id = cf.includeDirectory();
          ret.insert(id, id);
        }
      }
      return ret;
    }

    /* Function Printer implementation */

    string 
    FunctionPrinter::beginSwitch()  
    {
      stringstream buffer;
      buffer << "switch(idx)" << endl << "{";
      return buffer.str();
    }

    string 
    FunctionPrinter::endSwitch()  
    {
      return "}";
    }

    string 
    FunctionPrinter::beginExpression(string token, Option<Range> range) const
    {
      stringstream buffer;
      string block = "";
      if(range) 
      { 
        buffer << "if( " << token << "(idx) >= 1 && ";
        buffer << token << "(idx) <= " << range->size() << ")" << endl;
      }
      else 
      {
        buffer << "case " << token << ":" << endl;
      }
      buffer << "{" << endl;
      block += TAB;
      if(range) 
      { 
        buffer << block << "_get" << token << "_idxs(idx," << range->indexes() << ");" << endl;
        range->addLocalVariables();
      }
      return buffer.str();
    }
    
    string 
    FunctionPrinter::endExpression(Option<Range> range) const 
    {
      stringstream buffer;
      if(range)
      {
        buffer << "return;" << endl << "}"; 
      }
      else 
      {
        buffer << "return;" << endl << "}";
      }
      return buffer.str();
    }
    
    string
    FunctionPrinter::algebraics(EquationDependencyMatrix eqdm, depId key)
    {
      stringstream buffer;
      EquationTable algebraics = Utils::instance().algebraics();
      Option<EquationDependency> eqd = eqdm[key];
      if(eqd)
      {
        EquationDependency::iterator eqIt;
        for(eqIt = eqd->begin(); eqIt != eqd->end(); eqIt++)
        {
          Option<Equation> alg = algebraics[*eqIt];
          if(alg)
          {
            buffer << alg.get() << endl;;
          }
          else 
          {
            Error::instance().add(0, EM_CG | EM_NO_EQ, ER_Error, "Algebraic equation not found.");
          }
        }
      }
      return buffer.str();
    }

  }
}
