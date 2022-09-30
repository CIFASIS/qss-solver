#!/bin/bash
#===================================================================================
#
# 				 FILE: build.sh  
#
# 				USAGE: build.sh 
#
# 	DESCRIPTION: Build the Linux deb package including the QSS Solver engine, 
# 							 the QSS Solver GUI, the MicroModelica C Compiler with the 
# 						   corresponding user libraries and the SBML translator tool.
#
#    PARAMETERS: ---
#       OPTIONS: ---
#  REQUIREMENTS: ---
#         NOTES: --- 
#        AUTHOR: Joaquin Fernandez, joaquin.f.fernandez@gmail.com
#       PROJECT: QSS Solver
#       VERSION: 4.0.0
#===================================================================================

rm -rf qss-solver-*.deb

cd ../../
ARCH=`uname -m`
echo "Retrieving latest from Git";
git pull

cat ./deploy/linux/qss-solver.ini.in > ./deploy/linux/qss-solver.ini
# Set solver version
VER=`cat ./deploy/linux/version`
INI_VER="version="$VER
echo $INI_VER >> ./deploy/linux/qss-solver.ini

# Set solver branch
BRANCH=`git rev-parse --abbrev-ref HEAD`
BRANCH="branch="$BRANCH 
echo $BRANCH >> ./deploy/linux/qss-solver.ini

echo "Building QSS Solver DEB package for $ARCH version $VER";
echo "Building Binaries";
cd ./src
make clean
make 
cd ..
rm -rf tmp_deb
rm -rf tmp
mkdir tmp_deb
mkdir tmp
CHECKOUT_PATH=$MMOC_PATH/tmp/
mkdir -p $CHECKOUT_PATH
git checkout-index -a -f --prefix=$CHECKOUT_PATH 

cp -r ./tmp/deploy/linux/deb/* ./tmp_deb/
chmod 0755 tmp_deb/DEBIAN/post*
mkdir ./tmp_deb/opt
mkdir ./tmp_deb/opt/qss-solver
mkdir ./tmp_deb/opt/qss-solver/bin
mkdir ./tmp_deb/opt/qss-solver/src
mkdir ./tmp_deb/opt/qss-solver/build
mkdir ./tmp_deb/opt/qss-solver/output
mkdir ./tmp_deb/opt/qss-solver/lib/

cat ./tmp_deb/DEBIAN/control.amd64 | awk -v VERSION="$VER" '{ if(index($0,"Version: ")>=1) print "Version: " VERSION ; else print $0;}' >  ./tmp_deb/DEBIAN/control
rm ./tmp_deb/DEBIAN/control.amd64; 

cp deploy/linux/version ./tmp_deb/opt/qss-solver/
cp src/mmoc/usr/bin/mmoc  ./tmp_deb/opt/qss-solver/bin/ 
cp src/gui/usr/bin/qss-solver ./tmp_deb/opt/qss-solver/bin/ 
cp src/interfaces/sbml/usr/bin/translate-sbml  ./tmp_deb/opt/qss-solver/bin/
cp src/engine/3rd-party/partitioners/hmetis/khmetis ./tmp_deb/opt/qss-solver/bin/
cp deploy/linux/qss-solver.ini ./tmp_deb/opt/qss-solver/bin/qss-solver.ini
cp deploy/images/integrator.svg ./tmp_deb/opt/qss-solver/bin/
chmod 0755 `find tmp_deb/opt/qss-solver/bin`
cp LICENSE ./tmp_deb/opt/qss-solver/
cp INSTALL ./tmp_deb/opt/qss-solver/
cp README.md ./tmp_deb/opt/qss-solver/
cp CHANGELOG ./tmp_deb/opt/qss-solver/
cp ./tmp/bin/*.sh ./tmp_deb/opt/qss-solver/bin/
cp -r ./tmp/doc ./tmp_deb/opt/qss-solver/
cp -r ./tmp/models ./tmp_deb/opt/qss-solver/
cp -r ./tmp/packages ./tmp_deb/opt/qss-solver/
cp -r ./tmp/src/engine ./tmp_deb/opt/qss-solver/src/
cp -r ./tmp/src/mmoc ./tmp_deb/opt/qss-solver/src/
cp -r ./tmp/src/gui ./tmp_deb/opt/qss-solver/src/
cp -r ./tmp/src/interfaces ./tmp_deb/opt/qss-solver/src/
cp -r ./tmp/src/usr ./tmp_deb/opt/qss-solver/src/
cp lib/*.a ./tmp_deb/opt/qss-solver/lib
# Clean generated code.
rm -rf ./deploy/linux/qss-solver.ini
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

cp /usr/lib/x86_64-linux-gnu/libsbml.so.5.18.0 ./tmp_deb/opt/qss-solver/lib/libsbml.so.5
cp src/engine/3rd-party/partitioners/patoh/Linux-x86_64/libpatoh.a ./tmp_deb/opt/qss-solver/lib/libpatoh.a
cp src/engine/3rd-party/partitioners/metis/Linux-x86_64/libmetis.a ./tmp_deb/opt/qss-solver/lib/libmetis.a

chmod 0644 `find tmp_deb/ -iname *.cpp`
chmod 0644 `find tmp_deb/ -iname *.c`
chmod 0644 `find tmp_deb/ -iname *.h`
chmod 0644 `find tmp_deb/ -iname *.ini`
chmod 0644 `find tmp_deb/ -iname *.png`
chmod 0644 `find tmp_deb/ -iname *.tex`
chmod 0644 `find tmp_deb/opt/qss-solver/doc/ -type f`
chmod 0644 `find tmp_deb/opt/qss-solver/src/ -type f`
chmod 0644 `find tmp_deb/opt/qss-solver/packages/ -type f`
chmod 0644 `find tmp_deb/opt/qss-solver/src/usr/ -type f`
chmod 0755 `find tmp_deb/ -type d`
fakeroot dpkg -b tmp_deb qss-solver.deb
mv qss-solver.deb ./deploy/linux/qss-solver-$VER.deb
rm -rf tmp_deb
rm -rf tmp
cd deploy/linux
