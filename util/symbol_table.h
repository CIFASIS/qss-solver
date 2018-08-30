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
        typedef enum 
        {
          State,
          Algebraic
        } RealType;
        inline void 
        setRealType(RealType type) { _realType = type; };
        /**
         *
         * @return
         */
        inline AST_TypePrefix
        typePrefix() { return _tp; };
        /**
         *
         * @return
         */
        inline AST_Comment
        comment() { return _comm; };
        /**
         *
         * @param c
         */
        inline void
        setComment(AST_Comment c) { _comm = c; };
        /**
         *
         * @return
         */
        inline AST_Modification modification() { return _m; };
        /**
         *
         * @param m
         */
        inline void
        setModification(AST_Modification m) { _m = m; processModification(); };
        /**
         *
         * @return
         */
        inline Type
        type() { return _t; };
        /**
         *
         * @param t
         */
        inline void
        setType(Type t) { _t = t; };
        /**
         *
         */
        inline void
        setParameter() { _tp = TP_PARAMETER; unsetStartEach(); };
        /**
         *
         * @return
         */
        inline bool
        isParameter() const { return _tp & TP_PARAMETER; };
        /**
         *
         * @return
         */
        inline bool 
        isDiscrete() const { return (_tp & TP_DISCRETE) || _discrete; };
        /**
         *
         * @return
         */
        inline bool
        builtIn() const { return _builtin; };
        /**
         *
         */
        inline void
        setDiscrete() { _discrete = true; unsetAssignment(); };
        /**
         *
         */
        inline void
        setBuiltIn() { _builtin = true; };
        /**
         *
         * @return
         */
        inline bool
        isConstant() const { return _tp & TP_CONSTANT; };
        /**
         *
         * @return
         */
        inline bool
        isInput() const { return _tp & TP_INPUT; };
        /**
         *
         * @return
         */
        inline bool
        isOutput() const { return _tp & TP_OUTPUT; };
        /**
         *
         * @return
         */
        inline bool
        isForType() const { return _tp & TP_FOR; };
        /**
         *
         * @return
         */
        inline bool
        isState() { return _realType == State; };
        /**
         *
         */
        inline void
        setState() { unsetAssignment(); };
        /**
         *
         * @return
         */
        inline bool
        isUnknown() { return _unknown; };
        /**
         *
         */
        inline void
        setUnknown() { _unknown = true; };
        /**
         *
         * @return
         */
        inline bool
        isTime() { return _name.compare("time") == 0; };
        /**
         *
         * @return
         */
        inline bool
        isAlgebraic() { return _realType == Algebraic; };
        /**
         *
         */
        inline void
        setAlgebraic() { unsetAssignment(); unsetStartEach(); };

        /**
         *
         * @param val
         */
        inline void
        setValue(int val) { _value = val; };
        /**
         *
         * @return
         */
        inline int
        value() { return _value; };
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
        inline bool
        hasAssignment() { return _hasAssigment; };
        /**
         *
         * @return
         */
        inline bool
        hasStartModifier() { return _hasStart; };
        /**
         *
         * @return
         */
        inline bool
        hasEachModifier() { return _hasEach; };
        inline void
        setEachModifier(bool each) { _hasEach = each; }; 
        /**
         *
         * @return
         */
        inline string
        name() { return _name; };
        /**
         *
         * @param n
         */
        inline void
        setName(string n) { _name = n; };
        /**
         *
         * @return
         */
        inline AST_Expression
        exp() { return _exp; };
        /**
         *
         * @return
         */
        inline bool
        isArray() { return _isArray; };
        /**
         *
         * @param os
         * @param e
         * @return
         */
        friend ostream& operator<<(ostream &os, const Variable &e);
        /**
         *
         * @param dim
         * @return
         */
        inline int
        size(int dim) { return _size[dim]; };
        /**
         *
         * @return
         */
        inline int
        dimensions() { return _size.size(); };
        std::string  
        declaration(std::string prefix = "");
        std::string 
        initialization();
        inline bool
        hasOffset() { return _hasOffset; };
        inline void 
        setOffset(int offset) { _offset = offset; _hasOffset = true; };
        inline int  
        offset() { return _offset; };
      private:
        void
        processModification();
        void
        unsetAssignment() { _hasAssigment = false; };
        inline void
        unsetStartEach() { _hasEach = false; _hasStart = false; };
        bool              _unknown;
        bool              _discrete;
        Type              _t;
        AST_TypePrefix    _tp;
        AST_Modification  _m;
        AST_Comment       _comm;
        bool              _builtin;
        vector<int>       _size;
        int               _value;
        AST_Expression    _exp;
        bool              _hasStart;
        bool              _hasEach;
        bool              _hasAssigment;
        string            _name;
        bool              _isArray;
        bool              _hasOffset;
        int               _offset;
        RealType          _realType;
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
        insert(VarName n, Variable vi);
        /**
         *
         */
        list<Variable>
        parameters();
      private:
        int             _coeffs;
        list<Variable>  _parameters;
    };
  }
}
#endif /* SYMBOL_TABLE_H_ */
