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

    Function::~Function()
    {
    }
    
    void
    Function::definition()
    {
      _includes();
      _prototype();
      _definition();
      _return();
    }

    void 
    Function::header()
    {
    }

    void 
    Function::_includes()
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

          /*list<string> code;
      MMO_ImportTable it = f->imports();
      for(string i = it->begin(); !it->end(); i = it->next())
      {
        string addInclude = Util::getInstance()->packageName(i);
        if(_includes.find(addInclude) == _includes.end())
        {
          _includes[addInclude] = addInclude;
          _writer->write("#include \"" + addInclude + ".h\"", WR_FUNCTION_HEADER);
        }
      }
      if(f->annotation()->hasInclude())
      {
        string addInclude = f->annotation()->include();
        if(_includes.find(addInclude) == _includes.end())
        {
          _includes[addInclude] = addInclude;
          _writer->write(addInclude, WR_FUNCTION_HEADER);
        }
      }
      string indent = _writer->indent(1);
      stringstream buffer;
      buffer << f->prototype();
      _writer->write(&buffer, WR_FUNCTION_CODE);
      _writer->write("{", WR_FUNCTION_CODE);
      _writer->beginBlock();
      _printList(f->localDeclarations());
      MMO_StatementTable stt = f->statements();
      VarSymbolTable vt = f->varTable();
      vt->setPrintEnvironment(VST_FUNCTION);
      for(MMO_Statement st = stt->begin(); !stt->end(); st = stt->next())
      {
        list<string> sts = st->print(indent);
        _printList(st->getVariables());
        code.insert(code.end(), sts.begin(), sts.end());
      }
      _printList(code);
      MMO_ArgumentsTable at = f->externalFunctionCalls();
      for(MMO_FunctionData fd = at->begin(); !at->end(); fd = at->next())
      {
        _printList(fd->print(indent));
      }
      _writer->write(f->returnStatement(), WR_FUNCTION_CODE);
      _writer->endBlock();
      _writer->write("}", WR_FUNCTION_CODE);*/


    string 
    Function::_prototype()
    {
      stringstream input;
      stringstream output;
      stringstream func;
      VarSymbolTable::iterator it;
      VarSymbolTable vst = _function.arguments();
      int outputs = _function.outputNbr();
      string name = _function.name();
      for(VarInfo vi = vst.begin(it); !vst.end(it); vi = vst.next(it))
      {
        if(vi.isInput())
        {
          input << "double ";
          if(vi.isArray())
          {
            input << "*";
          }
          input << vi.name() << ",";
        }
        else if(vi.isOutput())
        {
          output << "double *" << vi.name() << ",";
          _returnVariable = vi.name();
        }
      }
      if(outputs == 0)
      {
        string in = input.str();
        if(!in.empty())
        {
          in.erase(in.end() - 1, in.end());
        }
        func << "void " << _prefix << name << "(" << in << ")";
      }
      else if(outputs == 1)
      {
        string in = input.str();
        if(!in.empty())
        {
          in.erase(in.end() - 1, in.end());
        }
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
    Function::prototype()
    {
      return _prototype()+";";
    }

    void 
    Function::_definition()
    {
      /*stringstream buffer;
      _writer->write("{", FUNCTION_CODE);
      _writer->beginBlock();
      _localSymbols();
      StatementTable stms = _function.statements();
      StatementTable::iterator it;
      for(Statement stm = stms.begin(it); !stms.end(it); stm = stms.next(it))
      {
        buffer << stm;
      }
      _writer->write(buffer.str(), FUNCTION_CODE);
      MMO_StatementTable stt = f->statements();
      VarSymbolTable vt = f->varTable();
      vt->setPrintEnvironment(VST_FUNCTION);
      for(MMO_Statement st = stt->begin(); !stt->end(); st = stt->next())
      {
        list<string> sts = st->print(indent);
        _printList(st->getVariables());
        code.insert(code.end(), sts.begin(), sts.end());
      }
      _printList(code);
      MMO_ArgumentsTable at = f->externalFunctionCalls();
      for(MMO_FunctionData fd = at->begin(); !at->end(); fd = at->next())
      {
        _printList(fd->print(indent));
      }
      _return();
      _writer->endBlock();
      _writer->write("}", FUNCTION_CODE);

*/
    }
   
    void 
    Function::_localSymbols()
    {
  /*    list<string> ret;
      stringstream buffer;
      VarSymbolTable localSymbols = _function.localSymbols();
      VarSymbolTable::iterator it;
      for(VarInfo vi = localSymbols.begin(it); !localSymbols.end(it); vi = localSymbols.next(it))
      {
        if(vi.isConstant())
        {
          continue;
        }
        Index idx = vi->index();
        buffer << "double " << vi.name();
        if(vi.isArray())
        {
          buffer << "[" << vi.size() << "]";
        }
        buffer << ";";
        ret.push_back(buffer.str());
        buffer.str("");
        string indexVar;
        if(vi.hasEachModifier())
        {
          indexVar = Utils::instance().newVarName("i", _function.symbols());
          ret.push_back("int " + indexVar + ";");
        }
        if(vi.hasAssignment() || vi.hasStartModifier() || vi.hasEachModifier())
        {

          _data->setSymbols(_localDeclarations);
          Util::getInstance()->setData(_data);
          buffer << Util::getInstance()->printInitialAssignment(vi, "", indexVar);
          ret.push_back(buffer.str());
          buffer.str("");
          _data->setSymbols(_declarations);
        }
      }
      if(_outputs == 1)
      {
        _declarations->setPrintEnvironment(VST_FUNCTION);
        for(VarInfo vi = _declarations->begin(); !_declarations->end(); vi =
            _declarations->next())
        {
          if(!vi->isOutput())
          {
            continue;
          }
          Index idx = vi->index();
          buffer << "double " << vi->name();
          if(vi->isArray())
          {
            buffer << "[" << vi->size() << "]";
          }
          buffer << ";";
          ret.push_back(buffer.str());
          buffer.str("");
          if(vi->hasAssignment() || vi->hasStartModifier() || vi->hasEachModifier())
          {
            _data->setSymbols(_localDeclarations);
            Util::getInstance()->setData(_data);
            buffer << Util::getInstance()->printInitialAssignment(vi, "");
            ret.push_back(buffer.str());
            buffer.str("");
            _data->setSymbols(_declarations);
          }
        }
      }
      return ret;
*/
    }

    void 
    Function::_return()
    {
      if(_function.outputNbr() == 1)
      {
        _writer->write("return " + _returnVariable + ";", FUNCTION_CODE);
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
  }
}


