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
#       VERSION: 3.2
#===================================================================================

cd ..
echo Export environment variables...
export MMOC_PATH=`pwd` 
export MMOC_SRC=$MMOC_PATH/src
export MMOC_DOC=$MMOC_PATH/doc
export MMOC_LIBS=$MMOC_PATH/lib
export MMOC_BUILD=$MMOC_PATH/build
export MMOC_MODELS=$MMOC_PATH/models
export MMOC_OUTPUT=$MMOC_PATH/output
export MMOC_BIN=$MMOC_PATH/bin
export MMOC_PACKAGES=$MMOC_PATH/packages
export MMOC_LIBRARIES=$MMOC_PATH/lib
export MMOC_TESTS=$MMOC_SRC/test-suite
export MMOC_ENGINE=$MMOC_SRC/engine/
export MMOC_INCLUDE=$MMOC_SRC/usr/include
cd bin
echo Done.
