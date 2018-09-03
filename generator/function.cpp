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

#include "function.h"

namespace MicroModelica {
  using namespace IR;
  using namespace Util;
  namespace Generator {
    Function::Function(IR::Function& function, CompileFlags& flags, WriterPtr writer) :
      _function(function),
      _flags(flags),
      _writer(writer),
      _prefix(),
      _include()
    {
    }

    void
    Function::definition()
    {
      macros();
      includes();
      body();
    }

    void 
    Function::includes()
    {
      list<string> code;
      ImportTable imports = _function.imports();
      ImportTable::iterator it;
      for(string i = imports.begin(it); !imports.end(it); i = imports.next(it))
      {
        string addInclude = Utils::instance().packageName(i);
        if(_include.lookup(addInclude))
        {
          _include[addInclude] = addInclude;
          _writer->write("#include \"" + addInclude + ".h\"", FUNCTION_HEADER);
        }
      }
      if(_function.annotations().hasInclude())
      {
        string addInclude = _function.annotations().include();
        if(_include.lookup(addInclude))
        {
          _include[addInclude] = addInclude;
          _writer->write(addInclude, FUNCTION_HEADER);
        }
      }
    }

    string 
    Function::prototype()
    {
      stringstream input;
      stringstream output;
      stringstream func;
      VarSymbolTable::iterator it;
      VarSymbolTable vst = _function.arguments();
      int outputs = _function.outputNbr();
      string name = _function.name();
      for(Variable var = vst.begin(it); !vst.end(it); var = vst.next(it))
      {
        if(var.isInput())
        {
          input << "double " << (var.isArray() ? "*" : "") << var.name() << ",";
        }
        else if(var.isOutput())
        {
          output << "double *" << var.name() << ",";
          _returnVariable = var.name();
        }
      }
      string in = input.str();
      if(outputs <= 1 && !in.empty()) 
      { 
        in.erase(in.end() - 1, in.end());
      }
      if(outputs == 0)
      {
        func << "void " << _prefix << name << "(" << in << ")";
      }
      else if(outputs == 1)
      {
        func << "double " << _prefix << name << "(" << in << ")";
      }
      else
      {
        string out = output.str();
        out.erase(out.end() - 1, out.end());
        func << "void " << _prefix << name << "(" << input.str() << out << ")";
      }
      return func.str();
    }

    string 
    Function::header()
    {
      return prototype()+";";
    }

    void 
    Function::body()
    {
      stringstream buffer;
      _writer->write(prototype(), FUNCTION_CODE);
      _writer->write("{", FUNCTION_CODE);
      _writer->beginBlock();
      localSymbols();
      StatementTable stms = _function.statements();
      StatementTable::iterator it;
      for(Statement stm = stms.begin(it); !stms.end(it); stm = stms.next(it))
      {
        buffer << stm;
      }
      _writer->write(buffer.str(), FUNCTION_CODE);
      ExternalFunctionTable eft = _function.externalFunctions();
      ExternalFunctionTable::iterator eit;
      for(ExternalFunction ef = eft.begin(eit); !eft.end(eit); ef = eft.next(eit))
      {
        buffer << ef;
      }
      _writer->write(buffer.str(), FUNCTION_CODE);
      if(_function.outputNbr() == 1)
      {
        _writer->write("return " + _returnVariable + ";", FUNCTION_CODE);
      }
      _writer->endBlock();
      _writer->write("}", FUNCTION_CODE);
    }
   
    void 
    Function::localSymbols()
    {
      list<string> ret;
      stringstream buffer;
      VarSymbolTable localSymbols = _function.localSymbols();
      VarSymbolTable::iterator it;
      for(Variable var = localSymbols.begin(it); !localSymbols.end(it); var = localSymbols.next(it))
      {
        if(var.isConstant())
        {
          continue;
        }
        _writer->write(var.declaration(), FUNCTION_CODE);
        _writer->write(var.initialization(_function.localSymbols()), FUNCTION_CODE);
      }
      if(_function.outputNbr() == 1)
      {
        VarSymbolTable symbols = _function.symbols();
        for(Variable var = symbols.begin(it); !symbols.end(it); var = symbols.next(it))
        {
          if(var.isOutput())
          {
            _writer->write(var.declaration(), FUNCTION_CODE);
            _writer->write(var.initialization(_function.localSymbols()), FUNCTION_CODE);
          }
        }
      }
    }

    void 
    Function::setPrefix(string prefix)
    {
      _prefix = prefix;
    }

    void 
    Function::addInclude(string include)
    {
      _include[include] = include;
    }

    void 
    Function::addInclude(list<string> includes)
    {
      list<string>::iterator it;
      for(it = includes.begin(); it != includes.end(); it++)
      {
        _include[*it] = *it;
      }
    }

    void 
    Function::macros()
    {
      stringstream buffer;
      VarSymbolTable localSymbols = _function.localSymbols();
      VarSymbolTable::iterator it;
      for(Variable var = localSymbols.begin(it); !localSymbols.end(it); var = localSymbols.next(it))
      {
        if(var.isConstant())
        {
          continue;
        }
        buffer << "#define " << var << " " << var.name();
        _writer->write(buffer.str(), FUNCTION_CODE);
      }
    }
  }
}


