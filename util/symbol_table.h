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

#include "../ast/ast_types.h"
#include "../ast/element.h"
#include "../ast/modification.h"
#include "../ir/mmo_types.h"
#include "../ir/mmo_util.h"
#include "md_index.h"
#include "type.h"
#include "util_types.h"

/**
 *
 */
template<class Key, class Value, class Compare = std::less<Key> >
    class SymbolTable
    {
    public:
        /**
         *
         */
        ~SymbolTable ()
        {
        }
        ;
        /**
         *
         * @param k
         * @param v
         */
        void
        insert (Key k, Value v)
        {
            _st[k] = v;
        }
        ;
        /**
         *
         * @param k
         * @return
         */
        Value
        lookup (Key k)
        {
            typename std::map<Key, Value, Compare>::iterator it;
            it = _st.find (k);
            if (it == _st.end ())
            {
                return (NULL);
            }
            else
            {
                return (it->second);
            }
        }
        ;
        int
        count ()
        {
            return (_st.size ());
        }
        ;
        /**
         *
         * @param i
         * @return
         */
        Value
        val (int i)
        {
            typename std::map<Key, Value, Compare>::iterator it;
            it = _st.begin ();
            for (int j = 0; j < i; j++)
            {
                it++;
            }
            return (it->second);
        }
        ;
        /**
         *
         * @param i
         * @return
         */
        Key
        key (int i)
        {
            typename std::map<Key, Value, Compare>::iterator it;
            it = _st.begin ();
            for (int j = 0; j < i; j++)
            {
                it++;
            }
            return (it->first);
        }
        ;
        /**
         *
         * @param k
         */
        void
        remove (Key k)
        {
            _st.erase (k);
        }
        ;
        /**
         *
         * @return
         */
        Value
        begin ()
        {
            _currentKey = 0;
            if (_st.empty ())
            {
                return (NULL);
            }
            return (val (_currentKey));
        }
        ;
        /**
         *
         * @return
         */
        Value
        next ()
        {
            _currentKey++;
            if (_currentKey < _st.size ())
            {
                return (val (_currentKey));
            }
            return (NULL);
        }
        ;
        /**
         *
         * @return
         */
        bool
        end ()
        {
            if (_st.empty ())
            {
                return (true);
            }
            return (_currentKey == _st.size ());
        }
        ;
        /**
         *
         * @return
         */
        Key
        key ()
        {
            if (_currentKey < _st.size ())
            {
                return (key (_currentKey));
            }
            return (key (0));
        }
        ;
    protected:
        std::map<Key, Value, Compare> _st;
        unsigned int _currentKey;
    };

/**
 *
 */
