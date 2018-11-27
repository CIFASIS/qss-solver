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

#ifndef TYPE_H_
#define TYPE_H_

#include <iostream>
#include <list>
#include <string>

#include "../ast/ast_types.h"
#include "../ast/expression.h"
#include "macros.h"

using namespace std;

/**
 *
 */
enum TypesType
{
  TYREAL,    //!< TYREAL
  TYINTEGER,  //!< TYINTEGER
  TYBOOLEAN, //!< TYBOOLEAN
  TYSTRING,  //!< TYSTRING
  TYARRAY,   //!< TYARRAY
  TYTUPLA,   //!< TYTUPLA
  TYFUNCTION  //!< TYFUNCTION
};

/**
 *
 */
DEFINE_TYPE(Type);
/**
 *
 */
DEFINE_TYPE(Type_Real);
/**
 *
 */
DEFINE_TYPE(Type_Integer);
/**
 *
 */
DEFINE_TYPE(Type_Boolean);
/**
 *
 */
DEFINE_TYPE(Type_String);
/**
 *
 */
DEFINE_TYPE(Type_Array);
/**
 *
 */
DEFINE_TYPE(Type_Tupla);
/**
 *
 */
DEFINE_TYPE(Type_Function);
/**
 *
 */
DEFINE_LIST(Type);

/**
 *
 */
class Type_
{
  public:
    /**
     *
     */
    virtual
    ~Type_()
    {
    }
    ;
    /**
     *
     * @return
     */
    virtual TypesType
    getType() = 0;
    /**
     *
     * @return
     */
    virtual string
    print() const =0;
    /**
     *
     * @param os
     * @param e
     * @return
     */
    friend ostream &
    operator<<(ostream &os, const Type_ &e);
    /**
     *
     * @param os
     * @param e
     * @return
     */
    friend ostream &
    operator<<(ostream &os, const Type &e);
    /**
     *
     * @param e1
     * @param e2
     * @return
     */
    friend int
    operator==(Type_ &e1, Type_ &e2);
    /**
     *
     * @param e1
     * @param e2
     * @return
     */
    friend int
    operator==(Type_ &e1, Type e2);
    /**
     *
     * @param e1
     * @param e2
     * @return
     */
    friend int
    operator!=(Type_ &e1, Type_ &e2);
    /**
     *
     * @param e1
     * @param e2
     * @return
     */
    friend int
    operator!=(Type_ &e1, Type e2);
    /**
     *
     * @return
     */
    Type_Array
    getAsArray();
    /**
     *
     * @return
     */
    Type_Tupla
    getAsTupla();
    /**
     *
     * @return
     */
    Type_Function
    getAsFunction();
};

/**
 *
 */
class Type_Real_: public Type_
{
  public:
    /**
     *
     */
    Type_Real_();
    /**
     *
     */
    virtual
    ~Type_Real_();
    /**
     *
     * @return
     */
    TypesType
    getType()
    {
      return TYREAL;
    }
    /**
     *
     * @return
     */
    string
    print() const;
};
/**
 *
 * @return
 */
Type_Real
newType_Real();
/**
 *
 * @param m
 */
void
deleteType_Real(Type_Real m);

/**
 *
 */
class Type_Integer_: public Type_
{
  public:
    /**
     *
     */
    Type_Integer_();
    /**
     *
     */
    virtual
    ~Type_Integer_();
    /**
     *
     * @return
     */
    TypesType
    getType()
    {
      return TYINTEGER;
    }
    ;
    /**
     *
     * @return
     */
    string
    print() const;
};
/**
 *
 * @return
 */
Type_Integer
newType_Integer();
/**
 *
 * @param m
 */
void
deleteType_Integer(Type_Integer m);

/**
 *
 */
class Type_Boolean_: public Type_
{
  public:
    /**
     *
     */
    virtual
    ~Type_Boolean_()
    {
    }
    ;
    /**
     *
     * @return
     */
    TypesType
    getType()
    {
      return TYBOOLEAN;
    }
    ;
    /**
     *
     * @return
     */
    string
    print() const;
};

/**
 *
 */
class Type_String_: public Type_
{
  public:
    /**
     *
     */
    virtual
    ~Type_String_();
    /**
     *
     * @return
     */
    TypesType
    getType()
    {
      return TYSTRING;
    }
    ;
    /**
     *
     * @return
     */
    string
    print() const;
};
/**
 *
 * @return
 */
Type_String
newType_String();
/**
 *
 * @param m
 */
void
deleteType_String(Type_String m);

/**
 *
 */
class Type_Array_: public Type_
{
  public:
    /**
     *
     * @param t
     * @param dim
     */
    Type_Array_(Type t, AST_Expression dim);
    /**
     *
     */
    virtual
    ~Type_Array_();
    /**
     *
     * @return
     */
    TypesType
    getType()
    {
      return TYARRAY;
    }
    /**
     *
     * @return
     */
    string
    print() const;
    /**
     *
     * @return
     */
    Type
    arrayOf();
    /**
     *
     * @return
     */
    AST_Expression
    dimension()
    {
      return _dim;
    }
    ;
    private:
    Type _t;
    AST_Expression _dim;
};

/**
 *
 */
class Type_Tupla_: public Type_
{
  public:
    /**
     *
     * @param tyl
     */
    Type_Tupla_(TypeList tyl);
    /**
     *
     */
    virtual
    ~Type_Tupla_();
    /**
     *
     * @return
     */
    string
    print() const;
    /**
     *
     * @return
     */
    TypeList
    tupla()
    {
      return _tyl;
    }
    ;
    /**
     *
     * @return
     */
    TypesType
    getType()
    {
      return TYTUPLA;
    }
  private:
    TypeList _tyl;
};

/**
 *
 */
class Type_Function_: public Type_
{
  public:
    /**
     *
     * @param output
     * @param input
     */
    Type_Function_(Type output, TypeList input);
    /**
     *
     */
    virtual
    ~Type_Function_();
    /**
     *
     * @return
     */
    string
    print() const;
    /**
     *
     * @return
     */
    TypeList
    input()
    {
      return _input;
    }
    ;
    /**
     *
     * @return
     */
    Type
    output()
    {
      return _output;
    }
    ;
    /**
     *
     * @return
     */
    TypesType
    getType()
    {
      return TYFUNCTION;
    }
    ;
    private:
    TypeList _input;
    Type _output;
};

#endif  /* TYPE_H_ */
