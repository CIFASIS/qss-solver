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
./deploy/common/repos.sh pull
head ./doc/version.major -c 4 > vm
git rev-list --count HEAD > rvn
cat vm rvn > version
REV=`cat rvn`
cat ./deploy/osx/qss-solver.ini.in ./deploy/osx/qss-solver.ini 
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
mkdir ./bin/qss-solver.app/Contents/Resources/src

rm -rf tmp
mkdir tmp
./deploy/common/repos.sh deploy tmp
cd ./src
make  
cd ..
cp -r ./tmp/deploy/osx/scripts ./bin/qss-solver.app/Contents/MacOS/
chmod +x ./bin/qss-solver.app/Contents/MacOS/scripts/*
cp src/mmoc/usr/bin/mmoc  ./bin/qss-solver.app/Contents/MacOS/ 
cp src/gui/usr/bin/qss-solver.app/Contents/MacOS/qss-solver ./bin/qss-solver.app/Contents/MacOS/ 
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
cp -r ./tmp/src/engine ./bin/qss-solver.app/Contents/Resources/src/
cp -r ./tmp/src/mmoc ./bin/qss-solver.app/Contents/Resources/src/
cp -r ./tmp/src/gui ./bin/qss-solver.app/Contents/Resources/src/
cp -r ./tmp/src/interfaces ./bin/qss-solver.app/Contents/Resources/src/
cp -r ./tmp/src/usr ./bin/qss-solver.app/Contents/Resources/src/
cp lib/*.a ./bin/qss-solver.app/Contents/Resources/lib
cp /usr/local/lib/libsbml.5.11.4.dylib ./bin/qss-solver.app/Contents/Resources/lib/libsbml.5.dylib
# Clean generated code.
rm -rf ./bin/qss-solver.app/Contents/Resources/src/engine/3rd-party
rm -rf ./bin/qss-solver.app/Contents/Resources/src/engine/usr/obj
rm -rf ./bin/qss-solver.app/Contents/Resources/src/mmoc/usr/obj
rm -rf ./bin/qss-solver.app/Contents/Resources/src/mmoc/usr/bin
rm -rf ./bin/qss-solver.app/Contents/Resources/src/mmoc/usr/share
rm -rf ./bin/qss-solver.app/Contents/Resources/src/mmoc/usr/lib
rm -rf ./bin/qss-solver.app/Contents/Resources/src/mmoc/usr/libexec
rm -rf ./bin/qss-solver.app/Contents/Resources/src/usr/src/*.o
rm -rf ./bin/qss-solver.app/Contents/Resources/src/usr/lib
rm -rf ./bin/qss-solver.app/Contents/Resources/src/interfaces/sbml/usr
rm -rf ./bin/qss-solver.app/Contents/Resources/src/gui/usr
rm -rf deploy/osx/qss-solver.ini
cd ./bin
rm -rf qss-solver.dmg
rm -rf qss-solver.app/Contents/Frameworks/*
rm -rf qss-solver.app/Contents/Resources/qt.conf
macdeployqt qss-solver.app -verbose=2 -dmg
cd ..
mv bin/qss-solver.dmg deploy/osx/
rm rvn vm version
