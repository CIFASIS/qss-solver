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

class IModelTests : public testing::TestWithParam<const char*> {
  public:
  bool checkGeneratedFunctions(const std::string model)
  {
    if (model == "par_airconds_cont") {
      return true;
    }
    return false;
  }
};

TEST_P(IModelTests, GenerateCode)
{
  const std::string NAME = GetParam();
  std::cout << "Testing model: " << NAME << std::endl;
  const std::string MODEL = " ./system/gt_data/" + NAME + "/" + NAME + ".mo";
  const std::string MMOC = "../usr/bin/mmoc";
  const std::string ARGS = " -o ./system/test_data/" + NAME + " -t -i ./system/test_data/packages ";
  const std::string TEST_FUNCTIONS = (checkGeneratedFunctions(NAME)) ? "TEST_FUNCTIONS" : "";
  const std::string TEST_CMD = "./system/test_results.sh " + NAME + " " + TEST_FUNCTIONS;
  const std::string RESULT_FILE = "./system/test_data/" + NAME + ".passed";
  const std::string COMP_CMD = MMOC + ARGS + MODEL + " > ./system/test_data/" + NAME + ".log";

  std::system(COMP_CMD.c_str());
  std::system(TEST_CMD.c_str());

  std::ifstream result(RESULT_FILE.c_str());
  EXPECT_TRUE(result.good());
}

const char* models[] = {"adr",
                        "advection",
                        "advection2D",
                        "advection2D_LI",
                        "advection_quoted",
                        "airconds",
                        "aircont",
                        "bball_downstairs",
                        "boost",
                        "buck",
                        "buckboost",
                        "buck_circuit",
                        "buck_term",
                        "cuk",
                        "cuk2",
                        "interleaved",
                        "inverters",
                        "lc_line",
                        "lotka_volterra",
                        "NeuralNetwork1",
                        "par_airconds",
                        "par_airconds_cont",
                        "rectifier",
                        "rltest",
                        "rltest_LI",
                        "spikings",
                        "VIRplanoS"};

INSTANTIATE_TEST_SUITE_P(Models, IModelTests, testing::ValuesIn(models));

/// @}
