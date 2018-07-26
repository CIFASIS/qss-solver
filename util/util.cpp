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

#include "util.h"

#include <stddef.h>
#include <cstdlib>
#include <iterator>
#include <sstream>
#include <utility>

#include "../ir/expression.h"
#include "../ir/mmo_util.h"
#include "../ir/package.h"
#include "compile_flags.h"
#include "index.h"
#include "error.h"
#include "symbol_table.h"

#define SLASH "/"

namespace MicroModelica {
  namespace Util {

    Util *Util::_instance = NULL;

    Util::Util() :
        _languageEspecification("C"), 
        _varCounter(1), 
        _flags(), 
        _builtInFunctions(), 
        _builtInVariables(), 
        _builtInFunctionImp()
    {
      _annotations.insert(pair<string, int>("StartTime", 0));
      _annotations.insert(pair<string, int>("StopTime", 1));
      _annotations.insert(pair<string, int>("Tolerance", 2));
      _annotations.insert(pair<string, int>("AbsTolerance", 3));
      _annotations.insert(pair<string, int>("StepSize", 4));
      _builtInVariables.insert(pair<string, BIV_NAMES>("time", BIV_TIME));
      _builtInFunctions.insert(pair<string, BIF_NAMES>("reinit", BIF_REINIT));
      _builtInFunctions.insert(pair<string, BIF_NAMES>("terminate", BIF_TERMINATE));
      _builtInFunctions.insert(pair<string, BIF_NAMES>("sum", BIF_SUM));
      _builtInFunctions.insert( pair<string, BIF_NAMES>("__INNER_PRODUCT", BIF_INNER_PRODUCT));
      _builtInFunctions.insert(pair<string, BIF_NAMES>("product", BIF_PRODUCT));
      _builtInFunctions.insert(pair<string, BIF_NAMES>("min", BIF_MIN));
      _builtInFunctions.insert(pair<string, BIF_NAMES>("max", BIF_MAX));
      _builtInFunctions.insert(pair<string, BIF_NAMES>("abs", BIF_ABS));
      _builtInFunctions.insert(pair<string, BIF_NAMES>("sign", BIF_SIGN));
      _builtInFunctions.insert(pair<string, BIF_NAMES>("sqrt", BIF_SQRT));
      _builtInFunctions.insert(pair<string, BIF_NAMES>("ceil", BIF_CEIL));
      _builtInFunctions.insert(pair<string, BIF_NAMES>("floor", BIF_FLOOR));
      _builtInFunctions.insert(pair<string, BIF_NAMES>("sin", BIF_SIN));
      _builtInFunctions.insert(pair<string, BIF_NAMES>("cos", BIF_COS));
      _builtInFunctions.insert(pair<string, BIF_NAMES>("tan", BIF_TAN));
      _builtInFunctions.insert(pair<string, BIF_NAMES>("asin", BIF_ASIN));
      _builtInFunctions.insert(pair<string, BIF_NAMES>("acos", BIF_ACOS));
      _builtInFunctions.insert(pair<string, BIF_NAMES>("atan", BIF_ATAN));
      _builtInFunctions.insert(pair<string, BIF_NAMES>("atan2", BIF_ATAN2));
      _builtInFunctions.insert(pair<string, BIF_NAMES>("sinh", BIF_SINH));
      _builtInFunctions.insert(pair<string, BIF_NAMES>("cosh", BIF_COSH));
      _builtInFunctions.insert(pair<string, BIF_NAMES>("tanh", BIF_TANH));
      _builtInFunctions.insert(pair<string, BIF_NAMES>("exp", BIF_EXP));
      _builtInFunctions.insert(pair<string, BIF_NAMES>("log", BIF_LOG));
      _builtInFunctions.insert(pair<string, BIF_NAMES>("log10", BIF_LOG10));
      _builtInFunctions.insert(pair<string, BIF_NAMES>("pre", BIF_PRE));
      _builtInFunctions.insert(pair<string, BIF_NAMES>("GQLink_GetB", BIF_GQLINK));
      _builtInFunctions.insert(pair<string, BIF_NAMES>("GQLink_GetBx", BIF_GQLINK));
      _builtInFunctions.insert(pair<string, BIF_NAMES>("GQLink_GetBy", BIF_GQLINK));
      _builtInFunctions.insert(pair<string, BIF_NAMES>("GQLink_GetBz", BIF_GQLINK));
      _builtInFunctionImp.insert(pair<BIF_NAMES, BIF*>(BIF_SUM, new BuiltInSumFunction()));
      _builtInFunctionImp.insert(pair<BIF_NAMES, BIF*>(BIF_PRODUCT, new BuiltInProductFunction()));
      _builtInFunctionImp.insert(pair<BIF_NAMES, BIF*>(BIF_INNER_PRODUCT, new BuiltInInnerProductFunction()));
      _builtInFunctionImp.insert(pair<BIF_NAMES, BIF*>(BIF_MIN, new BuiltInMinFunction())); 
      _builtInFunctionImp.insert(pair<BIF_NAMES, BIF*>(BIF_MAX, new BuiltInMaxFunction()));
      _binop[BINOPOR] = "||";
      _binop[BINOPAND] = "&&";
      _binop[BINOPLOWER] = "<";
      _binop[BINOPLOWEREQ] = "<=";
      _binop[BINOPGREATER] = ">";
      _binop[BINOPGREATEREQ] = ">=";
      _binop[BINOPCOMPNE] = "!=";
      _binop[BINOPCOMPEQ] = "==";
      _binop[BINOPADD] = "+";
      _binop[BINOPELADD] = "+";
      _binop[BINOPSUB] = "-";
      _binop[BINOPELSUB] = "-";
      _binop[BINOPDIV] = "/";
      _binop[BINOPELDIV] = "/";
      _binop[BINOPMULT] = "*";
      _binop[BINOPELMULT] = "*";
      _binop[BINOPEXP] = "^";
      _binop[BINOPELEXP] = "^";
    }

