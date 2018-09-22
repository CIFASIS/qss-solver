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

#include <util/debug.h>
#include <util/ast_visitors/replace_equation.h>

namespace Modelica {

    using namespace boost;
    ReplaceEquation::ReplaceEquation(Expression l, Expression r): look(l), rep(r), replace_exp(look,rep) {
      replace_exp.ignoreIndexes();
    };
    Equation ReplaceEquation::operator()(Connect v) const {
      ERROR("Replace in connect equation not implemented\n");
      return v;
    };
    Equation ReplaceEquation::operator()(Equality v) const {
      v.left_ref()=Apply(replace_exp, v.left_ref());
      v.right_ref()=Apply(replace_exp, v.right_ref());
      return v;
    };
    Equation ReplaceEquation::operator()(IfEq v) const {
      ERROR("Replace in if equation not implemented\n");
      return v;
    }
    Equation ReplaceEquation::operator()(CallEq v) const {
      ERROR("Replace in call equation not implemented\n");
      return v;
    };
    Equation ReplaceEquation::operator()(ForEq v) const {
      foreach_(Index &i, v.range_ref().indexes_ref()) {
        if (i.exp()) {
          i.exp_ref()=Apply(replace_exp, i.exp_ref().get());
        }
      }
      foreach_(Equation &e, v.elements_ref()) {
        e = ApplyThis(e);
      }
      //ERROR("Replace in for equation not implemented\n");
      return v;
    };
    Equation ReplaceEquation::operator()(WhenEq v) const {
      ERROR("Replace in when equation not implemented\n");
      return v;
    }
 
}
