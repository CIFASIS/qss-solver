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

#ifndef MMO_HELPERS_H
#define MMO_HELPERS_H 

#include <string>
#include "../util/ast_util.h"
#include "../util/symbol_table.h"
#include "../util/util.h"

namespace MicroModelica {
  namespace IR {
    
    /**
     * @breif Wrapper to external function calls inside function or model definitions.
     *
     */ 
    class ExternalFunction 
    {
      public:
        ExternalFunction() {};
        ExternalFunction(std::string lvalue, std::string name, AST_ExpressionList args, const Util::VarSymbolTable& symbols);
        ~ExternalFunction();
        friend std::ostream& operator<<(std::ostream& out, const ExternalFunction& e);
      private:
        std::string          _lvalue;
        std::string          _name;
        AST_ExpressionList   _args;
        Util::VarSymbolTable _symbols;
    };

    typedef ModelTable<int, ExternalFunction> ExternalFunctionTable;

    /**
     *
     */
    class CompiledFunction
    {
      public:
        /**
         *
         */
        CompiledFunction();
        /**
         *
         * @param name
         * @param includeDir
         * @param libraryDir
         * @param libraries
         */
        CompiledFunction(string name, string includeDir, string libraryDir, list<string> libraries);
        /**
         *
         */
        ~CompiledFunction();
        /**
         *
         * @return
         */
        list<string>
        def();
        /**
         *
         * @return
         */
        bool
        hasIncludeDirectory();
        /**
         *
         * @return
         */
        bool
        hasLibraryDirectory();
        /**
         *
         * @return
         */
        bool
        hasLibraries();
        /**
         *
         * @return
         */
        string
        includeDirectory();
        /**
         *
         * @return
         */
        string
        libraryDirectory();
        /**
         *
         * @return
         */
        list<string>
        libraries();
        /**
         *
         * @return
         */
        string
        name();
        /**
         *
         * @return
         */
        string
        prototype();
      private:
        list<string> _def;
        string _name;
        string _prototype;
        string _includeDirectory;
        string _libraryDirectory;
        list<string> _libraries;
    };
    
    typedef ModelTable<std::string,CompiledFunction> CompiledFunctionTable;

    /**
     *
     */
    class CompiledPackage
    {
      public:
        /**
         *
         */
        CompiledPackage();
        /**
         *
         * @param name
         * @param includeDir
         * @param libraryDir
         * @param libraries
         */
        CompiledPackage(string name, CompiledFunctionTable cft , Util::ImportTable objects);
        /**
         *
         */
        ~CompiledPackage();
        /**
         *
         * @return
         */
        string
        name();
        /**
         *
         * @return
         */
        string
        prefix();
        /**
         *
         * @return
         */
        list<string>
        linkLibraries();
        /**
         *
         * @return
         */
        list<string>
        includeDirectories();
        /**
         *
         * @return
         */
        list<string>
        libraryDirectories();
        /**
         *
         * @return
         */
        CompiledFunctionTable 
        definitions();
       /**
         *
         * @return
         */
        Util::ImportTable 
        objects();
      private:
        std::string             _name;
        CompiledFunctionTable   _cft;
        Util::ImportTable       _objects;
    };

    typedef ModelTable<std::string,CompiledPackage> CompiledPackageTable;


  }
}

#endif /* MMO_HELPERS_H */