    Util::~Util()
    {
    }

    Util *
    Util::getInstance()
    {
      if(!_instance)
      {
        _instance = new Util();
      }
      return _instance;
    }

    string
    Util::trimString(string str)
    {
      // trim trailing spaces
      string ret = str;
      size_t endpos = ret.find_last_not_of(" \t");
      if(string::npos != endpos)
      {
        ret = ret.substr(0, endpos + 1);
      }

      // trim leading spaces
      size_t startpos = ret.find_first_not_of(" \t");
      if(string::npos != startpos)
      {
        ret = ret.substr(startpos);
      }
      return ret;
    }

    string
    Util::languageEspecification()
    {
      return _languageEspecification;
    }

    void
    Util::setCompileFlags(CompileFlags flags)
    {
      _flags = flags;
    }

    bool
    Util::checkTypeString(string t)
    {
      if(t != "Integer" && t != "Real" && t != "Boolean")
      {
        return false;
      }
      return true;
    }

    bool
    Util::checkExperimentAnnotations(string *annotation)
    {
      map<string, int>::iterator it = _annotations.find(*annotation);
      return it != _annotations.end();
    }

    BIF_NAMES
    Util::checkBuiltInFunctions(string fname)
    {
      map<string, BIF_NAMES>::iterator it = _builtInFunctions.find(fname);
      if(it != _builtInFunctions.end())
      {
        return it->second;
      }
      return BIF_NONE;
    }

    BIV_NAMES
    Util::checkBuiltInVariables(string fname)
    {
      map<string, BIV_NAMES>::iterator it = _builtInVariables.find(fname);
      if(it != _builtInVariables.end())
      {
        return it->second;
      }
      return BIV_NONE;
    }

    void
    Util::addBuiltInVariables(string fname, BIV_NAMES type)
    {
      _builtInVariables.insert(pair<string, BIV_NAMES>(fname, type));
    }

    string
    Util::opString(BinOpType bot)
    {
      return _binop[bot];
    }

    string
    Util::getVarName(string name)
    {
      string ret;
      int ub = name.length() - 1;
      for(int i = 0; i < ub; i++)
      {
        ret += name[i];
      }
      return ret;
    }

    string
    Util::newVarName(string n, VarSymbolTable vt)
    {
      stringstream buffer;
      buffer << n << _varCounter++ << "0";
      while(vt->lookup(buffer.str()) != NULL)
      {
        buffer.str("");
        buffer << n << _varCounter++ << "0";
      }
      return buffer.str();
    }

    list<string>
    Util::_getValue(fstream *package, string token)
    {
      list<string> ret;
      string line;
      while(getline(*package, line))
      {
        if(!line.compare(token))
        {
          break;
        }
        ret.push_back(line);
      }
      return ret;
    }

