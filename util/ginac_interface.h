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

#ifndef GINAC_INTERFACE_H_
#define GINAC_INTERFACE_H_

#include <ginac/ex.h>
#include <ginac/function.h>
#include <ginac/symbol.h>
#include <map>
#include <string>

#include "../ast/ast_types.h"
#include "../ir/mmo_types.h"
#include "ast_util.h"
#include "util_types.h"

/**
 *
 */
DECLARE_FUNCTION_2P (var)
/**
 *
 */
DECLARE_FUNCTION_2P (der)
/**
 *
 */
DECLARE_FUNCTION_2P (der2)
/**
 *
 */
DECLARE_FUNCTION_1P (der3)

/**
 *
 */
DECLARE_FUNCTION_1P (pre)

/**
 *
 */
class ConvertToGiNaC: public AST_Expression_Visitor<GiNaC::ex>
{
  public:
    /**
     *
     * @param varEnv
     * @param forDerivation
     * @param exp
     */
    ConvertToGiNaC(VarSymbolTable varEnv, bool forDerivation = false,
        MMO_Expression exp = NULL);
    /**
     *
     * @param
     * @param replaceDer
     * @param generateIndexes
     * @return
     */
    GiNaC::ex
    convert(AST_Expression, bool replaceDer = true,
        bool generateIndexes = false);
    /**
     *
     * @param
     * @return
     */
    GiNaC::symbol&
    getSymbol(AST_Expression_ComponentReference);
    /**
     *
     * @param
     * @return
     */
    GiNaC::symbol&
    getSymbol(string);
    /**
     *
     * @param
     * @return
     */
    GiNaC::symbol&
    getSymbol(AST_Expression_Derivative);
    /**
     *
     * @return
     */
    GiNaC::symbol&
    getTime();
    GiNaC::symbol&
    first();
    GiNaC::symbol&
    next();
    bool
    end();
    map<string, GiNaC::symbol>
    directory();
    string
    identifier(string str);
    private:
    virtual GiNaC::ex
    foldTraverseElement(AST_Expression);
    virtual GiNaC::ex
    foldTraverseElementUMinus(AST_Expression);
    virtual GiNaC::ex
    foldTraverseElement(GiNaC::ex, GiNaC::ex, BinOpType);
    map<string, GiNaC::symbol> _directory;
    bool _forDerivation;
    VarSymbolTable _varEnv;
    bool _replaceDer;
    bool _generateIndexes;
    MMO_Expression _exp;
};

/**
 *
 */
class ConvertToExpression
{
  public:
    /**
     *
     * @param
     * @return
     */
    static AST_Expression
    convert(GiNaC::ex);
};

#endif  /* GINAC_INTERFACE_H_ */
