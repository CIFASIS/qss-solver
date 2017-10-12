#!/bin/bash
#===================================================================================
#
# 				 FILE: copyTestFiles.sh
#
# 				USAGE: copyTestFiles.sh 
#
# 	DESCRIPTION: Copy all the data files nedded for the test models defined in the
#                MMOC_TESTS directory. 
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

cd $MMOC_TESTS 

for fol in `ls`; do
	cd $fol
	echo "Copying files for test : " $fol >> $MMOC_BIN/test.log
	echo "Copying files for test : " $fol 
	cd $MMOC_OUTPUT/$fol
	rm -rf $MMOC_TESTS/$fol/*.dat
	cp *.dat $MMOC_TESTS/$fol/
  cd $MMOC_BUILD/$fol
	rm -rf $MMOC_TESTS/$fol/${fol}.c
	cp *.c $MMOC_TESTS/$fol/
  cd $MMOC_TESTS
	echo ""
done
cd $MMOC_TESTS 