    /*MMO_PackageData
    Util::readPackage(string fileName)
    {
      fstream package;
      string pname = packageName(fileName);
      string name = _packagePath(pname + ".c");
      name.append(pname).append(".moo");
      package.open(name.c_str());
      if(package.good())
      {
        string line;
        MMO_FunctionDefinitionTable fit = newMMO_FunctionDefinitionTable();
        MMO_ImportTable objects = newMMO_ImportTable();
        string fname;
        string derivative;
        string includeDir;
        string libraryDir;
        list<string> libraries;
        while(getline(package, line))
        {
          if(!line.compare("DEPENDENCES"))
          {
            list<string> objs = _getValue(&package, "ENDDEPENDENCES");
            for(list<string>::iterator it = objs.begin(); it != objs.end(); ++it)
            {
              objects->insert(*it);
            }
          }
          else if(!line.compare("FUNCTION"))
          {
            fname = _getValue(&package, "ENDFUNCTION").front();
          }
          else if(!line.compare("DERIVATIVE"))
          {
            derivative = _getValue(&package, "ENDDERIVATIVE").front();
          }
          else if(!line.compare("INCLUDEDIRECTORY"))
          {
            includeDir = _getValue(&package, "ENDINCLUDEDIRECTORY").front();
          }
          else if(!line.compare("LIBRARYDIRECTORY"))
          {
            libraryDir = _getValue(&package, "ENDLIBRARYDIRECTORY").front();
          }
          else if(!line.compare("LIBRARIES"))
          {
            libraries = _getValue(&package, "ENDLIBRARIES");
          }
          else if(!line.compare("ENDDEFINITION"))
          {
            MMO_FunctionDefinition fi = newMMO_FunctionDefinition(fname, includeDir,
                libraryDir, libraries);
            fit->insert(fi);
          }
        }
        MMO_PackageData pd = newMMO_PackageData(fileName, fit, objects);
        package.close();
        return pd;
      }
      return NULL;
    }

    bool
    Util::readPackage(string fileName, MMO_PackageTable pt)
    {
      MMO_PackageData pd = readPackage(fileName);
      if(pd == NULL)
      {
        return false;
      }
      pt->insert(pd);
      return true;
    }*/

    bool
    Util::searchCompiledPackage(string pname, CompileFlags flags)
    {
      return !packagePath(packageName(pname), flags, ".moo").empty();
    }

    bool
    Util::_checkCodeFiles(string name, string ext)
    {
      if(!ext.compare(".mo"))
      {
        return true;
      }
      ifstream h((name + ".h").c_str());
      ifstream c((name + ".c").c_str());
      if(h.good() && c.good())
      {
        h.close();
        c.close();
        return true;
      }
      return false;
    }

    string
    Util::_packagePath(string name)
    {
      list<string> objs = _flags.objects();
      for(list<string>::iterator it = objs.begin(); it != objs.end(); it++)
      {
        string i = *it;
        string p = *it;
        size_t found = i.rfind("/");
        if(found != std::string::npos)
        {
          p.erase(p.begin() + found + 1, p.end());
          i.erase(i.begin(), i.begin() + found + 1);
        }
        if(!i.compare(name))
        {
          return p;
        }
      }
      return "";
    }

    string
    Util::packagePath(string pname, CompileFlags flags, string ext)
    {
      string fname = flags.path();
      fname.append(pname + ext);
      ifstream f(fname.c_str());
      if(f.good() && _checkCodeFiles(flags.path() + pname, ext))
      {
        f.close();
        return flags.path();
      }
      else
      {
        f.close();
        if(flags.hasOutputFile())
        {
          fname = flags.outputFilePath();
          fname.append(SLASH + pname + ext);
          f.open(fname.c_str());
          if(f.good()
              && _checkCodeFiles(flags.outputFilePath() + SLASH + pname, ext))
          {
            f.close();
            return flags.outputFilePath();
          }
          f.close();
        }
        list<string> paths = flags.libraryPaths();
        for(list<string>::iterator it = paths.begin(); it != paths.end(); it++)
        {
          fname = *it;
          fname.append(SLASH + pname + ext);
          f.open(fname.c_str());
          if(f.good() && _checkCodeFiles(*it + SLASH + pname, ext))
          {
            f.close();
            return *it;
          }
          f.close();
        }
      }
      return "";
    }

    string
    Util::packageName(string name)
    {
      size_t f = name.find("pkg_");
      if(f == string::npos)
      {
        return "pkg_" + name;
      }
      return name;
    }

