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

#include "class.h"

#include <iterator>
#include <sstream>
#include <utility>

#include "../util/error.h"


using namespace MicroModelica::Util;

namespace MicroModelica {
  namespace IR
  {
/* MMO_Function class */

/*MMO_Function_::MMO_Function_(string name) :
    _name(name), _externalFuncs(0), _outputs(0), _outputName(), _externalFunctions(
    NULL), _prefix("__"), _arguments()
{
  _types = newTypeSymbolTable();
  _annotations = newMMO_FunctionAnnotation();
  _externalFunctionCalls = newMMO_ArgumentsTable();
  _calledFunctions = newMMO_SymbolRefTable();
  _declarations = newVarSymbolTable();
  _declarations->initialize(_types);
  _localDeclarations = newVarSymbolTable();
  _statements = newMMO_StatementTable();
  _packages = newMMO_PackageTable();
  _imports = newMMO_ImportTable();
  _data = newMMO_ModelData();
  _functions = newMMO_FunctionTable();
  _data->setAnnotation(_annotations);
  _data->setInitialCode(true);
  _data->setPackages(_packages);
  _data->setCalledFunctions(_calledFunctions);
  _data->setFunctions(_functions);
  _data->setSymbols(_declarations);
}


VarSymbolTable
MMO_Function_::varTable()
{
  return _declarations;
}

void
MMO_Function_::setImports(MMO_ImportTable it)
{
  for(string i = it->begin(); !it->end(); i = it->next())
  {
    insert(i);
  }
}

MMO_Annotation
MMO_Function_::annotation()
{
  return _annotations;
}

void
MMO_Function_::insert(string n)
{
}

void
MMO_Function_::insert(AST_Equation eq)
{
  return;
}

void
MMO_Function_::insert(AST_Statement stm)
{
  MMO_Statement s = newMMO_Statement(stm, _data);
  _statements->insert(s);
}

void
MMO_Function_::insert(AST_Statement stm, bool initial)
{
  insert(stm);
}

void
MMO_Function_::insert(MMO_Function f)
{
  return;
}

MMO_StatementTable
MMO_Function_::statements()
{
  return _statements;
}

void
MMO_Function_::insert(AST_External_Function_Call efc)
{
  string crName;
  if(efc->hasComponentReference())
  {
    AST_Expression_ComponentReference cr = efc->componentReference();
    VarInfo vi = _declarations->lookup(cr->name());
    if(vi == NULL)
    {
      vi = _localDeclarations->lookup(cr->name());
      if(vi == NULL)
      {
        Error::getInstance()->add(efc->lineNum(),
        EM_IR | EM_VARIABLE_NOT_FOUND,
            ER_Error, "%s", cr->name().c_str());
        return;
      }
    }
    crName = cr->name();
  }
  AST_ExpressionListIterator eli;
  ControlVars cv = newControlVars(_declarations, _localDeclarations);
  if(efc->args() != NULL)
  {
    foreach(eli,efc->args())
    {
      if(!cv->foldTraverse(current_element(eli)))
      {
        Error::getInstance()->add(efc->lineNum(),
        EM_IR | EM_VARIABLE_NOT_FOUND,
            ER_Error, "External function call.");
        return;
      }
    }
  }
  deleteControlVars(cv);
  Index i(_externalFuncs++, 0);
  MMO_FunctionData fd = newMMO_FunctionData(crName, efc->name(), efc->args(),
      _data);
  _externalFunctionCalls->insert(i, fd);
}

MMO_ArgumentsTable
MMO_Function_::externalFunctionCalls()
{
  return _externalFunctionCalls;
}

void
MMO_Function_::insert(VarName n, VarInfo vi, DEC_Type type)
{
  Index idx;
  idx.setHi(vi->size());
  MMO_EvalInitExp_ e(_declarations);
  vi->setName(n);
  vi->setIndex(idx);
  if(vi->typePrefix() & TP_CONSTANT)
  {
    vi->setValue(e.foldTraverse(vi->modification()->getAsEqual()->exp()));
  }
  _declarations->insert(n, vi);
  if(type == DEC_PUBLIC)
  {
    if(vi->isOutput())
    {
      _outputs++;
    }
    _arguments.push_back(vi);
  }
  else
  {
    _localDeclarations->insert(n, vi);
  }
}

void
MMO_Function_::insert(VarName n, VarInfo vi)
{
  insert(n, vi, DEC_PUBLIC);
}

void
MMO_Function_::insert(AST_Argument_Modification x)
{
  if(!_annotations->insert(x))
  {
    Error::getInstance()->add(x->lineNum(),
    EM_IR | EM_ANNOTATION_NOT_FOUND,
        ER_Error, "%s", x->name()->c_str());
  }
}

string
MMO_Function_::name() const
{
  return _name;
}

list<string>
MMO_Function_::localDeclarations()
{
  list<string> ret;
  stringstream buffer;
  _localDeclarations->setPrintEnvironment(VST_FUNCTION);
  for(VarInfo vi = _localDeclarations->begin(); !_localDeclarations->end(); vi =
      _localDeclarations->next())
  {
    if(vi->isConstant())
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
    string indexVar;
    if(vi->hasEachModifier())
    {
      indexVar = Util::getInstance()->newVarName("i", _declarations);
      ret.push_back("int " + indexVar + ";");
    }
    if(vi->hasAssignment() || vi->hasStartModifier() || vi->hasEachModifier())
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
}

void
MMO_Function_::setPrefix(string prefix)
{
  _prefix = prefix;
}

string
MMO_Function_::prototype()
{
  stringstream input;
  stringstream output;
  stringstream func;
  list<VarInfo>::iterator it;
  for(it = _arguments.begin(); it != _arguments.end(); it++)
  {
    VarInfo vi = *it;
    if(vi->isInput())
    {
      input << "double ";
      if(vi->isArray())
      {
        input << "*";
      }
      input << vi->name() << ",";
    }
    else if(vi->isOutput())
    {
      output << "double *" << vi->name() << ",";
      _outputName = vi->name();
    }
  }
  if(_outputs == 0)
  {
    string in = input.str();
    if(!in.empty())
    {
      in.erase(in.end() - 1, in.end());
    }
    func << "void " << _prefix << _name << "(" << in << ")";
  }
  else if(_outputs == 1)
  {
    string in = input.str();
    if(!in.empty())
    {
      in.erase(in.end() - 1, in.end());
    }
    func << "double " << _prefix << _name << "(" << in << ")";
  }
  else
  {
    string out = output.str();
    out.erase(out.end() - 1, out.end());
    func << "void " << _prefix << _name << "(" << input.str() << out << ")";
  }
  return func.str();
}

string
MMO_Function_::returnStatement()
{
  if(_outputs == 1)
  {
    return "return " + _outputName + ";";
  }
  return "";
}

int
MMO_Function_::outputs()
{
  return _outputs;
}*/
    /* Function Class Implementation*/

