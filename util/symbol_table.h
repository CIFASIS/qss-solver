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

#ifndef SYMBOL_TABLE_H_
#define SYMBOL_TABLE_H_

#include <functional>
#include <iostream>
#include <list>
#include <map>
#include <string>
#include <vector>

#include "../ast/ast_types.h"
#include "../ast/element.h"
#include "../ast/modification.h"
#include "../ir/mmo_util.h"
#include "index.h"
#include "type.h"
#include "table.h"

namespace MicroModelica {
  namespace Util {


    /**
     *
     */
    class Variable 
    {
      public:
        /**
         *
         * @param t
         * @param tp
         * @param m
         * @param c
         */
        Variable(Type t, AST_TypePrefix tp, AST_Modification m, AST_Comment c);
        /**
         *
         * @param t
         * @param tp
         * @param m
         * @param s
         */
        Variable(Type t, AST_TypePrefix tp, AST_Modification m, AST_Comment c, vector<int> s, bool array);
        /**
         *
         * @return
         */
        AST_TypePrefix
        typePrefix()
        {
          return _tp;
        };
        /**
         *
         * @return
         */
        AST_Comment
        comment()
        {
          return _comm;
        };
        /**
         *
         * @param c
         */
        void
        setComment(AST_Comment c)
        {
          _comm = c;
        };
        /**
         *
         * @return
         */
        AST_Modification
        modification()
        {
          return _m;
        };
        /**
         *
         * @param m
         */
        void
        setModification(AST_Modification m)
        {
          _m = m;
          _processModification();
        };
        /**
         *
         * @return
         */
        Type
        type();
        /**
         *
         * @param t
         */
        void
        setType(Type t)
        {
          _t = t;
        };
        /**
         *
         */
        void
        setParameter();
        /**
         *
         * @return
         */
        bool
        isParameter() const
        {
          return _tp & TP_PARAMETER;
        };
        /**
         *
         * @return
         */
        bool
        isDiscrete() const
        {
          return (_tp & TP_DISCRETE) || _discrete;
        };
        /**
         *
         * @return
         */
        bool
        builtIn() const
        {
          return _builtin;
        };
        /**
         *
         */
        void
        setDiscrete();
        /**
         *
         */
        void
        setBuiltIn()
        {
          _builtin = true;
        };
        /**
         *
         * @return
         */
        bool
        isConstant() const
        {
          return _tp & TP_CONSTANT;
        };
        /**
         *
         * @return
         */
        bool
        isInput() const
        {
          return _tp & TP_INPUT;
        };
        /**
         *
         * @return
         */
        bool
        isOutput() const
        {
          return _tp & TP_OUTPUT;
        };
        /**
         *
         * @return
         */
        bool
        isForType() const
        {
          return _tp & TP_FOR;
        };
        /**
         *
         * @return
         */
        bool
        isState();
        /**
         *
         */
        void
        setState();
        /**
         *
         * @return
         */
        bool
        isUnknown();
        /**
         *
         */
        void
        setUnknown();
        /**
         *
         * @return
         */
        bool
        isTime();
        /**
         *
         * @return
         */
        bool
        isAlgebraic();
        /**
         *
         */
        void
        setAlgebraic();
        /**
         *
         * @param val
         */
        void
        setValue(int val);
        /**
         *
         * @return
         */
        int
        value();
        /**
         *
         * @return
         */
        int
        size();
        /**
         *
         * @return
         */
        bool
        hasAssignment();
        /**
         *
         * @return
         */
        bool
        hasStartModifier();
        /**
         *
         * @return
         */
        bool
        hasEachModifier();
        void
        setEachModifier(bool each);
        /**
         *
         * @return
         */
        string
        name();
        /**
         *
         * @param n
         */
        void
        setName(string n);
        /**
         *
         * @return
         */
        AST_Expression
        exp();
        /**
         *
         * @return
         */
        bool
        isArray();
        /**
         *
         * @param os
         * @param e
         * @return
         */
        friend ostream &
        operator<<(ostream &os, const Variable &e);
        /**
         *
         * @param dim
         * @return
         */
        int
        size(int dim);
        /**
         *
         * @return
         */
        int
        dimensions();
        std::string  
        declaration();
        std::string 
        initialization();
      private:
        void
        _processModification();
        void
        _unsetAssignment();
        void
        _unsetStartEach();
        bool              _state;
        bool              _unknown;
        bool              _discrete;
        Type              _t;
        AST_TypePrefix    _tp;
        AST_Modification  _m;
        AST_Comment       _comm;
        bool              _builtin;
        vector<int>       _size;
        int               _value;
        bool              _algebraic;
        AST_Expression    _exp;
        bool              _hasStart;
        bool              _hasEach;
        bool              _hasAssigment;
        string            _name;
        bool              _isArray;
    };

    /**
     *
     */
    class TypeSymbolTable: public ModelTable<TypeName, Type>
    {
      public:
        /**
         *
         */
        TypeSymbolTable();
    };

    /**
     *
     */
    class VarSymbolTable: public ModelTable<VarName, Variable>
    {
      public:
        /**
         *
         */
        VarSymbolTable();
        /**
         *
         * @param
         */
        void
        initialize(TypeSymbolTable tst);
        /**
         *
         */
        void
        setPolyCoeffs(int order);
        /**
         *
         */
        void
        insert(VarName n, Variable vi);
        /**
         *
         */
        list<Variable>
        parameters();
      private:
        int _coeffs;
        list<Variable> _parameters;
    };
  }
}
#endif /* SYMBOL_TABLE_H_ */