    string
    Util::printInitialAssignment(VarInfo vi, string indent, string localVar)
    {
    /*  bool ic = _data->initialCode();
      _data->setInitialCode(true);
      VarSymbolTable vt = _data->symbols();
      MMO_Expression exp = newMMO_Expression(vi->exp(), _data);
      stringstream buffer;
      if(vi->hasAssignment() || vi->hasStartModifier())
      {
        buffer << vt->print(vi) << " = " << exp->print("") << ";";
      }
      else if(vi->hasEachModifier())
      {
        buffer << "for(" << localVar << " = " << vi->index().begin() << "; "
            << localVar << " <= " << vi->index().end() << ";" << localVar << "++)"
            << endl;
        buffer << indent << "{" << endl;
        buffer << indent << indent << vt->print(vi, localVar) << " = "
            << exp->print(localVar) << ";" << endl;
        buffer << indent << "}";
      }
      _data->setInitialCode(ic);
      delete exp;
      return buffer.str();*/
      return "";
    }

    string
    Util::environmentVariable(string ev)
    {
      char *li = getenv(ev.c_str());
      if(li != NULL)
      {
        return li;
      }
      return "";
    }

    string
    Util::getFilePath(string file)
    {
      string path = file;
      size_t found = path.rfind("/");
      if(found != std::string::npos)
      {
        path.erase(path.begin() + found + 1, path.end());
      }
      return path;
    }

    string
    Util::getFileName(string file)
    {
      string fn = file;
      size_t f = fn.rfind("/");
      if(f == std::string::npos)
      {
        return file;
      }
      fn.erase(fn.begin(), fn.begin() + f + 1);
      return fn;
    }

    BIF_NAMES
    Util::checkBuiltInReductionFunctions(string fname)
    {
      BIF_NAMES ret = checkBuiltInFunctions(fname);
      if(ret == BIF_SUM || ret == BIF_PRODUCT || ret == BIF_MIN || ret == BIF_MAX
          || ret == BIF_INNER_PRODUCT)
      {
        return ret;
      }
      return BIF_NONE;
    }

    list<string>
    BIF::generateCode(string variableMap, string variableIndex, list<VariableInterval> variableInterval, int expOrder)
    {
    /*  stringstream buffer;
      list<string> code;
      bool states = _hasStates(variableInterval);
      Index idx = _index(variableInterval);
      if(states)
      {
        buffer << "for(" << variableIndex << " = 0;" << variableIndex << " < "
            << expOrder << "; " << variableIndex << "++)";
        code.push_back(buffer.str());
        buffer.str("");
        code.push_back("{");
        code.push_back(_init(variableMap, variableIndex, variableInterval, states));
        code.push_back("}");
        buffer << "for(" << variableIndex << " = " << idx.begin() << "; "
            << variableIndex << " <= " << idx.end() << "; " << variableIndex
            << "++)";
        code.push_back(buffer.str());
        buffer.str("");
        code.push_back("{");
        for(int j = 0; j < expOrder; j++)
        {
          code.push_back(
              _reduce(variableMap, variableIndex, j, variableInterval, states));
        }
        code.push_back("}");

      }
      else
      {
        code.push_back(_init(variableMap, variableIndex, variableInterval, states));
        buffer << "for(" << variableIndex << " = " << idx.begin() << "; "
            << variableIndex << " <= " << idx.end() << "; " << variableIndex
            << "++)";
        code.push_back(buffer.str());
        buffer.str("");
        code.push_back("{");
        code.push_back(
            _reduce(variableMap, variableIndex, 0, variableInterval, states));
        code.push_back("}");
      }*/
      list<string> code;
      return code;
    }

    bool
    BIF::_hasStates(list<VariableInterval> variables)
    {
     /* list<VariableInterval>::iterator it;
      for(it = variables.begin(); it != variables.end(); it++)
      {
        VarInfo vi = _vt->lookup(it->name());
        if(vi == NULL)
        {
          Error::getInstance()->add(0,
          EM_IR | EM_VARIABLE_NOT_FOUND,
              ER_Fatal, "%s", it->name().c_str());
          return false;
        }
        if(vi->isState() || vi->isAlgebraic())
        {
          return true;
        }
      }*/
      return false;
    }

    void
    BIF::setSymbolTable(VarSymbolTable vt)
    {
      _vt = vt;
    }

    Index
    BIF::_index(list<VariableInterval> variables)
    {
      //VariableInterval vi = variables.front();
      return Index();
    }

    string
    BIF::variableName(VariableInterval vin)
    {
     /* VarInfo vi = _variableInfo(vin);
      string varStr = _vt->getTypePrefix();
      if(vi->isState())
      {
        varStr += "x";
      }
      else if(vi->isAlgebraic())
      {
        varStr += "alg";
      }
      else if(vi->isDiscrete())
      {
        varStr += "d";
      }
      else
      {
        varStr = "__PAR_" + vi->name();
      }*/
      return "";
    }

