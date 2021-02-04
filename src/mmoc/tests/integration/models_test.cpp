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

/// @file advection_model_test.cpp
///

/// @addtogroup QssSolverITests
/// @{

class ITests : public testing::TestWithParam<const char*> {
};

TEST_P(ITests, GenerateCode)
{
  const std::string NAME = GetParam();
  std::cout << "Testing model: " << NAME << std::endl;
  const std::string MODEL = " ./integration/gt_data/" + NAME + "/" + NAME + ".mo";
  const std::string MMOC = "../usr/bin/mmoc";
  const std::string ARGS = " -o ./integration/test_data/" + NAME + " --test";
  const std::string TEST_CMD = "./integration/test_results.sh " + NAME;
  const std::string RESULT_FILE = "./integration/test_data/" + NAME + ".passed";
  const std::string COMP_CMD = MMOC + ARGS + MODEL + " > ./integration/test_data/" + NAME + ".log";

  std::system(COMP_CMD.c_str());
  std::system(TEST_CMD.c_str());

  std::ifstream result(RESULT_FILE.c_str());
  EXPECT_TRUE(result.good());
}

// const char* models[] = {"advection", "lc_line", "lotka_volterra", "bball_downstairs", "airconds",  "aircont",   "boost",   "buck",
//                        "buckboost", "cuk",     "cuk2",           ,     "inverters", "rectifier", "spikings"};
const char* models[] = {"advection", "advection2D", "bball_downstairs", "boost",          "buck",      "buckboost", "buck_circuit",
                        "cuk",       "cuk2",        "lc_line",          "lotka_volterra", "rectifier", "rltest", "spikings"};

INSTANTIATE_TEST_SUITE_P(Models, ITests, testing::ValuesIn(models));

/// @}
