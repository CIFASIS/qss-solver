#!/bin/bash
#===================================================================================
#
# 				 FILE: build.sh
#
# 				USAGE: build.sh 
#
# 	DESCRIPTION: Build the MAC bundle that contains the QSS Solver engine, 
# 							 the QSS Solver GUI, the MicroModelica C Compiler with the 
# 						   corresponding user libraries and the SBML translator tool.
#
#    PARAMETERS: ---
#       OPTIONS: ---
#  REQUIREMENTS: ---
#         NOTES: --- 
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
./deploy/linux/setRevision.sh ./deploy/mac/qss-solver.ini $REV
VER=`cat version`
echo "Done"
echo "Building QSS Solver MAC bundle version $VER";
echo "Building Binaries";
rm -rf ./bin/qss-solver.app
mkdir ./bin/qss-solver.app
mkdir ./bin/qss-solver.app/Contents
mkdir ./bin/qss-solver.app/Contents/MacOS
mkdir ./bin/qss-solver.app/Contents/Resources
mkdir ./bin/qss-solver.app/Contents/Resources/output
mkdir ./bin/qss-solver.app/Contents/Resources/build
mkdir ./bin/qss-solver.app/Contents/Resources/usr
mkdir ./bin/qss-solver.app/Contents/Resources/usr/libs

rm -rf tmp
mkdir tmp
git checkout-index --prefix=tmp/ -a
cd ./src
make OS=mac  
cd ..
cp -r ./tmp/deploy/mac/scripts ./bin/qss-solver.app/Contents/MacOS/
chmod +x ./bin/qss-solver.app/Contents/MacOS/scripts/*
cp bin/mmoc  ./bin/qss-solver.app/Contents/MacOS/ 
cp bin/translate-sbml  ./bin/qss-solver.app/Contents/MacOS/ 
cp deploy/mac/qss-solver.ini ./bin/qss-solver.app/Contents/MacOS/qss-solver.ini
cp doc/COPYING  ./bin/qss-solver.app/Contents/MacOS/ 
cp doc/INSTALL  ./bin/qss-solver.app/Contents/MacOS/ 
cp doc/README.txt  ./bin/qss-solver.app/Contents/MacOS/ 
cp doc/ChangeLog  ./bin/qss-solver.app/Contents/MacOS/ 
cp -r ./tmp/doc ./bin/qss-solver.app/Contents/Resources/
cp -r ./tmp/models ./bin/qss-solver.app/Contents/Resources/
cp -r ./tmp/usr/* ./bin/qss-solver.app/Contents/Resources/usr/
cp -r ./tmp/packages ./bin/qss-solver.app/Contents/Resources/
cp -r ./tmp/src ./bin/qss-solver.app/Contents/Resources/
cp src/libs/*.a ./bin/qss-solver.app/Contents/Resources/src/libs
cp usr/libs/*.a ./bin/qss-solver.app/Contents/Resources/usr/libs
cp /usr/local/lib/libsbml.5.10.2.dylib ./bin/qss-solver.app/Contents/Resources/usr/libs/libsbml.5.dylib
cd ./bin
rm -rf qss-solver.dmg
rm -rf qss-solver.app/Contents/Frameworks/*
rm -rf qss-solver.app/Contents/Resources/qt.conf
macdeployqt qss-solver.app -verbose=2 -dmg
cd ..
mv bin/qss-solver.dmg deploy/mac/
rm rvn vm version