    string
    BIF::expressionOrderStr(int order, VariableInterval vin)
    {
    /*  VarInfo vi = _variableInfo(vin);
      stringstream expOrder;
      if(_vt->printEnvironment() != VST_CLASSIC_MODEL_FUNCTIONS)
      {
        expOrder << " * " << _expressionOrder << " + " << order;
      }
      string varStr = _vt->getTypePrefix() + "x";
      if(vi->isAlgebraic())
      {
        if(_vt->printEnvironment() == VST_CLASSIC_MODEL_FUNCTIONS)
        {
          expOrder << " * " << _expressionOrder << " + " << order;
        }
        varStr = _vt->getTypePrefix() + "alg";
      }
      return expOrder.str();*/
      return "";
    }

    VarSymbolTable
    BIF::symbolTable(VarSymbolTable vt)
    {
      return _vt;
    }

    string
    BIF::print(Index idx, string variableIndex)
    {
      //return idx.print(variableIndex, 0, false);
      return "";
    }

    BuiltInFunction::~BuiltInFunction()
    {
    }

    string
    BuiltInFunction::_reduce(string variableMap, string variableIndex, int variableOrder, list<VariableInterval> variableInterval, bool hasStates)
    {
      return "";
    }

    string
    BuiltInFunction::_init(string variableMap, string variableIndex, list<VariableInterval> variableInterval, bool hasStates)
    {
      return "";
    }

    BuiltInSumFunction::~BuiltInSumFunction()
    {
    }

    string
    BuiltInSumFunction::_reduce(string variableMap, string variableIndex, int variableOrder, list<VariableInterval> variableInterval, bool hasStates)
    {
     /* VariableInterval vin = variableInterval.front();
      string variablePrefix = BIF::variableName(vin);
      stringstream buffer;
      Index idx = vin.index();
      string expOrderStr = BIF::expressionOrderStr(variableOrder, vin);
      if(hasStates)
      {
        buffer << "\t" << variableMap << "[" << variableOrder << "] += "
            << variablePrefix << "[(" << print(idx, variableIndex) << ")"
            << expOrderStr << "];";
      }
      else
      {
        buffer << "\t" << variableMap << "[0] += " << variablePrefix << "["
            << print(idx, variableIndex) << "];";
      }
      return buffer.str();*/
      return "";
    }

    string
    BuiltInSumFunction::_init(string variableMap, string variableIndex, list<VariableInterval> variableInterval, bool hasStates)
    {
      stringstream buffer;
      if(hasStates)
      {
        buffer << "\t" << variableMap << "[" << variableIndex << "] = 0;";
      }
      else
      {
        buffer << "\t" << variableMap << "[0] = 0;";
      }
      return buffer.str();
    }

    BuiltInProductFunction::~BuiltInProductFunction()
    {
    }

    string
    BuiltInProductFunction::_reduce(string variableMap, string variableIndex, int variableOrder, list<VariableInterval> variableInterval, bool hasStates)
    {
    /*  VariableInterval vin = variableInterval.front();
      string variablePrefix = BIF::variableName(vin);
      stringstream buffer;
      Index idx = vin.index();
      string expOrderStr = BIF::expressionOrderStr(variableOrder, vin);
      if(hasStates)
      {
        buffer << "\t" << variableMap << "[" << variableOrder << "] *= "
            << variablePrefix << "[(" << print(idx, variableIndex) << ")"
            << expOrderStr << "];";
      }
      else
      {
        buffer << "\t" << variableMap << "[0] *= " << variablePrefix << "["
            << print(idx, variableIndex) << "];";
      }
      return buffer.str();*/
      return "";
    }

    string
    BuiltInProductFunction::_init(string variableMap, string variableIndex, list<VariableInterval> variableInterval, bool hasStates)
    {
      stringstream buffer;
      if(hasStates)
      {
        buffer << "\t" << variableMap << "[" << variableIndex << "] = 0;";
      }
      else
      {
        buffer << "\t" << variableMap << "[0] = 0;";
      }
      return buffer.str();
    }

    BuiltInInnerProductFunction::~BuiltInInnerProductFunction()
    {
    }

