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

#include "../ast/ast_types.h"
#include "../util/util_types.h"

/**
 *
 */
typedef enum
{
  CL_MODEL,  //!< CL_MODEL
  CL_PACKAGE,  //!< CL_PACKAGE
  CL_FUNCTION  //!< CL_FUNCTION
} CL_Type;

/**
 *
 */
typedef enum
{
  DEC_PUBLIC,  //!< DEC_PUBLIC
  DEC_LOCAL  //!< DEC_LOCAL
} DEC_Type;


class MMO_Model;

class MMO_Function;

class MMO_Package;

class MMO_Annotation;

class MMO_ModelAnnotation;

class MMO_ImportTable;

/**
 *
 */
class MMO_Class
{
  public:
    /**
     *
     */
    MMO_Class();
    /**
     *
     */
    ~MMO_Class();
    /**
     *
     * @return
     */
    MMO_Package*
    getAsPackage() const;
    /**
     *
     * @return
     */
    MMO_Model*
    getAsModel() const;
    /**
     *
     * @return
     */
    MMO_Function*
    getAsFunction() const;
    /**
     *
     * @return
     */
    virtual string
    name() const = 0;
    /**
     *
     * @return
     */
    virtual CL_Type
    classType() = 0;
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
     * @param f
     */
    virtual void
    insert(MMO_Function &f) = 0;
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
    virtual MMO_ImportTable
    imports() = 0;
};

#endif  /* MMO_CLASS_H_ */
