#!/bin/bash
#===================================================================================
#
# 				 FILE: build.sh
#
# 				USAGE: build.sh <file>
#
# 	DESCRIPTION: Move to the $MMOC_BUILD directory and run the MicroModelica 
# 							 C Compiler generated makefile. 
#
#    PARAMETERS: <file> must be the name of a previously compiled 
#  							 MicroModelica file.  
#  REQUIREMENTS: $MMOC_BUILD must point to the default build directory 
# 							 used by the QSS Solver GUI.
#         NOTES: ---
#        AUTHOR: Joaquin Fernandez, joaquin.f.fernandez@gmail.com
#       PROJECT: QSS Solver
#       VERSION: 3.2
#===================================================================================

FILE=$1

cd $MMOC_BUILD/$FILE

make -f $FILE.makefile 
