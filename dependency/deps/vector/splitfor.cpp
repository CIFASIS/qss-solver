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

#include <ast/equation.h>
#include <ast/queries.h>
#include <boost/variant/get.hpp>
#include <util/ast_visitors/splitfor_visitor.h>
#include <causalize/state_variables_finder.h>
#include <causalize/vector/splitfor.h>
#include <algorithm>
#include <vector>


namespace Modelica {
    
    SplitFor::SplitFor(MMO_Class &c): _c(c) {
    }
    
    void SplitFor::splitFor() {
        EquationList &el = _c.equations_ref().equations_ref();
        SplitForVisitor efv;
        EquationList el_new;
        foreach_(Equation e1, el) {
            EquationList eql = Apply(efv, e1);
            foreach_(Equation e2, eql) {
                el_new.push_back(e2);
            }
        }
        el=el_new;
    }

};