class VarInfo_
{
public:
    /**
     *
     * @param t
     * @param tp
     * @param m
     * @param c
     */
    VarInfo_ (Type t, AST_TypePrefix tp, AST_Modification m, AST_Comment c);
    /**
     *
     * @param t
     * @param tp
     * @param m
     * @param s
     */
    VarInfo_ (Type t, AST_TypePrefix tp, AST_Modification m, unsigned int s, bool array);
    /**
     *
     * @return
     */
    AST_TypePrefix
    typePrefix ()
    {
        return (_tp);
    }
    ;
    /**
     *
     * @return
     */
    AST_Comment
    comment ()
    {
        return (_comm);
    }
    /**
     *
     * @param c
     */
    void
    setComment (AST_Comment c)
    {
        _comm = c;
    }
    /**
     *
     * @return
     */
    AST_Modification
    modification ()
    {
        return (_m);
    }
    ;
    /**
     *
     * @param m
     */
    void
    setModification (AST_Modification m)
    {
        _m = m;
        _processModification ();
    }
    ;
    /**
     *
     * @return
     */
    Type
    type ();
    /**
     *
     * @param t
     */
    void
    setType (Type t)
    {
        _t = t;
    }
    ;
    /**
     *
     */
    void
    setParameter ();
    /**
     *
     * @return
     */
    bool
    isParameter () const
    {
        return (_tp & TP_PARAMETER);
    }
    /**
     *
     * @return
     */
    bool
    isDiscrete () const
    {
        return ((_tp & TP_DISCRETE) || _discrete);
    }
    /**
     *
     * @return
     */
    bool
    builtIn () const
    {
        return (_builtin);
    }
    /**
     *
     */
    void
    setDiscrete ();
    /**
     *
     */
    void
    setBuiltIn ()
    {
        _builtin = true;
    }
    ;
    /**
     *
     * @return
     */
    bool
    isConstant () const
    {
        return (_tp & TP_CONSTANT);
    }
    ;
    /**
     *
     * @return
     */
    bool
    isInput () const
    {
        return (_tp & TP_INPUT);
    }
    ;
    /**
     *
     * @return
     */
    bool
    isOutput () const
    {
        return (_tp & TP_OUTPUT);
    }
    ;
    /**
     *
     * @return
     */
    bool
    isForType () const
    {
        return (_tp & TP_FOR);
    }
    /**
     *
     * @return
     */
    bool
    isState ();
    /**
     *
     */
    void
    setState ();
    /**
     *
     * @return
     */
    bool
    isUnknown ();
    /**
     *
     */
    void
    setUnknown ();
    /**
     *
     * @return
     */
    bool
    isTime ();
    /**
     *
     * @return
     */
    bool
    isAlgebraic ();
    /**
     *
     */
    void
    setAlgebraic ();
    /**
     *
     * @return
     */
    bool
    hasIndex ();
    /**
     *
     * @param idx
     */
    void
    setIndex (Index idx);
    /**
     *
     * @return
     */
    Index
    index ();
    /**
     *
     * @param val
     */
    void
    setValue (int val);
    /**
     *
     * @return
     */
    int
    value ();
    /**
     *
     * @return
     */
    unsigned int
    size ();
    /**
     *
     * @return
     */
    bool
    hasAssignment ();
    /**
     *
     * @return
     */
    bool
    hasStartModifier ();
    /**
     *
     * @return
     */
    bool
    hasEachModifier ();
    void
    setEachModifier (bool each);
    /**
     *
     * @return
     */
    string
    name ();
    /**
     *
     * @param n
     */
    void
    setName (string n);
    /**
     *
     * @return
     */
    AST_Expression
    exp ();
    /**
     *
     * @return
     */
    bool
    isArray ();
    /**
     *
     * @param os
     * @param e
     * @return
     */
    friend ostream &
    operator<< (ostream &os, const VarInfo_ &e);
private:
    void
    _processModification ();
    void
    _unsetAssignment ();
    void
    _unsetStartEach ();
    bool _state;
    bool _unknown;
    bool _discrete;
    Type _t;
    AST_TypePrefix _tp;
    AST_Modification _m;
    AST_Comment _comm;
    bool _builtin;
    Index _index;
    int _size;
    int _value;
    bool _algebraic;
    AST_Expression _exp;
    bool _hasStart;
    bool _hasEach;
    bool _hasAssigment;
    string _name;
    bool _isArray;
};

/**
 *
 */
typedef enum
{
    VST_CLASSIC_INIT,           //!< VST_CLASSIC_INIT
        VST_INIT,                   //!< VST_INIT
        VST_MODEL_FUNCTIONS,        //!< VST_MODEL_FUNCTIONS
        VST_MODEL_OUTPUT_FUNCTIONS,        //!< VST_MODEL_OUTPUT_FUNCTIONS
        VST_CLASSIC_MODEL_FUNCTIONS,        //!< VST_CLASSIC_MODEL_FUNCTIONS
        VST_FUNCTION,               //!< VST_FUNCTION
        VST_OUTPUT                  //!< VST_OUTPUT
} VST_Environment;

/**
 *
 */
