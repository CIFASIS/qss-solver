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
#include <iostream>

#include "util/visitors/convert_cont_red.h"
#include "util/util.h"
#include "util/symbol_table.h"
#include "tests/test_utils.h"

/// @addtogroup QSSSolverUTests
/// @{

using namespace MicroModelica::Util;

/// Unit tests for ConvertContRed variable visitor.
class ConvertContRedTest : public ::testing::Test, public ConvertContRed {
  public:
  ConvertContRedTest() : ConvertContRed(Utils::instance().symbols()) {}

  virtual ~ConvertContRedTest() {}
};

TEST_F(ConvertContRedTest, testSetReduction)
{
  setReduction(0);
  EXPECT_EQ(_reduction, ContReduction::SUM);
  EXPECT_EQ(_oper, "+");
  EXPECT_EQ(_variables.size(), 0);
  EXPECT_EQ(_code.size(), 0);
  setReduction(1);
  EXPECT_EQ(_reduction, ContReduction::PROD);
  EXPECT_EQ(_oper, "*");
  EXPECT_EQ(_variables.size(), 0);
  EXPECT_EQ(_code.size(), 0);
}

TEST_F(ConvertContRedTest, testSumReductionExp)
{
  stringstream code;
  stringstream variables;
  int res;
  variables << "Real x[10];";
  code << "sum(x)" << endl;
  AST_Expression exp = parseExpression(code.str(), &res);
  EXPECT_EQ(res, 0);
  _symbols = getSymbols(variables.str());
  setReduction(0);
  AST_Expression mod_exp = apply(exp);
  EXPECT_TRUE(hasReduction());
  EXPECT_EQ(_code.size(), 2);
  EXPECT_EQ(_variables.size(), 1);
}

/// @}
