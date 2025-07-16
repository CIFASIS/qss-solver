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

  const std::string TEST_DIR = "../..";
  const std::string SRC_DIR = "../../../..";
  const std::string TEST_DATA_DIR = TEST_DIR + "/system/test_data/";
  const std::string GT_DATA_DIR = TEST_DIR + "/system/gt_data/";

  const std::string MODEL = GT_DATA_DIR + NAME + "/" + NAME + ".mo";
  const std::string MMOC = SRC_DIR + "/mmoc/build/mmoc";
  const std::string ARGS = " -o " + TEST_DATA_DIR + NAME + " -t -i " + GT_DATA_DIR + "packages ";
  const std::string TEST_FUNCTIONS = (checkGeneratedFunctions(NAME)) ? "TEST_FUNCTIONS" : "";
  const std::string TEST_CMD = TEST_DIR + "/system/test_results.sh " + NAME + " " + TEST_FUNCTIONS;
  const std::string RESULT_FILE = TEST_DATA_DIR + NAME + ".passed";
  const std::string COMP_CMD = MMOC + ARGS + MODEL + " >  " + TEST_DATA_DIR + NAME + ".log";

  std::system(COMP_CMD.c_str());
  std::system(TEST_CMD.c_str());

  std::ifstream result(RESULT_FILE.c_str());
  EXPECT_TRUE(result.good());
}

//  const char* models[] = {
//                          "virus_replication"
// };

const char* models[] = {"adr",
                        "adr2D",
                        "advection",
                        "advection2D",
                        "advection2D_LI",
                        "advection_quoted",
                        "advectionFlux",
                        "airconds",
                        "aircont",
                        "allow_discrete_integer",
                        "bball_downstairs",
                        "boost",
                        "BouncingBall",
                        "buck",
                        "buckboost",
                        "buck_circuit",
                        "buck_term",
                        "burgers",
                        "cuk",
                        "cuk2",
                        "init_code_time",
                        "interleaved",
                        "inverters",
                        "lc_line",
                        "lotka_volterra",
                        "mliqss_adr",
                        "mliqss_buck",
                        "mliqss_test",
                        "mliqss_TYSON",
                        "NeuralNetwork1",
                        "par_airconds",
                        "par_airconds_cont",
                        "rectifier",
                        "rltest",
                        "rltest_LI",
                        "spikings",
                        "testFor",
                        "test_input",
                        "TYSON",
                        "VIRplanoS",
                        "virus_replication"};

INSTANTIATE_TEST_SUITE_P(Models, IModelTests, testing::ValuesIn(models));

/// @}
