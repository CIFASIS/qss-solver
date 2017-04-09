#!/bin/bash
#===================================================================================
#
# 				 FILE: exportvars.sh
#
# 				USAGE: exportvars.sh 
#
# 	DESCRIPTION: Exports all the environment variables that the MicroModelica C  
# 							 Compiler uses. 
#
#    PARAMETERS: ---
#       OPTIONS: --- 
#  REQUIREMENTS: --- 
#         NOTES: ---
#        AUTHOR: Joaquin Fernandez, joaquin.f.fernandez@gmail.com
#       PROJECT: QSS Solver
#       VERSION: 3.1
#===================================================================================

cd ..
echo Export environment variables...
export MMOC_PATH=`pwd` 
echo $MMOC_PATH
echo `pwd`
export MMOC_ENGINE=$MMOC_PATH/src/engine/
export MMOC_LIBS=$MMOC_PATH/src/libs
export MMOC_BUILD=$MMOC_PATH/build
export MMOC_MODELS=$MMOC_PATH/models
export MMOC_OUTPUT=$MMOC_PATH/output
export MMOC_BIN=$MMOC_PATH/bin
export MMOC_PACKAGES=$MMOC_PATH/testsuite
export MMOC_LIBRARIES=$MMOC_PATH/usr/libs
export MMOC_INCLUDE=$MMOC_PATH/usr/include
export MMOC_TESTS=$MMOC_PATH/tests
export MMOC_SRC=$MMOC_PATH/src
export MMOC_DOC=$MMOC_PATH/doc
cd bin
echo Done.
