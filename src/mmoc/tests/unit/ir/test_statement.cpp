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

#include <cassert>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../ir/statement.h"

using ::testing::_;
using ::testing::InSequence;
using ::testing::Return;
using ::testing::StrictMock;

using namespace Microdelica::IR;
using namespace Microdelica::Util;

class StatementTest : public ::testing::Test, public Statement {
  protected:
  StatementTest() {}

  virtual ~StatementTest() {}
};

TEST_F(StatementTest, testInitialState) {}
