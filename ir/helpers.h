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
#include "index.h"
#include "../util/symbol_table.h"
#include "../util/util.h"
#include "../util/dependency_matrix.h"

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
        ~ExternalFunction() {};
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
        CompiledFunction(std::string name, std::string includeDir, std::string libraryDir, Util::SymbolTable& libraries, std::string prefix = "");
        /**
         *
         */
        ~CompiledFunction() {};
        /**
         *
         * @return
         */
        inline bool
        hasIncludeDirectory() const { return !_includeDirectory.empty(); };
        /**
         *
         * @return
         */
        inline bool
        hasLibraryDirectory() const { return !_libraryDirectory.empty(); };
        /**
         *
         * @return
         */
        inline bool
        hasLibraries() const { return _libraries.size() > 0; };
        /**
         *
         * @return
         */
        inline string
        includeDirectory() const { return _includeDirectory; };
        /**
         *
         * @return
         */
        inline string
        libraryDirectory() const { return _libraryDirectory; };
        /**
         *
         * @return
         */
        inline Util::SymbolTable 
        libraries() { return _libraries; };
        /**
         *
         * @return
         */
        inline string
        name() const { return _name; };
        /**
         *
         * @return
         */
        inline string
        prototype() const { return _prototype; };
        friend std::ostream& operator<<(std::ostream& out, const CompiledFunction& cf);
        std::string 
        print() const;
        void 
        setArguments(AST_ExpressionList arguments) { _arguments = arguments; };
        std::string 
        code();
      private:
        string                                _name;
        string                                _prototype;
        string                                _includeDirectory;
        string                                _libraryDirectory;
        Util::SymbolTable                     _libraries;
        AST_ExpressionList                    _arguments;
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
        CompiledPackage(string name);
        /**
         *
         */
        ~CompiledPackage() {};
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
        Util::SymbolTable  
        linkLibraries();
        /**
         *
         * @return
         */
        Util::SymbolTable  
        includeDirectories();
        /**
         *
         * @return
         */
        Util::SymbolTable  
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
        inline void 
        setObjects(Util::ImportTable& objects) { _objects = objects; };
        inline void 
        setDefinitions(CompiledFunctionTable& cft) { _cft = cft; };
      private:
        std::string             _name;
        CompiledFunctionTable   _cft;
        Util::ImportTable       _objects;
    };

    typedef ModelTable<std::string,CompiledPackage> CompiledPackageTable;

    class FunctionPrinter
    {
      public:
        FunctionPrinter() {};
        ~FunctionPrinter() {};
        std::string 
        beginSwitch();
        std::string 
        endSwitch();
        std::string 
        beginExpression(std::string token, Option<Range> range) const;
        std::string 
        endExpression(Option<Range> range) const;
        std::string
        algebraics(Util::EquationDependencyMatrix eqdm, Util::depId key);
        std::string 
        macro(std::string token, Option<Range> range, int id, int offset = 0) const;
      private:
        Range range;
        std::string 
        mod(std::string var, int dim) const;
        std::string 
        div(std::string var, int dim) const;
    };

  }
}

#endif /* MMO_HELPERS_H */
