#!/bin/bash
#===================================================================================
#
# 				 FILE: registervars.sh
#
# 				USAGE: registervars.sh 
#
# 	DESCRIPTION: Register all the environment variables that the MicroModelica C  
# 							 Compiler uses in .profile. 
#
#    PARAMETERS: ---
#       OPTIONS: --- 
#  REQUIREMENTS: --- 
#         NOTES: ---
#        AUTHOR: Joaquin Fernandez, joaquin.f.fernandez@gmail.com
#       PROJECT: QSS Solver
#       VERSION: 3.2
#===================================================================================

cd ../../
echo Export environment variables...
sed -i.bak '/MMOC/d' ~/.profile
echo 'export MMOC_PATH='`pwd`'' >> ~/.profile
echo 'export MMOC_SRC=$MMOC_PATH/Resources/src' >> ~/.profile
echo 'export MMOC_LIBS=$MMOC_PATH/Resources/lib' >> ~/.profile
echo 'export MMOC_BUILD=$MMOC_PATH/Resources/build' >> ~/.profile
echo 'export MMOC_MODELS=$MMOC_PATH/Resources/models' >> ~/.profile
echo 'export MMOC_OUTPUT=$MMOC_PATH/Resources/output' >> ~/.profile
echo 'export MMOC_BIN=$MMOC_PATH/MacOS' >> ~/.profile
echo 'export MMOC_PACKAGES=$MMOC_PATH/Resources/packages' >> ~/.profile
echo 'export MMOC_LIBRARIES=$MMOC_PATH/Resources/lib' >> ~/.profile
echo 'export MMOC_TESTS=$MMOC_SRC/test-suite' >> ~/.profile
echo 'export MMOC_INCLUDE=$MMOC_SRC/usr/include' >> ~/.profile
echo 'export MMOC_ENGINE=$MMOC_SRC/engine/' >> ~/.profile
cd ./MacOS/scripts
echo Done.