class VarSymbolTable_ : public SymbolTable<VarName, VarInfo>
{
public:
    /**
     *
     */
    VarSymbolTable_ ();
    /**
     *
     * @param
     */
    void
    initialize (TypeSymbolTable);
    /**
     *
     * @param i
     * @return
     */
    VarInfo
    varInfo (int i)
    {
        return (val (i));
    }
    ;
    /**
     *
     * @param i
     * @return
     */
    VarName
    varName (int i)
    {
        return (key (i));
    }
    /**
     *
     * @param vse
     */
    void
    setPrintEnvironment (VST_Environment vse);
    /**
     *
     * @return
     */
    VST_Environment
    printEnvironment ();
    /**
     *
     * @param vi
     * @param idx
     * @param offset
     * @param order
     * @param idxs
     * @param constant
     * @param forOffset
     * @return
     */
    string
    print (VarInfo vi, string idx = "", int offset = 0, int order = 0, list<Index> idxs = list<Index> (), int constant = -1, int forOffset = 0);
    string
    printIndex (Index idx, list<Index> idxs, string sub, int offset, int constant, VarInfo vi, int order);
    void
    setPolyCoeffs (int order);
    void
    insert (VarName n, VarInfo vi);
    list<VarInfo>
    parameters ();
    string
    getTypePrefix ();
private:
    VST_Environment _vste;
    int _coeffs;
    list<VarInfo> _parameters;
};

/**
 *
 */
class TypeSymbolTable_ : public SymbolTable<TypeName, Type>
{
public:
    /**
     *
     */
    TypeSymbolTable_ ();
};

/**
 *
 * @param t
 * @param tp
 * @param
 * @param
 * @param s
 * @return
 */
VarInfo
newVarInfo (Type t, AST_TypePrefix tp, AST_Modification, AST_Comment, unsigned int s = 0, bool array = false);
/**
 *
 * @return
 */
VarSymbolTable
newVarSymbolTable ();
/**
 *
 * @return
 */
TypeSymbolTable
newTypeSymbolTable ();

/**
 *
 */
template<class Value>
    class IndexTable : public SymbolTable<Index, Value>
    {
    public:
        /**
         *
         * @return
         */
        Value
        begin ()
        {
            Value e = SymbolTable<Index, Value>::begin ();
            if (e == NULL)
            {
                return (NULL);
            }
            Index i = SymbolTable<Index, Value>::key ();
            if (i.hasRange ())
            {
                _beginRange = true;
                _endRange = false;
                _range = i.range ();
                _begin = i.begin ();
                _end = i.end ();
            }
            else
            {
                _beginRange = false;
                _endRange = false;
                _range = 0;
                _begin = 0;
                _end = 0;
            }
            return (e);
        }
        ;
        /** @brief Returns an iterator to the next equation of the table.
         *
         * 	@return Next MMO_Equation in the table.
         *
         * 		The \emph{beginRange} and \emph{endRange} flags are set in the following way:
         * 			-# If the next equation is a generic then:
         * 				+# If the range is the same as the previous equation, then set
         * 				   \emph{beginRange} to false.
         * 				+# Otherwise if the previous equation had a range defined, then set
         * 			       \emph{endRange} to true.
         * 			    +# Finally, if there was no previous range defined, set \emph{beginRange}
         * 			       to true.
         * 			-# If the next equation is a simple equation, then:
         *				+# If the there was a previous range defined, then set \emph{endRange} to true.
         *				+# Otherwise set \emph{endRange} to false.
         *				+# Set \emph{beginRange} to false.
         *
         */
        Value
        next ()
        {
            Value e = SymbolTable<Index, Value>::next ();
            Index i = SymbolTable<Index, Value>::key ();
            if (e == NULL)
            {
                return (NULL);
            }
            if (i.hasRange ())
            {
                int range = i.range ();
                int begin = i.begin ();
                int end = i.end ();
                if (range == _range && _begin == begin && _end == end)
                {
                    _beginRange = false;
                    _endRange = false;
                }
                else if (_range > 0)
                {
                    _endRange = true;
                    _beginRange = true;
                    _range = range;
                    _begin = begin;
                    _end = end;
                }
                else if (_range == 0)
                {
                    _beginRange = true;
                    _endRange = false;
                    _range = range;
                    _begin = begin;
                    _end = end;
                }
            }
            else
            {
                if (_range > 0)
                {
                    _endRange = true;
                }
                else
                {
                    _endRange = false;
                }
                _beginRange = false;
                _range = 0;
                _begin = 0;
                _end = 0;
            }
            return (e);
        }
        ;

        /** @brief Returns true if the end of the table is reached.
         *
         * 	@return True if the end of the table is reached, false otherwise.
         *
         * 	@note{As a size effect, the \emph{beginRange} and \emph{endRange} flags
         * 		  are cleared.}
         *
         */
        bool
        end ()
        {
            bool ret = SymbolTable<Index, Value>::end ();
            if (ret)
            {
                _beginRange = false;
                _endRange = false;
                _range = 0;
            }
            return (ret);
        }
        ;
        /**
         *
         * @return
         */
        bool
        beginGenericDefinition ()
        {
            return (_beginRange);
        }
        ;
        /**
         *
         * @return
         */
        bool
        endGenericDefinition ()
        {
            return (_endRange);
        }
        ;
        /**
         *
         * @param val
         * @return
         */
        bool
        find (int val)
        {
            int s = SymbolTable<Index, Value>::count ();
            for (int i = 0; i < s; i++)
            {
                Index idx = SymbolTable<Index, Value>::key (i);
                if (idx.factor () != 0)
                {
                    if (idx.checkReverseValue (val))
                    {
                        return (true);
                    }
                }
            }
            return (false);
        }
    private:
        int _range;
        int _begin;
        int _end;
        bool _beginRange;
        bool _endRange;
    };

