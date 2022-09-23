/*****************************************************************************

 This file is part of QSSModelInstance Solver.

 QSSModelInstance Solver is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 QSSModelInstance Solver is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with QSSModelInstance Solver.  If not, see
 <http://www.gnu.org/licenses/>.

 ******************************************************************************/

#include <gtest/gtest.h>

#include <cstdlib>
#include <fstream>

/// @addtogroup QssSolverITests
/// @{

class IPackageTests : public testing::TestWithParam<const char*> {
};

TEST_P(IPackageTests, GenerateCode)
{
  const std::string NAME = GetParam();
  std::cout << "Testing package: " << NAME << std::endl;
  const std::string MODEL = " ./integration/gt_data/packages/" + NAME + ".mo";
  const std::string TEST_CMD = "./integration/test_package_results.sh " + NAME;
  const std::string RESULT_FILE = "./integration/test_data/packages/" + NAME + ".passed";

  std::system(TEST_CMD.c_str());

  std::ifstream result(RESULT_FILE.c_str());
  EXPECT_TRUE(result.good());
}

const char* packages[] = {"math"};

INSTANTIATE_TEST_SUITE_P(Package, IPackageTests, testing::ValuesIn(packages));

/// @}
