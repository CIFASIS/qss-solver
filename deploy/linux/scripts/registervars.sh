#!/bin/bash
#===================================================================================
#
# 				 FILE: registervars.sh
#
# 				USAGE: registervars.sh 
#
# 	DESCRIPTION: Register all the environment variables that the MicroModelica C  
# 							 Compiler uses in .bashrc. 
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
sed -i.bak '/MMOC/d' ~/.bashrc
echo 'export MMOC_PATH='`pwd`'' >> ~/.bashrc
echo 'export MMOC_ENGINE=$MMOC_PATH/src/engine/' >> ~/.bashrc
echo 'export MMOC_LIBS=$MMOC_PATH/src/libs' >> ~/.bashrc
echo 'export MMOC_BUILD=$MMOC_PATH/build' >> ~/.bashrc
echo 'export MMOC_MODELS=$MMOC_PATH/models' >> ~/.bashrc
echo 'export MMOC_OUTPUT=$MMOC_PATH/output' >> ~/.bashrc
echo 'export MMOC_BIN=$MMOC_PATH/bin' >> ~/.bashrc
echo 'export MMOC_TESTS=$MMOC_PATH/testsuite' >> ~/.bashrc
echo 'export MMOC_PACKAGES=$MMOC_PATH/packages' >> ~/.bashrc
echo 'export MMOC_LIBRARIES=$MMOC_PATH/usr/libs' >> ~/.bashrc
echo 'export MMOC_INCLUDE=$MMOC_PATH/usr/include' >> ~/.bashrc
echo 'export MMOC_SRC=$MMOC_PATH/src' >> ~/.bashrc
echo 'export MMOC_DOC=$MMOC_PATH/doc' >> ~/.bashrc
cd bin
echo Done.
