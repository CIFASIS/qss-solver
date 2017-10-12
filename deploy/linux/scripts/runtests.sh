#!/bin/bash
#===================================================================================
#
# 				 FILE: runtests.sh
#
# 				USAGE: runtests.sh 
#
# 	DESCRIPTION: Runs all the test models defined in the MMOC_TESTS directoy,
# 							 control if all the corresponding files are correctly 
# 							 generated and finally compare the results with the ones
# 							 found in the model MMOC_TESTS directory.
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

rm -rf test.log

rm -rf $MMOC_PACKAGES/pkg_*

> $MMOC_BIN/test.log

for fol in `(ls -d */ | grep -v parallel | sed 's/\/$//')`; do
	echo $fol
    cd $fol
	echo "Running test for : " $fol >> $MMOC_BIN/test.log
	echo "Running test for : " $fol 
	mkdir -p $MMOC_OUTPUT/$fol
	rm -rf $MMOC_OUTPUT/$fol/*
	mkdir -p $MMOC_BUILD/$fol
	rm -rf $MMOC_BUILD/$fol/*
	$MMOC_BIN/runqss.sh $fol $MMOC_TESTS/$fol
	if [ ! -f $MMOC_BUILD/$fol/$fol.c ]; then
		echo "ERROR Test: "$fol "Can't generate c file." >> $MMOC_BIN/test.log
		echo "ERROR Test: "$fol "Can't generate c file." 
		continue
	fi
	if [ ! -f $MMOC_BUILD/$fol/$fol.makefile ]; then
		echo "ERROR Test: "$fol "Can't generate makefile." >> $MMOC_BIN/test.log
		diff $MMOC_BUILD/$fol/$fol.c $MMOC_TESTS/$fol/$fol.c >> $MMOC_BIN/test.log
		echo "ERROR Test: "$fol "Can't generate makefile." 
		continue
	fi
	if [ ! -f $MMOC_BUILD/$fol/$fol ]; then
		echo "ERROR Test: "$fol "Can't generate executable file." >> $MMOC_BIN/test.log
		diff $MMOC_BUILD/$fol/$fol.c $MMOC_TESTS/$fol/$fol.c >> $MMOC_BIN/test.log
		echo "ERROR Test: "$fol "Can't generate executable file." 
		continue
	fi
	cd $MMOC_OUTPUT/$fol
	for m in `ls *.dat`; do
		diff $MMOC_OUTPUT/$fol/$m $MMOC_TESTS/$fol/$m > /dev/null 
		if [ $? -ne 0 ]; then
			echo "ERROR Test: "$fol "Different output values in file: "$m  >> $MMOC_BIN/test.log
			echo "ERROR Test: "$fol "Different output values in file: "$m  
		    diff $MMOC_OUTPUT/$fol/$m $MMOC_TESTS/$fol/$m >> $MMOC_BIN/test.log
		else
			echo  >> $MMOC_BIN/test.log
			echo  
			echo "File: "$m "OK." >> $MMOC_BIN/test.log
			echo "File: "$m "OK." 
		fi
 	done
	echo "Finished " >> $MMOC_BIN/test.log
	echo "Finished " 
	echo  >> $MMOC_BIN/test.log
	echo 
	cd $MMOC_TESTS 
done

cd parallel

for fol in `ls`; do
	cd $fol
	echo "Running Parallel test for : " $fol >> $MMOC_BIN/test.log
	echo "Running Parallel test for : " $fol 
	mkdir -p $MMOC_OUTPUT/$fol
	rm -rf $MMOC_OUTPUT/$fol/*
	mkdir -p $MMOC_BUILD/$fol
	rm -rf $MMOC_BUILD/$fol/*
	cp $MMOC_TESTS/parallel/$fol/*.c $MMOC_BUILD/$fol/  
	$MMOC_BIN/runqss.sh $fol $MMOC_TESTS/parallel/$fol true
	if [ ! -f $MMOC_BUILD/$fol/$fol.c ]; then
		echo "ERROR Parallel Test: "$fol "Can't generate c file." >> $MMOC_BIN/test.log
		echo "ERROR Parallel Test: "$fol "Can't generate c file." 
		continue
	fi
	if [ ! -f $MMOC_BUILD/$fol/$fol.makefile ]; then
		echo "ERROR Parallel Test: "$fol "Can't generate makefile." >> $MMOC_BIN/test.log
		diff $MMOC_BUILD/$fol/$fol.c $MMOC_TESTS/$fol/$fol.c >> $MMOC_BIN/test.log
		echo "ERROR Parallel Test: "$fol "Can't generate makefile." 
		continue
	fi
	if [ ! -f $MMOC_BUILD/$fol/$fol ]; then
		echo "ERROR Parallel Test: "$fol "Can't generate executable file." >> $MMOC_BIN/test.log
		diff $MMOC_BUILD/$fol/$fol.c $MMOC_TESTS/$fol/$fol.c >> $MMOC_BIN/test.log
		echo "ERROR Parallel Test: "$fol "Can't generate executable file." 
		continue
	fi
	echo "Finished " >> $MMOC_BIN/test.log
	echo "Finished " 
	echo  >> $MMOC_BIN/test.log
	echo 
	cd $MMOC_TESTS/parallel 
done

if grep -q ERROR "$MMOC_BIN/test.log"; then
  echo ERROR: Check "$MMOC_BIN/test.log" 
else
	echo All tests finished OK.
fi
