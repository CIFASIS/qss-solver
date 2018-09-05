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
#include "../ir/class.h"
#include "../ir/index.h"
#include "../ir/built_in_functions.h"
#include "compile_flags.h"
#include "error.h"
#include "symbol_table.h"

namespace MicroModelica {
  using namespace IR;
  namespace Util {

    Utils::Utils() :
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
      _builtInVariables.insert(pair<string, BuiltIn::Variable>("time", BuiltIn::Time));
      _builtInFunctions.insert(pair<string, BuiltIn::Function>("reinit", BuiltIn::REINIT));
      _builtInFunctions.insert(pair<string, BuiltIn::Function>("terminate", BuiltIn::TERMINATE));
      _builtInFunctions.insert(pair<string, BuiltIn::Function>("sum", BuiltIn::SUM));
      _builtInFunctions.insert( pair<string, BuiltIn::Function>("__INNER_PRODUCT", BuiltIn::INNER_PRODUCT));
      _builtInFunctions.insert(pair<string, BuiltIn::Function>("product", BuiltIn::PRODUCT));
      _builtInFunctions.insert(pair<string, BuiltIn::Function>("min", BuiltIn::MIN));
      _builtInFunctions.insert(pair<string, BuiltIn::Function>("max", BuiltIn::MAX));
      _builtInFunctions.insert(pair<string, BuiltIn::Function>("abs", BuiltIn::ABS));
      _builtInFunctions.insert(pair<string, BuiltIn::Function>("sign", BuiltIn::SIGN));
      _builtInFunctions.insert(pair<string, BuiltIn::Function>("sqrt", BuiltIn::SQRT));
      _builtInFunctions.insert(pair<string, BuiltIn::Function>("ceil", BuiltIn::CEIL));
      _builtInFunctions.insert(pair<string, BuiltIn::Function>("floor", BuiltIn::FLOOR));
      _builtInFunctions.insert(pair<string, BuiltIn::Function>("sin", BuiltIn::SIN));
      _builtInFunctions.insert(pair<string, BuiltIn::Function>("cos", BuiltIn::COS));
      _builtInFunctions.insert(pair<string, BuiltIn::Function>("tan", BuiltIn::TAN));
      _builtInFunctions.insert(pair<string, BuiltIn::Function>("asin", BuiltIn::ASIN));
      _builtInFunctions.insert(pair<string, BuiltIn::Function>("acos", BuiltIn::ACOS));
      _builtInFunctions.insert(pair<string, BuiltIn::Function>("atan", BuiltIn::ATAN));
      _builtInFunctions.insert(pair<string, BuiltIn::Function>("atan2", BuiltIn::ATAN2));
      _builtInFunctions.insert(pair<string, BuiltIn::Function>("sinh", BuiltIn::SINH));
      _builtInFunctions.insert(pair<string, BuiltIn::Function>("cosh", BuiltIn::COSH));
      _builtInFunctions.insert(pair<string, BuiltIn::Function>("tanh", BuiltIn::TANH));
      _builtInFunctions.insert(pair<string, BuiltIn::Function>("exp", BuiltIn::EXP));
      _builtInFunctions.insert(pair<string, BuiltIn::Function>("log", BuiltIn::LOG));
      _builtInFunctions.insert(pair<string, BuiltIn::Function>("log10", BuiltIn::LOG10));
      _builtInFunctions.insert(pair<string, BuiltIn::Function>("pre", BuiltIn::PRE));
      _builtInFunctions.insert(pair<string, BuiltIn::Function>("GQLink_GetB", BuiltIn::GQLINK));
      _builtInFunctions.insert(pair<string, BuiltIn::Function>("GQLink_GetBx", BuiltIn::GQLINK));
      _builtInFunctions.insert(pair<string, BuiltIn::Function>("GQLink_GetBy", BuiltIn::GQLINK));
      _builtInFunctions.insert(pair<string, BuiltIn::Function>("GQLink_GetBz", BuiltIn::GQLINK));
      _builtInFunctionImp.insert(pair<BuiltIn::Function, BIF*>(BuiltIn::SUM, new BuiltInSumFunction()));
      _builtInFunctionImp.insert(pair<BuiltIn::Function, BIF*>(BuiltIn::PRODUCT, new BuiltInProductFunction()));
      _builtInFunctionImp.insert(pair<BuiltIn::Function, BIF*>(BuiltIn::INNER_PRODUCT, new BuiltInInnerProductFunction()));
      _builtInFunctionImp.insert(pair<BuiltIn::Function, BIF*>(BuiltIn::MIN, new BuiltInMinFunction())); 
      _builtInFunctionImp.insert(pair<BuiltIn::Function, BIF*>(BuiltIn::MAX, new BuiltInMaxFunction()));
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

    Utils::~Utils()
    {
    }

    string
    Utils::trimString(string str)
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
    Utils::languageEspecification()
    {
      return _languageEspecification;
    }

    void
    Utils::setCompileFlags(CompileFlags flags)
    {
      _flags = flags;
    }

    bool
    Utils::checkTypeString(string t)
    {
      if(t != "Integer" && t != "Real" && t != "Boolean")
      {
        return false;
      }
      return true;
    }

    bool
    Utils::checkExperimentAnnotations(string *annotation)
    {
      map<string, int>::iterator it = _annotations.find(*annotation);
      return it != _annotations.end();
    }

