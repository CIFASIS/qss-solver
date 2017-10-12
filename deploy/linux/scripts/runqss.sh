#!/bin/bash
#===================================================================================
#
# 				 FILE: runqss.sh
#
# 				USAGE: runqss.sh <FILE> <FOLDER> <PARALLEL>
#
# 	DESCRIPTION: Wrapper to the compile and simulate scripts. 
#
#    PARAMETERS: <FILE> MicroModelica file to be compiled.
#    						 <FOLDER> see compile.sh.
#    						 <PARALLEL> see simulate.sh.
#       OPTIONS: --- 
#  REQUIREMENTS: $MMOC_BIN must point to the default bin directory 
# 							 used by the QSS Solver GUI.
#         NOTES: ---
#        AUTHOR: Joaquin Fernandez, joaquin.f.fernandez@gmail.com
#       PROJECT: QSS Solver
#       VERSION: 3.2
#===================================================================================

FILE=$1

FOLDER=$2

PARALLEL=$3

$MMOC_BIN/compile.sh $FILE $FOLDER $PARALLEL

$MMOC_BIN/simulate.sh $FILE $PARALLEL
