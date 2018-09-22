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

#ifndef SOLVE_H
#define SOLVE_H

#include <ast/equation.h>
#include <util/table.h>
#include <mmo/mmo_class.h>
#include <ginac/ginac.h>

using namespace Modelica::AST;
class EquationSolver {
public:
  static EquationList Solve(EquationList eqs, ExpList crs, VarSymbolTable &syms, std::list<std::string> &c_code, ClassList &cl, const std::string path);
  static Equation Solve(Equation eq, Expression exp, VarSymbolTable &syms, std::list<std::string> &c_code, ClassList &cl, const std::string path);
};
#endif
