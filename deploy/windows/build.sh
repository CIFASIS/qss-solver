#!/bin/bash
#===================================================================================
#
# 				 FILE: build.sh  
#
# 				USAGE: build.sh 
#
# 	DESCRIPTION: Build the Windows installer including the QSS Solver engine, 
# 							 the QSS Solver GUI, the MicroModelica C Compiler with the 
# 						   corresponding user libraries and the SBML translator tool.
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
echo "Retrieving latest from Git";
git pull
head ./doc/version.major -c 4 > vm
git rev-list --count HEAD > rvn
cat vm rvn > version
REV=`cat rvn`
./deploy/linux/setRevision.sh ./deploy/windows/qss-solver.ini $REV 
VER=`cat version`
echo Building Windows Package version $VER
echo Creating directories
rm -rf tmp-win-installer 
rm -rf tmp 
mkdir tmp
mkdir tmp-win-installer
mkdir ./tmp-win-installer/qss-solver
mkdir ./tmp-win-installer/qss-solver/bin
mkdir ./tmp-win-installer/qss-solver/build
mkdir ./tmp-win-installer/qss-solver/output
mkdir ./tmp-win-installer/qss-solver/usr
mkdir ./tmp-win-installer/qss-solver/usr/libs
echo Done.

echo Exporting files
git checkout-index --prefix=tmp/ -a
cp -r ./tmp/deploy/windows/export/*  ./tmp-win-installer/
cp -r ./tmp/models ./tmp-win-installer/qss-solver/
cp -r ./tmp/src ./tmp-win-installer/qss-solver/
cp -r ./tmp/usr/* ./tmp-win-installer/qss-solver/usr/
cp -r ./tmp/packages ./tmp-win-installer/qss-solver/
cp -r ./tmp/doc ./tmp-win-installer/qss-solver/
cp doc/COPYING ./tmp-win-installer/qss-solver/
cp doc/INSTALL ./tmp-win-installer/qss-solver/
cp doc/README.txt ./tmp-win-installer/qss-solver/
cp doc/ChangeLog ./tmp-win-installer/qss-solver/
cp deploy/images/integrator.svg ./tmp-win-installer/qss-solver/bin
cp deploy/windows/qss-solver.ini ./tmp-win-installer/qss-solver/bin
cp deploy/windows/qss-solver.gpr ./tmp-win-installer
echo Done.
echo Building binaries
cd src
make OS=win32 
cd ..
echo Done.
echo Copying executables files 
cd bin
cp qss-solver.exe ../tmp-win-installer/qss-solver/bin/
cp mmoc.exe ../tmp-win-installer/qss-solver/bin/
cp translate-sbml.exe ../tmp-win-installer/qss-solver/bin/
echo Done.
echo Copying libraries
cd ../src/libs
cp *.a ../../tmp-win-installer/qss-solver/src/libs/
cp ../../usr/libs/*.a ../../tmp-win-installer/qss-solver/usr/libs/
cd ../../tmp-win-installer
mv ./qss-solver/bin/*.a ./qss-solver/src/libs/
echo Done.
echo Creating installer
gibuild qss-solver.gpr
cp ./Output/qss-solver-install.exe ../
cd ..
rm rvn vm version
rm -rf tmp-win-installer 
mv qss-solver-install.exe deploy/windows/
echo Done.
