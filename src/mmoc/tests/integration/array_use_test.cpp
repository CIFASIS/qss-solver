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

#include <ast/parser/parse.h>
#include "util/model_config.h"
#include "util/visitors/array_use.h"
#include "util/symbol_table.h"
#include "tests/test_utils.h"

/// @addtogroup QSSSolverUTests
/// @{

using namespace MicroModelica::Util;

/// Unit tests for array use visitor.
class ArrayUseTest : public ::testing::Test, public ArrayUse {
  public:
  ArrayUseTest() : ArrayUse() {}

  virtual ~ArrayUseTest() {}
};

TEST_F(ArrayUseTest, testCorrectArrayUse)
{
  stringstream code;
  stringstream variables;
  int res;
  variables << "Real a[10,10];";
  ModelConfig::instance().setSymbols(getSymbols(variables.str()));
  code << "a[1,2]" << endl;
  AST_Expression exp = parseExpression(code.str(), &res);
  EXPECT_EQ(res, 0);
  EXPECT_TRUE(apply(exp));
}

TEST_F(ArrayUseTest, testWrongArrayUse)
{
  stringstream code;
  stringstream variables;
  int res;
  variables << "Real a[10];";
  code << "a[1,2]" << endl;
  AST_Expression exp = parseExpression(code.str(), &res);
  EXPECT_EQ(res, 0);
  ModelConfig::instance().setSymbols(getSymbols(variables.str()));
  EXPECT_FALSE(apply(exp));
}

/// @}
