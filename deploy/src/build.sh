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
#       VERSION: 3.1
#===================================================================================

cd ../../
ARCH=`uname -m`
echo "Retrieving latest from Git";
git pull
head ./doc/version.major -c 4 > vm
git rev-list --count HEAD > rvn
cat vm rvn > version
VER=`cat version`
echo "Building QSS Solver tarball file version $VER";
git archive --format=tar.gz --prefix=qss-solver-${VER}/ HEAD > qss-solver-${VER}.tar.gz 
mv qss-solver-${VER}.tar.gz ./deploy/src/
rm version vm rvn