/**
 *
 */
class MMO_EquationTable_ : public IndexTable<MMO_Equation>
{
public:
    /**
     *
     */
    MMO_EquationTable_ ();
    /**
     *
     * @param index
     * @return
     */
    list<MMO_Equation>
    equation (Index index);
    /**
     *
     * @param index
     * @return
     */
    Index
    equationIndex (Index index);
    bool
    equations (Index index);
    bool
    findGenericDependencies (int val);
private:
    MMO_Equation
    _equation (Index index);
};
/**
 *
 * @return
 */
MMO_EquationTable
newMMO_EquationTable ();
/**
 *
 * @param m
 */
void
deleteMMO_EquationTable (MMO_EquationTable m);

/**
 *
 */
class MMO_EventTable_ : public IndexTable<MMO_Event>
{
public:
    /**
     *
     */
    MMO_EventTable_ ();
};
/**
 *
 * @return
 */
MMO_EventTable
newMMO_EventTable ();
/**
 *
 * @param m
 */
void
deleteMMO_EventTable (MMO_EventTable m);

/**
 *
 */
class MMO_FunctionTable_ : public IndexTable<MMO_Function>
{
public:
    /**
     *
     */
    MMO_FunctionTable_ ();
};
/**
 *
 * @return
 */
MMO_FunctionTable
newMMO_FunctionTable ();
/**
 *
 * @param m
 */
void
deleteMMO_FunctionTable (MMO_FunctionTable m);

/**
 *
 */
class MMO_DependenciesTable_ : public IndexTable<Dependencies>
{
public:
    /**
     *
     */
    MMO_DependenciesTable_ ();
    bool
    find (int val);
};
/**
 *
 * @return
 */
MMO_DependenciesTable
newMMO_DependenciesTable ();
/**
 *
 * @param m
 */
void
deleteMMO_DependenciesTable (MMO_DependenciesTable m);

/**
 *
 */
class MMO_ArgumentsTable_ : public IndexTable<MMO_FunctionData>
{
public:
    /**
     *
     */
    MMO_ArgumentsTable_ ();
};
/**
 *
 * @return
 */
MMO_ArgumentsTable
newMMO_ArgumentsTable ();
/**
 *
 * @param m
 */
void
deleteMMO_ArgumentsTable (MMO_ArgumentsTable m);

/**
 *
 */
class MMO_IndexTable_ : public SymbolTable<Index, Index*>
{
public:
    /**
     *
     */
    MMO_IndexTable_ ();
};
/**
 *
 * @return
 */
