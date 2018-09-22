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

#include <util/ast_visitors/splitfor_visitor.h>
#include <util/debug.h>


namespace Modelica {

    using namespace boost;
 
    EquationList SplitForVisitor::operator()(Connect eq) const {
        return EquationList(1, eq);
    };

    EquationList SplitForVisitor::operator()(Equality eq) const {
        return EquationList(1, eq);
    };

    EquationList SplitForVisitor::operator()(CallEq eq) const {
        return EquationList(1, eq);
    };

    EquationList SplitForVisitor::operator()(IfEq eq) const {
        return EquationList(1, eq);
    };

    EquationList SplitForVisitor::operator()(WhenEq eq) const {
        return EquationList(1, eq);
    };

    EquationList SplitForVisitor::operator()(ForEq eq) const {
        EquationList eql;
        foreach_(Equation e, eq.els) {
            if (is<ForEq>(e)) {
                ERROR("Nested 'for' is not supported");
            }
            else {
                ForEq foreq = ForEq(eq.r, EquationList(1, e));
                eql.push_back(foreq);
            }
        }
        return eql;
    };
    
}