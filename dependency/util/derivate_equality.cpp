
/*****************************************************************************

    This file is part of Modelica C Compiler.

    Modelica C Compiler is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Modelica C Compiler is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Modelica C Compiler.  If not, see <http://www.gnu.org/licenses/>.

******************************************************************************/
/*
 * La idea es hacer una función que tome una igualdad y devuelva la misma pero con sus miembros derivados.
 * 
 */
 
#include <util/derivate_equality.h>
#include <ast/equation.h>
#include <ast/queries.h>
#include <util/derivate.h>
#include <boost/variant/get.hpp>
#include <util/ast_visitors/part_evalexp.h>


namespace Modelica {
    Equality derivate_equality(Equality eq, VarSymbolTable syms) {
    PartEvalExp eval(syms, true);
    Expression left= Apply(eval,eq.left_ref());    
    Expression right= Apply(eval,eq.right_ref());
    eq.left_ref()=derivate(left,syms);
    eq.right_ref()=derivate(right,syms);
// aplicar evaluador al resultado... 
    return eq;
  }

}