MMO_IndexTable
newMMO_IndexTable ();
/**
 *
 * @param m
 */
void
deleteMMO_IndexTable (MMO_IndexTable m);

/**
 *
 */
template<class R>
    class ListTable_
    {
    public:
        /**
         *
         * @param i
         */
        void
        insert (R i)
        {
            _ls.push_back (i);
        }
        ;
        /**
         *
         * @return
         */
        R
        begin ()
        {
            _currentKey = 0;
            if (_ls.empty ())
            {
                return (R ());
            }
            return (_val (_currentKey));
        }
        ;
        /**
         *
         * @return
         */
        R
        next ()
        {
            _currentKey++;
            if (_currentKey < _ls.size ())
            {
                return (_val (_currentKey));
            }
            return (R ());
        }
        ;
        /**
         *
         * @return
         */
        bool
        end ()
        {
            if (_ls.empty ())
            {
                return (true);
            }
            return (_currentKey == _ls.size ());
        }
        ;
        /**
         *
         * @return
         */
        R
        last ()
        {
            return (_ls.back ());
        }
        /**
         *
         * @return
         */
        int
        count ()
        {
            return (_ls.size ());
        }
        ;
    private:
        R
        _val (int i)
        {
            typename std::list<R>::iterator it;
            it = _ls.begin ();
            for (int j = 0; j < i; j++)
            {
                it++;
            }
            return (*it);
        }
        ;
        list<R> _ls;
        unsigned int _currentKey;
    };

/**
 *
 */
class MMO_StatementTable_ : public ListTable_<MMO_Statement>
{
public:
    /**
     *
     */
    MMO_StatementTable_ ();
    /**
     *
     */
    ~MMO_StatementTable_ ();
};
/**
 *
 * @return
 */
MMO_StatementTable
newMMO_StatementTable ();
/**
 *
 * @param m
 */
void
deleteMMO_StatementTable (MMO_StatementTable m);

/**
 *
 */
class MMO_ImportTable_ : public ListTable_<string>
{
public:
    /**
     *
     */
    MMO_ImportTable_ ();
    /**
     *
     */
    ~MMO_ImportTable_ ();
};
/**
 *
 * @return
 */
MMO_ImportTable
newMMO_ImportTable ();
/**
 *
 * @param m
 */
void
deleteMMO_ImportTable (MMO_ImportTable m);

/**
 *
 */
class MMO_FunctionDefinitionTable_ : public ListTable_<MMO_FunctionDefinition>
{
public:
    /**
     *
     */
    MMO_FunctionDefinitionTable_ ();
    /**
     *
     */
    ~MMO_FunctionDefinitionTable_ ();
};
/**
 *
 * @return
 */
MMO_FunctionDefinitionTable
newMMO_FunctionDefinitionTable ();
/**
 *
 * @param m
 */
void
deleteMMO_FunctionDefinitionTable (MMO_FunctionDefinitionTable m);

/**
 *
 */
class MMO_SymbolRefTable_ : public SymbolTable<string, MMO_FunctionInfo *>
{
public:
    /**
     *
     */
    MMO_SymbolRefTable_ ();
    /**
     *
     */
    ~MMO_SymbolRefTable_ ();
};
/**
 *
 * @return
 */
MMO_SymbolRefTable
newMMO_SymbolRefTable ();
/**
 *
 * @param m
 */
void
deleteMMO_SymbolRefTable (MMO_SymbolRefTable m);

/**
 *
 */
class MMO_PackageTable_ : public ListTable_<MMO_PackageData>
{
public:
    /**
     *
     */
    MMO_PackageTable_ ();
    /**
     *
     * @param name
     * @return
     */
    string
    functionPrefix (string name);
};
/**
 *
 * @return
 */
MMO_PackageTable
newMMO_PackageTable ();
/**
 *
 * @param m
 */
void
deleteMMO_PackageTable (MMO_PackageTable m);

#endif /* SYMBOL_TABLE_H_ */
