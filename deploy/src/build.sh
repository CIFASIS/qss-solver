#!/bin/bash
#===================================================================================
#
# 				 FILE: build.sh  
#
# 				USAGE: build.sh 
#
# 	DESCRIPTION: Build the tar.gz file that contains source files and libraries.
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
ARCH=`uname -m`
echo "Retrieving latest from Git";
./deploy/common/repos.sh pull
git pull
head ./doc/version.major -c 4 > vm
git rev-list --count HEAD > rvn
cat vm rvn > version
VER=`cat version`
echo "Building QSS Solver tarball file version $VER";
rm -rf qss-solver-${VER}
mkdir qss-solver-${VER}
./deploy/common/repos.sh deploy qss-solver-${VER}
tar -zcvf qss-solver-${VER}.tar.gz qss-solver-${VER}
mv qss-solver-${VER}.tar.gz ./deploy/src/
rm -rf qss-solver-${VER}
rm version vm rvn
