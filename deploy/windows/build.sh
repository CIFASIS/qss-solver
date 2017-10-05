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
./deploy/common/repos.sh pull
head ./doc/version.major -c 4 > vm
git rev-list --count HEAD > rvn
cat vm rvn > version
REV=`cat rvn`
cat ./deploy/windows/qss-solver.ini.in > ./deploy/windows/qss-solver.ini
./deploy/common/setRevision.sh ./deploy/windows/qss-solver.ini $REV 
VER=`cat version`
echo Building Windows Package version $VER
echo Creating directories
rm -rf tmp-win-installer 
rm -rf tmp 
mkdir tmp
mkdir tmp-win-installer
mkdir ./tmp-win-installer/qss-solver
mkdir ./tmp-win-installer/qss-solver/bin
mkdir ./tmp-win-installer/qss-solver/src
mkdir ./tmp-win-installer/qss-solver/build
mkdir ./tmp-win-installer/qss-solver/output
mkdir ./tmp-win-installer/qss-solver/lib
echo Done.

echo Exporting files
./deploy/common/repos.sh deploy tmp 
cp -r ./tmp/deploy/windows/export/*  ./tmp-win-installer/
cp -r ./tmp/models ./tmp-win-installer/qss-solver/
cp -r ./tmp/packages ./tmp-win-installer/qss-solver/
cp -r ./tmp/doc ./tmp-win-installer/qss-solver/
cp -r ./tmp/src/engine ./tmp-win-installer/qss-solver/src/
cp -r ./tmp/src/mmoc ./tmp-win-installer/qss-solver/src/
cp -r ./tmp/src/gui ./tmp-win-installer/qss-solver/src/
cp -r ./tmp/src/interfaces ./tmp-win-installer/qss-solver/src/
cp -r ./tmp/src/usr ./tmp-win-installer/qss-solver/src/
cp LICENSE ./tmp-win-installer/qss-solver/
cp INSTALL ./tmp-win-installer/qss-solver/
cp README.md ./tmp-win-installer/qss-solver/
cp CHANGELOG ./tmp-win-installer/qss-solver/
cp deploy/images/integrator.svg ./tmp-win-installer/qss-solver/bin
cp deploy/windows/qss-solver.ini ./tmp-win-installer/qss-solver/bin
cp deploy/windows/qss-solver.gpr ./tmp-win-installer
# Code cleanup.
rm -rf ./deploy/windows/qss-solver.ini
rm -rf ./tmp_deb/opt/qss-solver/src/engine/3rd-party
rm -rf ./tmp_deb/opt/qss-solver/src/engine/usr/obj
rm -rf ./tmp_deb/opt/qss-solver/src/mmoc/usr/obj
rm -rf ./tmp_deb/opt/qss-solver/src/mmoc/usr/bin
rm -rf ./tmp_deb/opt/qss-solver/src/mmoc/usr/share
rm -rf ./tmp_deb/opt/qss-solver/src/mmoc/usr/lib
rm -rf ./tmp_deb/opt/qss-solver/src/mmoc/usr/libexec
rm -rf ./tmp_deb/opt/qss-solver/src/usr/src/*.o
rm -rf ./tmp_deb/opt/qss-solver/src/usr/lib
rm -rf ./tmp_deb/opt/qss-solver/src/interfaces/sbml/usr
rm -rf ./tmp_deb/opt/qss-solver/src/gui/usr
echo Done.
echo Building binaries
cd src
make clean
make 
cd ..
echo Done.
echo Copying executables files 
cp src/gui/usr/bin/qss-solver.exe tmp-win-installer/qss-solver/bin/
cp src/mmoc/usr/bin/mmoc.exe tmp-win-installer/qss-solver/bin/
cp src/interfaces/sbml/usr/bin/translate-sbml.exe tmp-win-installer/qss-solver/bin/
echo Done.
echo Copying libraries
cp src/engine/usr/lib/*.a tmp-win-installer/qss-solver/lib/
cp src/usr/lib/*.a tmp-win-installer/qss-solver/lib/
rm -rf rvn
rm -rf version
rm -rf vm
cd tmp-win-installer
mv ./qss-solver/bin/*.a ./qss-solver/lib/
echo Done.
echo Build finished, now create the installer.
