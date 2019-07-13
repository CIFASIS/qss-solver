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
#include "../deps/model_dependencies.h"
#include "util_types.h"
#include "symbol_table.h"
#include "compile_flags.h"
#include "table.h"

namespace MicroModelica {
  namespace IR {
    class Equation;
    class CompiledPackage;
    class CompiledFunction;
    typedef ModelTable<std::string,CompiledPackage> CompiledPackageTable;
    typedef ModelTable<std::string,CompiledFunction> CompiledFunctionTable;
    typedef ModelTable<int,Equation> EquationTable;
  }
  namespace Util {

    typedef ModelTable<string, string> ImportTable;
    
    typedef ModelTable<string, string> SymbolTable;

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
        readPackage(string fileName, IR::CompiledPackageTable& pt);
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
        bool 
        checkGKLinkFunctions(std::string name) { return false; };
        IR::CompiledFunctionTable
        compiledFunctions();
        void 
        addCompiledFunction(IR::CompiledFunction f);
        void 
        addCompiledFunctions(IR::CompiledFunctionTable fs);
        bool 
        checkBuiltInFunctions(std::string name);
        inline VarSymbolTable& 
        symbols() { return _symbols; };
        inline SymbolTable 
        localSymbols() { return _localSymbols; };
        inline void  
        addLocalSymbol(std::string symbol) { _localSymbols.insert(symbol, symbol); };
        inline void 
        setSymbols(const VarSymbolTable& symbols) { _symbols = symbols; };
        inline void 
        setLocalSymbols() { _localSymbols.clear(); };
        inline std::string 
        fileName() { return _fileName; };
      private:
        Utils();
        bool
        checkCodeFiles(string name, string ext);
        SymbolTable
        getValue(fstream *package, string token);
        string
        packagePath(string name);
        string                            _languageEspecification;
        int                               _varCounter;
        CompileFlags                      _flags;
        map<string, int>                  _annotations;
        string                            _binop[BINOPS];
        IR::CompiledFunctionTable         _compiledFunctions;
        VarSymbolTable                    _symbols;
        SymbolTable                       _localSymbols;
        std::string                       _fileName;
    };

  }
}

#endif  /* UTIL_H_ */