    string
    BuiltInInnerProductFunction::_reduce(string variableMap, string variableIndex, int variableOrder, list<VariableInterval> variableInterval, bool hasStates)
    {
     /* if(variableInterval.size() != 2)
      {
        Error::getInstance()->add(0,
        EM_IR | EM_ARGUMENTS,
            ER_Fatal, "");
      }
      VariableInterval vin[2];
      list<VariableInterval>::iterator it;
      int i = 0;
      for(it = variableInterval.begin(); it != variableInterval.end(); it++)
      {
        vin[i++] = *it;
      }
      Index idx0 = vin[0].index();
      Index idx1 = vin[1].index();
      stringstream buffer;
      string variable0Prefix = BIF::variableName(vin[0]);
      string variable1Prefix = BIF::variableName(vin[1]);
      if(hasStates)
      {
        if(BIF::isState(vin[0]) && BIF::isState(vin[1]))
        {
          if(variableOrder == 0)
          {
            string expOrderStr = BIF::expressionOrderStr(variableOrder, vin[0]);
            buffer << "\t" << variableMap << "[" << variableOrder << "] += "
                << variable0Prefix << "[(" << print(idx0, variableIndex) << ")"
                << expOrderStr
                << "] * " << variable1Prefix << "[(" << print(idx1, variableIndex)
                << ")" << expOrderStr << "];";
          }
          else if(variableOrder == 1)
          {
            string expOrder00Str = BIF::expressionOrderStr(0, vin[0]);
            string expOrder01Str = BIF::expressionOrderStr(variableOrder, vin[0]);
            string expOrder10Str = BIF::expressionOrderStr(0, vin[1]);
            string expOrder11Str = BIF::expressionOrderStr(variableOrder, vin[1]);
            buffer << "\t" << variableMap << "[" << variableOrder << "] += "
                << variable0Prefix << "[(" << print(idx0, variableIndex) << ")"
                << expOrder01Str << " ]* " << variable1Prefix << "[("
                << print(idx1, variableIndex) << ")" << expOrder10Str << " ] + "
                << variable0Prefix << "[(" << print(idx0, variableIndex) << ")"
                << expOrder00Str << " ] * " << variable1Prefix << "[("
                << print(idx1, variableIndex) << ")" << expOrder11Str << "];";
          }
          else if(variableOrder == 2)
          {
            string expOrder00Str = BIF::expressionOrderStr(0, vin[0]);
            string expOrder01Str = BIF::expressionOrderStr(1, vin[0]);
            string expOrder02Str = BIF::expressionOrderStr(2, vin[0]);
            string expOrder10Str = BIF::expressionOrderStr(0, vin[1]);
            string expOrder11Str = BIF::expressionOrderStr(1, vin[1]);
            string expOrder12Str = BIF::expressionOrderStr(2, vin[1]);
            buffer << "\t" << variableMap << "[" << variableOrder << "] += "
                << variable0Prefix << "[(" << print(idx0, variableIndex) << ")"
                << expOrder02Str << "] * " << variable1Prefix << "[("
                << print(idx1, variableIndex) << ")" << expOrder10Str << "] + 2 * "
                << variable0Prefix << "[(" << print(idx0, variableIndex) << ")"
                << expOrder01Str << " ] * " << variable1Prefix << "[("
                << print(idx1, variableIndex) << ")" << expOrder11Str << "] + "
                << variable0Prefix << "[(" << print(idx0, variableIndex) << ")"
                << expOrder00Str << "] * " << variable1Prefix << "[("
                << print(idx1, variableIndex) << ")" << expOrder12Str << "];";
          }
          else if(variableOrder == 3)
          {
            string expOrder00Str = BIF::expressionOrderStr(0, vin[0]);
            string expOrder01Str = BIF::expressionOrderStr(1, vin[0]);
            string expOrder02Str = BIF::expressionOrderStr(2, vin[0]);
            string expOrder03Str = BIF::expressionOrderStr(3, vin[0]);
            string expOrder10Str = BIF::expressionOrderStr(0, vin[1]);
            string expOrder11Str = BIF::expressionOrderStr(1, vin[1]);
            string expOrder12Str = BIF::expressionOrderStr(2, vin[1]);
            string expOrder13Str = BIF::expressionOrderStr(3, vin[1]);
            buffer << "\t" << variableMap << "[" << variableOrder << "] += "
                << variable0Prefix << "[(" << print(idx0, variableIndex) << ")"
                << expOrder03Str << "] * " << variable1Prefix << "[("
                << print(idx1, variableIndex) << ")" << expOrder10Str << "] + 3 * "
                << variable0Prefix << "[(" << print(idx0, variableIndex) << ")"
                << expOrder02Str << "] * " << variable1Prefix << "[("
                << print(idx1, variableIndex) << ")" << expOrder11Str << "] + 3 * "
                << variable0Prefix << "[(" << print(idx0, variableIndex) << ")"
                << expOrder01Str << "] * " << variable1Prefix << "[("
                << print(idx1, variableIndex) << ")" << expOrder12Str << "] + "
                << variable0Prefix
                << "[(" << print(idx0, variableIndex) << ")" << expOrder00Str
                << "] * " << variable1Prefix << "[(" << print(idx1, variableIndex)
                << ")"
                << expOrder13Str << "];";
          }
          else if(variableOrder == 4)
          {
            string expOrder00Str = BIF::expressionOrderStr(0, vin[0]);
            string expOrder01Str = BIF::expressionOrderStr(1, vin[0]);
            string expOrder02Str = BIF::expressionOrderStr(2, vin[0]);
            string expOrder03Str = BIF::expressionOrderStr(3, vin[0]);
            string expOrder04Str = BIF::expressionOrderStr(4, vin[0]);
            string expOrder10Str = BIF::expressionOrderStr(0, vin[1]);
            string expOrder11Str = BIF::expressionOrderStr(1, vin[1]);
            string expOrder12Str = BIF::expressionOrderStr(2, vin[1]);
            string expOrder13Str = BIF::expressionOrderStr(3, vin[1]);
            string expOrder14Str = BIF::expressionOrderStr(4, vin[1]);
            buffer << "\t" << variableMap << "[" << variableOrder << "] += "
                << variable0Prefix << "[(" << print(idx0, variableIndex) << ")"
                << expOrder04Str << "] * " << variable1Prefix << "[("
                << print(idx1, variableIndex) << ")" << expOrder10Str << "] + 4 * "
                << variable0Prefix << "[(" << print(idx0, variableIndex) << ")"
                << expOrder03Str << "] * " << variable1Prefix << "[("
                << print(idx1, variableIndex) << ")" << expOrder11Str << "] + 6 * "
                << variable0Prefix << "[(" << print(idx0, variableIndex) << ")"
                << expOrder02Str << "] * " << variable1Prefix << "[("
                << print(idx1, variableIndex) << ")" << expOrder12Str << "] + 4 * "
                << variable0Prefix << "[(" << print(idx0, variableIndex) << ")"
                << expOrder01Str << "] * " << variable1Prefix << "[("
                << print(idx1, variableIndex) << ")" << expOrder13Str << "] + "
                << variable0Prefix << "[(" << print(idx0, variableIndex) << ")"
                << expOrder00Str << "] * " << variable1Prefix << "[("
                << print(idx1, variableIndex) << ")" << expOrder14Str << "];";
          }
        }
        else if(BIF::isState(vin[0]) && !BIF::isState(vin[1]))
        {
          string expOrderStr = BIF::expressionOrderStr(variableOrder, vin[0]);
          buffer << "\t" << variableMap << "[" << variableOrder << "] += "
              << variable0Prefix << "[(" << print(idx0, variableIndex) << ")"
              << expOrderStr
              << "] * " << variable1Prefix << "[" << print(idx1, variableIndex)
              << "];";

        }
        else if(!BIF::isState(vin[0]) && BIF::isState(vin[1]))
        {
          string expOrderStr = BIF::expressionOrderStr(variableOrder, vin[1]);
          buffer << "\t" << variableMap << "[" << variableOrder << "] += "
              << variable0Prefix << "[" << print(idx0, variableIndex) << "]" << "*"
              << variable1Prefix << "[(" << print(idx1, variableIndex) << ")"
              << expOrderStr << "];";
        }
      }
      else
      {
        buffer << "\t" << variableMap << "[0] += " << variable0Prefix << "["
            << print(idx0, variableIndex) << "] * " << variable1Prefix << "["
            << print(idx1, variableIndex) << "];";
      }
      return buffer.str();*/
        return "";
    }

