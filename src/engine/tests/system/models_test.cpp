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

/// @addtogroup QssSolverSTests
/// @{

class SModelTests : public testing::TestWithParam<const char*> {
};

TEST_P(SModelTests, SimulateModel)
{
  const std::string NAME = GetParam();
  std::cout << "Testing model: " << NAME << std::endl;
  const std::string MODEL = " ./system/gt_data/" + NAME + "/" + NAME + ".mo";
  const std::string FOLDER_CMD = "mkdir ./system/test_data/" + NAME;
  const std::string MMOC = "../../mmoc/usr/bin/mmoc";
  const std::string ARGS = " -o ./system/test_data/" + NAME + "/" + NAME + " -i ./system/test_data/packages ";
  const std::string TEST_CMD = "./system/test_results.sh " + NAME;
  const std::string SIM_CMD = "cd ./system/test_data/" + NAME + "; ./" + NAME;
  const std::string RESULT_FILE = "./system/test_data/" + NAME + "/" + NAME + ".passed";
  const std::string COMP_CMD = MMOC + ARGS + MODEL + " > ./system/test_data/" + NAME + "/" + NAME + ".log";
  const std::string MAKE_CMD = "make -f ./system/test_data/" + NAME + "/" + NAME + ".makefile";

  std::cout << "Setup data folders for " << NAME << std::endl;
  std::system(FOLDER_CMD.c_str());
  std::cout << "Testing model: " << NAME << std::endl;
  std::system(COMP_CMD.c_str());
  std::cout << "Running make for: " << NAME << std::endl;
  std::system(MAKE_CMD.c_str());
  std::cout << "Simulate model: " << NAME << std::endl;
  std::system(SIM_CMD.c_str());
  std::cout << "Check results for model: " << NAME << std::endl;
  std::system(TEST_CMD.c_str());

  std::ifstream result(RESULT_FILE.c_str());
  EXPECT_TRUE(result.good());
}

const char* models[] = {"acousticsBase",
                        "acousticsRiemann",
                        "acousticsRiemDes",
                        "adr",
                        "adr2D",
                        "advection",
                        "advection2D",
                        "advectionFlux",
                        "advectionUpwind",
                        "airconds",
                        "aircondsDOPRI",
                        "aircont",
                        "ar2d2",
                        "bball_downstairs",
                        "BouncingBall",
                        "boost",
                        "buck",
                        "buckboost",
                        "buck_circuit",
                        "buck_term",
                        "burgers",
                        "cuk",
                        "cuk2",
                        "cukDASSL",
                        "GOLDBETER",
                        "interleaved",
                        "inverters",
                        "invertersDOPRI",
                        "mliqss_adr",
                        "mliqss_buck",
                        "mliqss_test",
                        "mliqss_TYSON",
                        "lc_line",
                        "lotka_volterra",
                        "lotka_volterraDASSL",
                        "lotka_volterraDOPRI",
                        "par_air_conds",
                        "rectifier",
                        "rectifierDASSL",
                        "rectifierLIQSS3",
                        "rltest",
                        "spikings",
                        "testFor",
                        "test_input",
                        "TYSON",
                        "VIRplanoS"};

INSTANTIATE_TEST_SUITE_P(Models, SModelTests, testing::ValuesIn(models));

/// @}
