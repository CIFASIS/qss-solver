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

#ifndef UTIL_H_
#define UTIL_H_

#include <fstream>
#include <list>
#include <map>
#include <string>

#include "../ast/ast_types.h"
#include "../ir/built_in_functions.h"
#include "util_types.h"
#include "compile_flags.h"
#include "table.h"

namespace MicroModelica {
  namespace IR {
    class CompiledPackage;
    typedef ModelTable<std::string,CompiledPackage> CompiledPackageTable;
  }
  namespace Util {

    typedef ModelTable<string, string> ImportTable;
    
    /**
     *
     */
#define SLASH "/"
    /**
     *
     */
#define 	BINOPS 18

#if __x86_64__
    typedef long int grp_t;
#else
    typedef int grp_t;
#endif

#define TAB "  "


    /**
     *
     */
    class Utils
    {
      public:
        /**
         *
         * @return
         */
        static Utils&
        instance()
        {
          static Utils _instance;
          return _instance;
        }
        /**
         *
         */
        ~Utils();
        string
        trimString(string str);
        /**
         *
         * @return
         */
        string
        languageEspecification();
        /**
         *
         * @param t
         * @return
         */
        bool
        checkTypeString(string t);
        /**
         *
         * @param annotation
         * @return
         */
        bool
        checkExperimentAnnotations(string *annotation);
        /**
         *
         * @param fname
         * @return
         */
        IR::BuiltIn::Function 
        checkBuiltInFunctions(string fname);
        /**
         *
         * @param fname
         * @return
         */
        IR::BuiltIn::Function 
        checkBuiltInReductionFunctions(string fname);
        /**
         *
         * @param fname
         * @return
         */
        IR::BuiltIn::Variable
        checkBuiltInVariables(string fname);
        /**
         *
         * @param fname
         * @param type
         */
        void
        addBuiltInVariables(string fname, IR::BuiltIn::Variable type);
        /**
         *
         * @param bot
         * @return
         */
        string
        opString(BinOpType bot);
        /**
         *
         * @param n
         * @param vt
         * @return
         */
        string
        iteratorVar();
        /**
         *
         * @param name
         * @return
         */
        string
        getVarName(string name);
        /**
         *
         * @param fileName
         * @param pt
         * @return
         */
        bool
        readPackage(string fileName, IR::CompiledPackageTable pt);
        /**
         *
         * @param fileName
         * @return
         */
        Option<MicroModelica::IR::CompiledPackage> 
        readPackage(string fileName);
        /**
         *
         * @param pname
         * @param flags
         * @return
         */
        bool
        searchCompiledPackage(string pname, CompileFlags flags);
        /**
         *
         * @param pname
         * @param flags
         * @param ext
         * @return
         */
        string
        packagePath(string pname, CompileFlags flags, string ext = ".mo");
        /**
         *
         * @param flags
         */
        void
        setCompileFlags(CompileFlags flags);
        /**
         *
         * @param name
         * @return
         */
        string
        packageName(string name);
        /**
         *
         * @param vi
         * @param indent
         * @param localVar
         * @return
         */
        string
        printInitialAssignment(Variable vi, string indent, string localVar = "i0");
        /**
         *
         * @param ev
         * @return
         */
        string
        environmentVariable(string ev);
        /**
         *
         * @param file
         * @return
         */
        string
        getFilePath(string file);
        /**
         *
         * @param file
         * @return
         */
        string
        getFileName(string file);
        IR::BIF*
        builtInReductionFunctions(IR::BuiltIn::Function fn);
        bool
        checkGKLinkFunctions(string name);
      private:
        Utils();
        bool
        checkCodeFiles(string name, string ext);
        list<string>
        getValue(fstream *package, string token);
        string
        packagePath(string name);
        string                            _languageEspecification;
        int                               _varCounter;
        CompileFlags                      _flags;
        map<string, IR::BuiltIn::Function>    _builtInFunctions;
        map<string, IR::BuiltIn::Variable>    _builtInVariables;
        map<IR::BuiltIn::Function, IR::BIF*>  _builtInFunctionImp;
        map<string, int>                  _annotations;
        string                            _binop[BINOPS];
    };

  }
}
#endif  /* UTIL_H_ */