    string
    BuiltInInnerProductFunction::_init(string variableMap, string variableIndex, list<VariableInterval> variableInterval, bool hasStates)
    {
      stringstream buffer;
      if(hasStates)
      {
        buffer << "\t" << variableMap << "[" << variableIndex << "] = 0;";
      }
      else
      {
        buffer << "\t" << variableMap << "[0] = 0;";
      }
      return buffer.str();
    }

    BuiltInMinFunction::~BuiltInMinFunction()
    {
    }

    string
    BuiltInMinFunction::_reduce(string variableMap, string variableIndex, int variableOrder, list<VariableInterval> variableInterval, bool hasStates)
    {
      /*stringstream buffer;
      VariableInterval vin = variableInterval.front();
      string expOrder = BIF::expressionOrderStr(0, vin);
      string variablePrefix = BIF::variableName(vin);
      Index idx = vin.index();
      if(hasStates)
      {
        buffer << "\t if (" << variableMap << "[" << variableOrder << "] > "
            << variablePrefix << "[(" << print(idx, variableIndex) << ")"
            << expOrder << "])"
            << endl;
        buffer << "\t \t" << variableMap << "[" << variableOrder << "] = "
            << variablePrefix << "[(" << print(idx, variableIndex) << ")"
            << expOrder << "];";
      }
      else
      {
        buffer << "\t if (" << variableMap << "[0] > " << variablePrefix << "["
            << print(idx, variableIndex) << "])" << endl;
        buffer << "\t \t" << variableMap << "[0] = " << variablePrefix << "["
            << print(idx, variableIndex) << "];";
      }
      return buffer.str();*/
      return "";
    }