    BuiltIn::Function
    Utils::checkBuiltInFunctions(string fname)
    {
      map<string, BuiltIn::Function>::iterator it = _builtInFunctions.find(fname);
      if(it != _builtInFunctions.end())
      {
        return it->second;
      }
      return BuiltIn::NONE;
    }

    BuiltIn::Variable
    Utils::checkBuiltInVariables(string fname)
    {
      map<string, BuiltIn::Variable>::iterator it = _builtInVariables.find(fname);
      if(it != _builtInVariables.end())
      {
        return it->second;
      }
      return BuiltIn::None;
    }

    void
    Utils::addBuiltInVariables(string fname, BuiltIn::Variable var)
    {
      _builtInVariables.insert(pair<string, BuiltIn::Variable>(fname, var));
    }

    string
    Utils::opString(BinOpType bot)
    {
      return _binop[bot];
    }

    string
    Utils::getVarName(string name)
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
    Utils::iteratorVar()
    {
      stringstream buffer;
      buffer << "_it" << _varCounter++;
      return buffer.str();
    }

    list<string>
    Utils::getValue(fstream *package, string token)
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

    Option<MicroModelica::IR::CompiledPackage>
    Utils::readPackage(string fileName)
    {
      fstream package;
      string pname = packageName(fileName);
      string name = packagePath(pname + ".c");
      name.append(pname).append(".moo");
      package.open(name.c_str());
      if(package.good())
      {
        string line;
        IR::CompiledFunctionTable cft;
        ImportTable objects;
        string fname;
        string derivative;
        string includeDir;
        string libraryDir;
        list<string> libraries;
        while(getline(package, line))
        {
          if(!line.compare("DEPENDENCES"))
          {
            list<string> objs = getValue(&package, "ENDDEPENDENCES");
            for(list<string>::iterator it = objs.begin(); it != objs.end(); ++it)
            {
              objects.insert(*it,*it);
            }
          }
          else if(!line.compare("FUNCTION"))
          {
            fname = getValue(&package, "ENDFUNCTION").front();
          }
          else if(!line.compare("DERIVATIVE"))
          {
            derivative = getValue(&package, "ENDDERIVATIVE").front();
          }
          else if(!line.compare("INCLUDEDIRECTORY"))
          {
            includeDir = getValue(&package, "ENDINCLUDEDIRECTORY").front();
          }
          else if(!line.compare("LIBRARYDIRECTORY"))
          {
            libraryDir = getValue(&package, "ENDLIBRARYDIRECTORY").front();
          }
          else if(!line.compare("LIBRARIES"))
          {
            libraries = getValue(&package, "ENDLIBRARIES");
          }
          else if(!line.compare("ENDDEFINITION"))
          {
            IR::CompiledFunction fi(fname, includeDir, libraryDir, libraries);
            cft.insert(fname, fi);
          }
        }
        package.close();
        return Option<IR::CompiledPackage>(IR::CompiledPackage (fileName, cft, objects));
      }
      return Option<IR::CompiledPackage>();
    }

    bool
    Utils::readPackage(string fileName, IR::CompiledPackageTable& pt)
    {
      Option<IR::CompiledPackage> cp = readPackage(fileName);
      if(!cp)
      {
        return false;
      }
      pt.insert(cp.get().name(),cp.get());
      return true;
    }

    bool
    Utils::searchCompiledPackage(string pname, CompileFlags flags)
    {
      return !packagePath(packageName(pname), flags, ".moo").empty();
    }

    bool
    Utils::checkCodeFiles(string name, string ext)
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
    Utils::packagePath(string name)
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
    Utils::packagePath(string pname, CompileFlags flags, string ext)
    {
      string fname = flags.path();
      fname.append(pname + ext);
      ifstream f(fname.c_str());
      if(f.good() && checkCodeFiles(flags.path() + pname, ext))
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
          if(f.good() && checkCodeFiles(flags.outputFilePath() + SLASH + pname, ext))
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
          if(f.good() && checkCodeFiles(*it + SLASH + pname, ext))
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
    Utils::packageName(string name)
    {
      size_t f = name.find("pkg_");
      if(f == string::npos)
      {
        return "pkg_" + name;
      }
      return name;
    }

    string
    Utils::environmentVariable(string ev)
    {
      char *li = getenv(ev.c_str());
      if(li != NULL)
      {
        return li;
      }
      return "";
    }

    string
    Utils::getFilePath(string file)
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
    Utils::getFileName(string file)
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

    BuiltIn::Function
    Utils::checkBuiltInReductionFunctions(string fname)
    {
      BuiltIn::Function ret = checkBuiltInFunctions(fname);
      if(ret == BuiltIn::SUM || ret == BuiltIn::PRODUCT || ret == BuiltIn::MIN || ret == BuiltIn::MAX
          || ret == BuiltIn::INNER_PRODUCT)
      {
        return ret;
      }
      return BuiltIn::NONE;
    }


    BIF*
    Utils::builtInReductionFunctions(BuiltIn::Function fn)
    {
      map<BuiltIn::Function, BIF*>::iterator it = _builtInFunctionImp.find(fn);
      if(it != _builtInFunctionImp.end())
      {
        return it->second;
      }
      return new BuiltInFunction();
    }

    bool
    Utils::checkGKLinkFunctions(string name)
    {
      map<string, BuiltIn::Function>::iterator it = _builtInFunctions.find(name);
      return it != _builtInFunctions.end();
    }
  }
}
