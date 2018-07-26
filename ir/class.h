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
#include <boost/variant/variant_fwd.hpp>


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
        insert(string n) = 0;
        /**
        *
        * @param eq
        */
        virtual void
        insert(AST_Equation eq) = 0;
        /**
        *
        * @param stm
        * @param initial
        */
        virtual void
        insert(AST_Statement stm, bool initial) = 0;
        /**
        *
        * @param stm
        */
        virtual void
        insert(AST_Statement stm) = 0;
        /**
        *
        * @param efc
        */
        virtual void
        insert(AST_External_Function_Call efc) = 0;
        /**
        *
        * @param n
        * @param vi
        * @param type
        */
        virtual void
        insert(VarName n, VarInfo vi, DEC_Type type) = 0;
        /**
        *
        * @param n
        * @param vi
        */
        virtual void
        insert(VarName n, VarInfo vi) = 0;
        /**
        *
        * @param x
        */
        virtual void
        insert(AST_Argument_Modification x) = 0;
        /**
        *
        * @return
        */
        virtual VarSymbolTable
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
        insert(AST_External_Function_Call efc);
        /**
        *
        * @param n
        * @param vi
        * @param type
        */
        void
        insert(VarName n, VarInfo vi, DEC_Type type);
        /**
        *
        * @param n
        * @param vi
        */
        void
        insert(VarName n, VarInfo vi);
        /**
        *
        * @param eq
        */
        void
        insert(AST_Equation eq);
        /**
        *
        * @param stm
        * @param initial
        */
        void
        insert(AST_Statement stm, bool initial);
        /**
        *
        * @param stm
        */
        void
        insert(AST_Statement stm);
        /**
        *
        * @param n
        */
        void
        insert(string n);
        /**
        *
        * @param x
        */
        void
        insert(AST_Argument_Modification x);
        /**
        *
        * @return
        */
        VarSymbolTable
        varTable();
        /**
        *
        * @return
        */
        MicroModelica::Util::ImportTable
        imports();
    };

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
        VarSymbolTable
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
        insert(string n);
        /**
        *
        * @param eq
        */
        void
        insert(AST_Equation eq);
        /**
        *
        * @param stm
        * @param initial
        */
        void
        insert(AST_Statement stm, bool initial);
        /**
        *
        * @param stm
        */
        void
        insert(AST_Statement stm);
        /**
        *
        * @param f
        */
        void
        insert(Function &f);
        /**
        *
        * @param efc
        */
        void
        insert(AST_External_Function_Call efc);
        /**
        *
        * @param n
        * @param vi
        * @param type
        */
        void
        insert(VarName n, VarInfo vi, DEC_Type type);
        /**
        *
        * @param n
        * @param vi
        */
        void
        insert(VarName n, VarInfo vi);
        /**
        *
        * @param x
        */
        void
        insert(AST_Argument_Modification x);
        /**
        *
        * @return
        */
        MicroModelica::Util::ImportTable
        imports();
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
        insert(string n);
        /**
        *
        * @param n
        * @param vi
        * @param type
        */
        void
        insert(VarName n, VarInfo vi, DEC_Type type);
        /**
        *
        * @param n
        * @param vi
        */
        void
        insert(VarName n, VarInfo vi);
        /**
        *
        * @param eq
        */
        void
        insert(AST_Equation eq);
        /**
        *
        * @param stm
        * @param initial
        */
        void
        insert(AST_Statement stm, bool initial);
        /**
        *
        * @param stm
        */
        void
        insert(AST_Statement stm);
        /**
        *
        * @param f
        */
        void
        insert(Function &f);
        /**
        *
        * @param efc
        */
        void
        insert(AST_External_Function_Call efc);
        /**
        *
        * @param x
        */
        void
        insert(AST_Argument_Modification x);
        /**
        *
        * @return
        */
        VarSymbolTable
        varTable();
        /**
        *
        * @return
        */
        MicroModelica::Util::ImportTable
        imports();
    };
    
    typedef boost::variant<
      Function,
      Package,
      Model 
      > ClassType;
  }
}
#endif  /* MMO_CLASS_H_ */