    string
    BuiltInMinFunction::_init(string variableMap, string variableIndex, list<VariableInterval> variableInterval, bool hasStates)
    {
      /*stringstream buffer;
      VariableInterval vin = variableInterval.front();
      string expOrder = BIF::expressionOrderStr(0, vin);
      string variablePrefix = BIF::variableName(vin);
      Index idx = vin.index();
      if(hasStates)
      {
        buffer << "\t" << variableMap << "[" << variableIndex << "] = "
            << variablePrefix << "[" << idx.offset() << expOrder << "];";
      }
      else
      {
        buffer << "\t" << variableMap << "[0] = " << variablePrefix << "[0];";
      }
      return buffer.str();*/
      return "";
    }

    BuiltInMaxFunction::~BuiltInMaxFunction()
    {
    }

    string
    BuiltInMaxFunction::_reduce(string variableMap, string variableIndex, int variableOrder, list<VariableInterval> variableInterval, bool hasStates)
    {
      /*stringstream buffer;
      VariableInterval vin = variableInterval.front();
      string expOrder = BIF::expressionOrderStr(0, vin);
      string variablePrefix = BIF::variableName(vin);
      Index idx = vin.index();
      if(hasStates)
      {
        buffer << "\t if (" << variableMap << "[" << variableOrder << "] < "
            << variablePrefix << "[(" << print(idx, variableIndex) << ")"
            << expOrder << "])"
            << endl;
        buffer << "\t \t" << variableMap << "[" << variableOrder << "] = "
            << variablePrefix << "[(" << print(idx, variableIndex) << ")"
            << expOrder << "];";
      }
      else
      {
        buffer << "\t if (" << variableMap << "[0] < " << variablePrefix << "["
            << print(idx, variableIndex) << "])" << endl;
        buffer << "\t \t" << variableMap << "[0] = " << variablePrefix << "["
            << print(idx, variableIndex) << "];";
      }
      return buffer.str();*/
      return "";
    }

    string
    BuiltInMaxFunction::_init(string variableMap, string variableIndex, list<VariableInterval> variableInterval, bool hasStates)
    {
     /* stringstream buffer;
      VariableInterval vin = variableInterval.front();
      string expOrder = BIF::expressionOrderStr(0, vin);
      string variablePrefix = BIF::variableName(vin);
      Index idx = vin.index();
      if(hasStates)
      {
        buffer << "\t" << variableMap << "[" << variableIndex << "] = "
            << variablePrefix << "[" << idx.offset() << expOrder << "];";
      }
      else
      {
        buffer << "\t" << variableMap << "[0] = " << variablePrefix << "[0];";
      }
      return buffer.str();
      */
      return "";
    }

    void
    BIF::setExpressionOrder(int expressionOrder)
    {
      _expressionOrder = expressionOrder;
    }

    int
    BIF::expressionOrder()
    {
      return _expressionOrder;
    }

    VarInfo
    BIF::_variableInfo(VariableInterval vin)
    {
      /*VarInfo vi = _vt->lookup(vin.name());
      if(vi == NULL)
      {
        Error::getInstance()->add(0,
        EM_IR | EM_VARIABLE_NOT_FOUND,
            ER_Fatal, "%s", vin.name().c_str());
        return NULL;
      }
      return vi;*/
      return NULL;
    }

    bool
    BIF::isState(VariableInterval vin)
    {
      VarInfo vi = _variableInfo(vin);
      return vi->isState() || vi->isAlgebraic();
    }

    BIF*
    Util::builtInReductionFunctions(BIF_NAMES fn)
    {
      map<BIF_NAMES, BIF*>::iterator it = _builtInFunctionImp.find(fn);
      if(it != _builtInFunctionImp.end())
      {
        return it->second;
      }
      return new BuiltInFunction();
    }

    bool
    Util::checkGKLinkFunctions(string name)
    {
      map<string, BIF_NAMES>::iterator it = _builtInFunctions.find(name);
      return it != _builtInFunctions.end();
    }
  }
}