    Function::Function(string name) :
      _imports(),
      _name(name),
      _symbols(),
      _localSymbols(),
      _statements(),
      _types(),
      _packages()
    {
    }

    Function::~Function()
    {
    }

    VarSymbolTable
    Function::varTable() const
    {
      return _symbols;
    }

    void
    Function::insert(string n)
    {
      _imports[n] = n;
      if(!Util::Util::getInstance()->readPackage(n, _packages))
      {
        Error::getInstance()->add(0, EM_IR | EM_CANT_OPEN_FILE, ER_Error, "%s.moo", n.c_str());
      }
    }

    void
    Function::insert(AST_Equation eq)  
    {
      return;
    }

    void
    Function::insert(AST_Statement stm) 
    {
    }

    void
    Function::insert(AST_Statement stm, bool initial) 
    {
    }

    void
    Function::insert(AST_External_Function_Call efc)   
    {
    }

    void
    Function::insert(VarName n, VarInfo vi, DEC_Type type)  
    {
    }

    void
    Function::insert(VarName n, VarInfo vi)  
    {
    }

    void
    Function::insert(AST_Argument_Modification x)  
    { 
      if(!_annotations.insert(x))
      {
        Error::getInstance()->add(x->lineNum(), EM_IR | EM_ANNOTATION_NOT_FOUND, ER_Error, "%s", x->name()->c_str());
      }
    }

    string
    Function::name() const
    {
      return _name; 
    }

    ImportTable 
    Function::imports() const
    {
      return _imports;
    }

    /* Package Class Implementation */

    
    Package::Package(string name) :
      _imports(),
      _name(),
      _functions()
    {
    }

    Package::~Package()
    {
    }

    VarSymbolTable
    Package::varTable() const 
    {
      return VarSymbolTable();
    }

    void
    Package::insert(string n) 
    {
      _imports[n] = n;
    }

    void
    Package::insert(AST_Equation eq)  
    {
      return;
    }

    void
    Package::insert(AST_Statement stm)  
    {
      return;
    }

    void
    Package::insert(AST_Statement stm, bool initial) 
    {
      return;
    }

    void
    Package::insert(Function &f)  
    {
      _functions[f.name()] = f;
    }

    void
    Package::insert(AST_External_Function_Call efc)  
    {
      return;
    }

    void
    Package::insert(VarName n, VarInfo vi, DEC_Type type)  
    {
      return;
    }

