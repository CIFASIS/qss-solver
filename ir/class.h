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

#ifndef MMO_CLASS_H_
#define MMO_CLASS_H_

#include <list>
#include <map>
#include <string>
#include <vector>
#include <boost/variant/variant.hpp>


#include "../ast/ast_types.h"
#include "../util/util_types.h"
#include "../util/util.h"

namespace MicroModelica
{
  namespace IR
  {
    /**
    *
    */
    typedef enum
    {
      DEC_PUBLIC,  //!< DEC_PUBLIC
      DEC_LOCAL  //!< DEC_LOCAL
    } DEC_Type;


    /**
    *
    */
    class Class
    {
      public:
        Class(){};
        ~Class(){};
        /**
        *
        * @return
        */
        virtual string
        name() const = 0;
        /**
        *
        * @param n
        */
        virtual void
        insert(string n) const = 0;
        /**
        *
        * @param eq
        */
        virtual void
        insert(AST_Equation eq) const = 0;
        /**
        *
        * @param stm
        * @param initial
        */
        virtual void
        insert(AST_Statement stm, bool initial) const = 0;
        /**
        *
        * @param stm
        */
        virtual void
        insert(AST_Statement stm) const = 0;
        /**
        *
        * @param efc
        */
        virtual void
        insert(AST_External_Function_Call efc) const = 0;
        /**
        *
        * @param n
        * @param vi
        * @param type
        */
        virtual void
        insert(VarName n, MicroModelica::Util::VarInfo vi, DEC_Type type) const = 0;
        /**
        *
        * @param n
        * @param vi
        */
        virtual void
        insert(VarName n, MicroModelica::Util::VarInfo vi) const = 0;
        /**
        *
        * @param x
        */
        virtual void
        insert(AST_Argument_Modification x) const = 0;
        /**
        *
        * @return
        */
        virtual MicroModelica::Util::VarSymbolTable
        varTable() = 0;
        /**
        *
        * @return
        */
        virtual MicroModelica::Util::ImportTable
        imports() = 0;
    };
    
    /**
    *
    */
    class Function: public Class
    {
      public:
        /**
        *
        */
        Function() {};
        /**
        *
        * @param name
        */
        Function(string name);
        /**
        *
        */
        ~Function();
        /**
        *
        * @param c
        */
        void
        setFather(Class* c);
        /**
        *
        * @return
        */
        bool
        hasFather();
        /**
        *
        * @return
        */
        Class*
        father() const;
        /**
        *
        * @return
        */
        string
        name() const;
        /**
        *
        * @param efc
        */
        void
        insert(AST_External_Function_Call efc) const;
        /**
        *
        * @param n
        * @param vi
        * @param type
        */
        void
        insert(VarName n, MicroModelica::Util::VarInfo vi, DEC_Type type) const;
        /**
        *
        * @param n
        * @param vi
        */
        void
        insert(VarName n, MicroModelica::Util::VarInfo vi) const;
        /**
        *
        * @param eq
        */
        void
        insert(AST_Equation eq) const;
        /**
        *
        * @param stm
        * @param initial
        */
        void
        insert(AST_Statement stm, bool initial) const;
        /**
        *
        * @param stm
        */
        void
        insert(AST_Statement stm) const;
        /**
        *
        * @param n
        */
        void
        insert(string n) const;
        /**
        *
        * @param x
        */
        void
        insert(AST_Argument_Modification x) const;
        /**
        *
        * @return
        */
        MicroModelica::Util::VarSymbolTable
        varTable();
        /**
        *
        * @return
        */
        MicroModelica::Util::ImportTable
        imports();
      private:
        MicroModelica::Util::ImportTable    _imports;
        std::string                         _name;
        MicroModelica::Util::VarSymbolTable _symbols;
        MicroModelica::Util::VarSymbolTable _localSymbols;

    };
    
    typedef ModelTable<std::string,Function> FunctionTable;

    /**
    *
    */
    class Package: public Class
    {
      public:
        /**
        *
        */
        Package() {};
        /**
        *
        * @param name
        */
        Package(string name);
        /**
        *
        */
        ~Package();
        /**
        *
        * @return
        */
        MicroModelica::Util::VarSymbolTable
        varTable();
        /**
        *
        * @return
        */
        string
        name() const;
        /**
        *
        * @param n
        */
        void
        insert(string n) const;
        /**
        *
        * @param eq
        */
        void
        insert(AST_Equation eq) const;
        /**
        *
        * @param stm
        * @param initial
        */
        void
        insert(AST_Statement stm, bool initial) const;
        /**
        *
        * @param stm
        */
        void
        insert(AST_Statement stm) const;
        /**
        *
        * @param f
        */
        void
        insert(Function &f) const;
        /**
        *
        * @param efc
        */
        void
        insert(AST_External_Function_Call efc) const;
        /**
        *
        * @param n
        * @param vi
        * @param type
        */
        void
        insert(VarName n, MicroModelica::Util::VarInfo vi, DEC_Type type) const;
        /**
        *
        * @param n
        * @param vi
        */
        void
        insert(VarName n, MicroModelica::Util::VarInfo vi) const;
        /**
        *
        * @param x
        */
        void
        insert(AST_Argument_Modification x) const;
        /**
        *
        * @return
        */
        MicroModelica::Util::ImportTable
        imports();
        /**
          *
          * @return
        */
        FunctionTable 
        definitions();
        /*
          *
          * @return
        */
        std::string 
        fileName();
        /*
          *
          * @return 
        */
        std::string 
        prefix();
      private:
        MicroModelica::Util::ImportTable _imports;
        std::string                      _name;
        FunctionTable                    _functions;
    };

    /**
    *
    */
    class Model: public Class
    {
      public:
        /**
        *
        * @param name
        */
        Model() {};
        /**
        *
        * @param name
        */
        Model(string name);
        /**
        *
        */
        ~Model();
        /**
        *
        * @return
        */
        string
        name() const;
        /**
        *
        * @param n
        */
        void
        insert(string n) const;
        /**
        *
        * @param n
        * @param vi
        * @param type
        */
        void
        insert(VarName n, MicroModelica::Util::VarInfo vi, DEC_Type type) const;
        /**
        *
        * @param n
        * @param vi
        */
        void
        insert(VarName n, MicroModelica::Util::VarInfo vi) const;
        /**
        *
        * @param eq
        */
        void
        insert(AST_Equation eq) const;
        /**
        *
        * @param stm
        * @param initial
        */
        void
        insert(AST_Statement stm, bool initial) const;
        /**
        *
        * @param stm
        */
        void
        insert(AST_Statement stm) const;
        /**
        *
        * @param f
        */
        void
        insert(Function &f) const;
        /**
        *
        * @param efc
        */
        void
        insert(AST_External_Function_Call efc) const;
        /**
        *
        * @param x
        */
        void
        insert(AST_Argument_Modification x) const;
        /**
        *
        * @return
        */
        MicroModelica::Util::VarSymbolTable
        varTable();
        /**
        *
        * @return
        */
        MicroModelica::Util::ImportTable
        imports();
      private:
        std::string                        _name;
        MicroModelica::Util::ImportTable   _imports;
    };
    
    typedef boost::variant<
      Function,
      Package,
      Model 
      > ClassType;

    typedef const Class* ClassPtr;
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
        CompiledPackage(string name, CompiledFunctionTable cft , MicroModelica::Util::ImportTable objects);
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
        MicroModelica::Util::ImportTable 
        objects();
      private:
        string                            _name;
        CompiledFunctionTable             _cft;
        MicroModelica::Util::ImportTable  _objects;
    };

  }
}
#endif  /* MMO_CLASS_H_ */
