#!/bin/bash
#===================================================================================
#
# 				 FILE: createTests.sh
#
# 				USAGE: createTests.sh 
#
# 	DESCRIPTION: Runs all the test models defined in the MMOC_TESTS directoy,
# 							 and then copy the files nedded in the corresponding folder. 
#
#    PARAMETERS: ---
#       OPTIONS: --- 
#  REQUIREMENTS: Environment variables MMOC_BIN, MMOC_TESTS, MMOC_OUTPUT, 
#                MMOC_PACKAGES and MMOC_BUILD must point to the corresponding 
#                default directories used by the QSS Solver GUI.
#         NOTES: The ground truth values for the tests are obtained by the 
# 							 latest stable working version of the solver without using 
# 							 symbolic derivatives and only for the Lunix version.
#        AUTHOR: Joaquin Fernandez, joaquin.f.fernandez@gmail.com
#       PROJECT: QSS Solver
#       VERSION: 3.2
#===================================================================================

./runtests.sh

./copyTestFiles.sh