    void
    Package::insert(VarName n, VarInfo vi)  
    {
      return;
    }

    void
    Package::insert(AST_Argument_Modification x) 
    {
    }

    string
    Package::name() const
    {
      return _name;
    }

    ImportTable 
    Package::imports() const
    {
      return _imports;
    }
    
    FunctionTable
    Package::definitions()
    {
      return _functions;
    }

    string
    Package::fileName() 
    {
      return MicroModelica::Util::Util::getInstance()->packageName(_name);
    }

    string
    Package::prefix()
    {
      return "__" + _name + "__";
    }

    /* Model Class Implementation */

    Model::Model(string name) :
      _name(name),
      _imports()
    {
    }

    Model::~Model()
    {
    }

    void
    Model::insert(VarName n, VarInfo vi, DEC_Type type)  
    {
    }

    void
    Model::insert(VarName n, VarInfo vi) 
    {
    }

    void
    Model::insert(AST_Equation eq)  
    {
    }

    void
    Model::insert(AST_Statement stm)  
    {
    }

    void
    Model::insert(AST_External_Function_Call efc) 
    {
      return;
    }

    void
    Model::insert(AST_Statement stm, bool initial) 
    {
    }

    void
    Model::insert(Function &f) 
    {
    }

    void
    Model::insert(AST_Argument_Modification x) 
    {
    }

    VarSymbolTable
    Model::varTable() const
    {
      return VarSymbolTable();
    }

    void
    Model::insert(string n) 
    {
    }

    string
    Model::name() const
    {
      return "";
    }

    ImportTable
    Model::imports() const
    {
      return _imports;
    }

    /* CompiledFunction Class Implementation */

    CompiledFunction::CompiledFunction() :
      _def(), 
      _name(), 
      _prototype(), 
      _includeDirectory(), 
      _libraryDirectory(), 
      _libraries()
    {
    }


    CompiledFunction::CompiledFunction(string name, string includeDir, string libraryDir, list<string> libraries) :
      _def(), 
      _name(name), 
      _prototype(), 
      _includeDirectory(includeDir), 
      _libraryDirectory(libraryDir), 
      _libraries(libraries)
    {
    }

    CompiledFunction::~CompiledFunction()
    {
    }

    bool
    CompiledFunction::hasIncludeDirectory()
    {
      return !_includeDirectory.empty();
    }

    bool
    CompiledFunction::hasLibraryDirectory()
    {
      return !_libraryDirectory.empty();
    }

    bool
    CompiledFunction::hasLibraries()
    {
      return _libraries.size() > 0;
    }

    string
    CompiledFunction::includeDirectory()
    {
      return _includeDirectory;
    }

    string
    CompiledFunction::libraryDirectory()
    {
      return _libraryDirectory;
    }

    list<string>
    CompiledFunction::libraries()
    {
      return _libraries;
    }

    string
    CompiledFunction::name()
    {
      return _name;
    }

    list<string>
    CompiledFunction::def()
    {
      return _def;
    }

    string
    CompiledFunction::prototype()
    {
      return _prototype;
    }

    /* CompiledPackage Class Implementation */
    
    CompiledPackage::CompiledPackage()
    {
    }

    CompiledPackage::CompiledPackage(string name, CompiledFunctionTable cft, ImportTable objects) :
        _name(name), 
        _cft(cft), 
        _objects(objects)
    {
    }

    CompiledPackage::~CompiledPackage()
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

    list<string>
    CompiledPackage::linkLibraries()
    {
      list<string> ret;
      for(CompiledFunctionTable::iterator it = _cft.begin(); !_cft.end(it); _cft.next(it))
      {
        CompiledFunction cf = _cft.value(it);
        if(cf.hasLibraries())
        {
          list<string> libs = cf.libraries();
          ret.insert(ret.end(), libs.begin(), libs.end());
        }
      }
      return ret;
    }

    list<string>
    CompiledPackage::libraryDirectories()
    {
      list<string> ret;
      for(CompiledFunctionTable::iterator it = _cft.begin(); !_cft.end(it); _cft.next(it))
      {
        CompiledFunction cf = _cft.value(it);
        if(cf.hasLibraryDirectory())
        {
          ret.push_back(cf.libraryDirectory());
        }
      }
      return ret;
    }

    list<string>
    CompiledPackage::includeDirectories()
    {
      list<string> ret;
      for(CompiledFunctionTable::iterator it = _cft.begin(); !_cft.end(it); _cft.next(it))
      {
        CompiledFunction cf = _cft.value(it);
        if(cf.hasIncludeDirectory())
        {
          ret.push_back(cf.includeDirectory());
        }
      }
      return ret;
    }
  }
}
