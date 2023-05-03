#!/bin/bash
#===================================================================================
#
# 				 FILE: compile.sh
#
# 				USAGE: compile.sh <FILE> [FOLDER]
#
# 	DESCRIPTION: Creates the default build directory for the QSS Solver GUI 
# 							 and runs the MicroModelica compiler 
#
#    PARAMETERS: <FILE> MicroModelica file to be compiled.
#       OPTIONS: [FOLDER] path where the MicroModelica file is located.
#				 [FLAGS] flags passed to the compiler. 
#  REQUIREMENTS: $MMOC_BUILD must point to the default build directory 
# 							 used by the QSS Solver GUI.
#								 $MMOC_BIN must point to the default bin directory 
# 							 used by the QSS Solver GUI.
#         NOTES: ---
#        AUTHOR: Joaquin Fernandez, joaquin.f.fernandez@gmail.com
#       PROJECT: QSS Solver
#       VERSION: 4.1.3
#===================================================================================

FILE=$1

FOLDER=$2

FLAGS=$3

mkdir -p $MMOC_BUILD/$FILE

if [ -z $FOLDER ]; then
	$MMOC_BIN/mmoc $FLAGS -o $MMOC_BUILD/$FILE/$FILE $FILE.mo
else
	$MMOC_BIN/mmoc $FLAGS -o $MMOC_BUILD/$FILE/$FILE $FOLDER/$FILE.mo
fi

cd $MMOC_BUILD/$FILE

make -f $FILE.makefile clean 

make -f $FILE.makefile 
