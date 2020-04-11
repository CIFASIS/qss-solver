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

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <iostream>

#include "util/visitors/called_functions.h"
#include "util/util.h"
#include "tests/test_utils.h"

/// @addtogroup QSSSolverUTests
/// @{

using namespace MicroModelica::Util;

/// Unit tests for called functions visitor.
class CalledFunctionsTest : public ::testing::Test, public CalledFunctions {
  public:
  CalledFunctionsTest() : CalledFunctions() {}

  virtual ~CalledFunctionsTest() {}
};

TEST_F(CalledFunctionsTest, testCalledFunctions)
{
  stringstream code;
  int res;
  code << "f() + g()" << endl;
  AST_Expression exp = parseExpression(code.str(), &res);
  EXPECT_EQ(res, 0);
  SymbolTable symbols = apply(exp);
  EXPECT_EQ(symbols.size(), 2);
  Option<std::string> f = symbols["f"];
  Option<std::string> g = symbols["g"];
  EXPECT_EQ(f.get(), "f");
  EXPECT_EQ(g.get(), "g");
}

TEST_F(CalledFunctionsTest, testEmptyFunctions)
{
  stringstream code;
  int res;
  code << "a + b" << endl;
  AST_Expression exp = parseExpression(code.str(), &res);
  EXPECT_EQ(res, 0);
  SymbolTable symbols = apply(exp);
  EXPECT_EQ(symbols.size(), 0);
}

/// @}
