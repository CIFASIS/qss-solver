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
./deploy/common/update-repos.sh
head ./doc/version.major -c 4 > vm
git rev-list --count HEAD > rvn
cat vm rvn > version
REV=`cat rvn`
./deploy/common/setRevision.sh ./deploy/osx/qss-solver.ini $REV
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
mkdir ./bin/qss-solver.app/Contents/Resources/lib

rm -rf tmp
mkdir tmp
./deploy/common/deploy-repos.sh tmp
cd ./src
make  
cd ..
cp -r ./tmp/deploy/osx/scripts ./bin/qss-solver.app/Contents/MacOS/
chmod +x ./bin/qss-solver.app/Contents/MacOS/scripts/*
cp src/mmoc/usr/bin/mmoc  ./bin/qss-solver.app/Contents/MacOS/ 
cp src/interfaces/sbml/usr/bin/translate-sbml  ./bin/qss-solver.app/Contents/MacOS/ 
cp deploy/osx/qss-solver.ini ./bin/qss-solver.app/Contents/MacOS/qss-solver.ini
cp LICENSE  ./bin/qss-solver.app/Contents/MacOS/ 
cp INSTALL  ./bin/qss-solver.app/Contents/MacOS/ 
cp README.md  ./bin/qss-solver.app/Contents/MacOS/ 
cp ChangeLog  ./bin/qss-solver.app/Contents/MacOS/ 
cp -r ./tmp/doc ./bin/qss-solver.app/Contents/Resources/
cp -r ./tmp/models ./bin/qss-solver.app/Contents/Resources/
cp -r ./tmp/packages ./bin/qss-solver.app/Contents/Resources/
cp -r ./tmp/src ./bin/qss-solver.app/Contents/Resources/
cp lib/*.a ./bin/qss-solver.app/Contents/Resources/lib
cp /usr/local/lib/libsbml.5.11.4.dylib ./bin/qss-solver.app/Contents/Resources/usr/libs/libsbml.5.dylib
cd ./bin
rm -rf qss-solver.dmg
rm -rf qss-solver.app/Contents/Frameworks/*
rm -rf qss-solver.app/Contents/Resources/qt.conf
macdeployqt qss-solver.app -verbose=2 -dmg
cd ..
mv bin/qss-solver.dmg deploy/osx/
rm rvn vm version
