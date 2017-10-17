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
#       VERSION: 3.2
#===================================================================================

rm -rf qss-solver-i386.deb
rm -rf qss-solver-amd64.deb

cd ../../
ARCH=`uname -m`
echo "Retrieving latest from Git";
./deploy/common/repos.sh pull
VM=`head ./doc/version.major -c 4`
VMC=`head ./doc/version.major -c 3`
if [ "$ARCH" == "x86_64" ]; then
  ./deploy/common/repos.sh tag ${VM} 
fi
REV=`./deploy/common/repos.sh version`
cat ./deploy/linux/qss-solver.ini.in > ./deploy/linux/qss-solver.ini
./deploy/common/setRevision.sh ./deploy/linux/qss-solver.ini $REV $VMC
VER=$VM$REV
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
./deploy/common/repos.sh deploy tmp
cp -r ./tmp/deploy/linux/deb/* ./tmp_deb/
chmod 0755 tmp_deb/DEBIAN/post*
mkdir ./tmp_deb/opt
mkdir ./tmp_deb/opt/qss-solver
mkdir ./tmp_deb/opt/qss-solver/bin
mkdir ./tmp_deb/opt/qss-solver/src
mkdir ./tmp_deb/opt/qss-solver/build
mkdir ./tmp_deb/opt/qss-solver/output
mkdir ./tmp_deb/opt/qss-solver/lib/
cp ./tmp/deploy/linux/scripts/* ./tmp_deb/opt/qss-solver/bin/
if [ "$ARCH" == "i686" ]; then
  cat ./tmp_deb/DEBIAN/control.i386 | awk -v VERSION="$VER" '{ if(index($0,"Version: ")>=1) print "Version: " VERSION ; else print $0;}' >  ./tmp_deb/DEBIAN/control
  rm ./tmp_deb/DEBIAN/control.amd64; 
  rm ./tmp_deb/DEBIAN/control.i386; 
fi
if [ "$ARCH" == "x86_64" ]; then
  cat ./tmp_deb/DEBIAN/control.amd64 | awk -v VERSION="$VER" '{ if(index($0,"Version: ")>=1) print "Version: " VERSION ; else print $0;}' >  ./tmp_deb/DEBIAN/control
  rm ./tmp_deb/DEBIAN/control.amd64; 
  rm ./tmp_deb/DEBIAN/control.i386; 
fi
cp bin/mmoc  ./tmp_deb/opt/qss-solver/bin/ 
cp bin/qss-solver ./tmp_deb/opt/qss-solver/bin/ 
cp bin/translate-sbml  ./tmp_deb/opt/qss-solver/bin/
cp src/engine/3rd-party/partitioners/hmetis/khmetis ./tmp_deb/opt/qss-solver/bin/
cp deploy/linux/qss-solver.ini ./tmp_deb/opt/qss-solver/bin/qss-solver.ini
cp deploy/images/integrator.svg ./tmp_deb/opt/qss-solver/bin/
chmod 0755 `find tmp_deb/opt/qss-solver/bin`
cp LICENSE ./tmp_deb/opt/qss-solver/
cp INSTALL ./tmp_deb/opt/qss-solver/
cp README.md ./tmp_deb/opt/qss-solver/
cp CHANGELOG ./tmp_deb/opt/qss-solver/
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

if [ "$ARCH" == "i686" ]; then
	cp /usr/lib/libsbml.so.5.12.0 ./tmp_deb/opt/qss-solver/lib/libsbml.so.5
	cp src/engine/3rd-party/partitioners/patoh/Linux-i386/libpatoh.a ./tmp_deb/opt/qss-solver/lib/libpatoh.a
	cp src/engine/3rd-party/partitioners/metis/Linux-i386/libmetis.a ./tmp_deb/opt/qss-solver/lib/libmetis.a
	cp src/engine/3rd-party/partitioners/scotch/Linux-i386/libscotch.a ./tmp_deb/opt/qss-solver/lib/libscotch.a
	cp src/engine/3rd-party/partitioners/scotch/Linux-i386/libscotcherr.a ./tmp_deb/opt/qss-solver/lib/libscotcherr.a
fi
if [ "$ARCH" == "x86_64" ]; then
	cp /usr/lib/libsbml.so.5.12.0 ./tmp_deb/opt/qss-solver/lib/libsbml.so.5
	cp src/engine/3rd-party/partitioners/patoh/Linux-x86_64/libpatoh.a ./tmp_deb/opt/qss-solver/lib/libpatoh.a
	cp src/engine/3rd-party/partitioners/metis/Linux-x86_64/libmetis.a ./tmp_deb/opt/qss-solver/lib/libmetis.a
	cp src/engine/3rd-party/partitioners/scotch/Linux-x86_64/libscotch.a ./tmp_deb/opt/qss-solver/lib/libscotch.a
	cp src/engine/3rd-party/partitioners/scotch/Linux-x86_64/libscotcherr.a ./tmp_deb/opt/qss-solver/lib/libscotcherr.a
fi
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
if [ "$ARCH" == "i686" ]; then
  mv qss-solver.deb ./deploy/linux/qss-solver-i386.deb
fi
if [ "$ARCH" == "x86_64" ]; then
  mv qss-solver.deb ./deploy/linux/qss-solver-amd64.deb
fi
rm -rf tmp_deb
rm -rf tmp
cd deploy/linux
