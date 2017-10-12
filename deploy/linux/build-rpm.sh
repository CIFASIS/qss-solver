#!/bin/bash
#===================================================================================
#
# 				 FILE: build-rpm.sh  
#
# 				USAGE: build-rmp.sh 
#
# 	DESCRIPTION: Build the Linux RPM package including the QSS Solver engine, 
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

cd ../../
ARCH=`uname -m`
echo "Retrieving latest from Git";
git pull
head ./doc/version.major -c 4 > vm
git rev-list --count HEAD > rvn
cat vm rvn > version
VER=`cat version`
echo "Building QSS Solver RPM package for $ARCH version $VER";
echo "Building Binaries";
cd ./src
make clean
make 
cd ..
RPMDIR=~/rpmbuild/BUILDROOT/qss-solver-3.1-1.el7.centos.x86_64 
rm -rf ${RPMDIR} 
rm -rf tmp
mkdir tmp
mkdir ${RPMDIR} 
mkdir ${RPMDIR}/opt
mkdir ${RPMDIR}/opt/qss-solver
git checkout-index --prefix=tmp/ -a
mkdir ${RPMDIR}/opt
mkdir ${RPMDIR}/opt/qss-solver
mkdir ${RPMDIR}/opt/qss-solver/bin
mkdir ${RPMDIR}/opt/qss-solver/build
mkdir ${RPMDIR}/opt/qss-solver/output
mkdir ${RPMDIR}/opt/qss-solver/usr/
mkdir ${RPMDIR}/opt/qss-solver/usr/libs
cp ./tmp/deploy/linux/scripts/* ${RPMDIR}/opt/qss-solver/bin/
cp bin/mmoc  ${RPMDIR}/opt/qss-solver/bin/ 
cp bin/qss-solver ${RPMDIR}/opt/qss-solver/bin/ 
cp bin/translate-sbml  ${RPMDIR}/opt/qss-solver/bin/
cp deploy/linux/qss-solver.ini ${RPMDIR}/opt/qss-solver/bin/qss-solver.ini
chmod 0755 `find ${RPMDIR}/opt/qss-solver/bin`
cp doc/COPYING ${RPMDIR}/opt/qss-solver/
cp doc/INSTALL ${RPMDIR}/opt/qss-solver/
cp doc/README.txt ${RPMDIR}/opt/qss-solver/
cp version ${RPMDIR}/opt/qss-solver/
cp -r ./tmp/doc ${RPMDIR}/opt/qss-solver/
cp -r ./tmp/models ${RPMDIR}/opt/qss-solver/
cp -r ./tmp/usr/* ${RPMDIR}/opt/qss-solver/usr/
cp -r ./tmp/packages ${RPMDIR}/opt/qss-solver/
cp -r ./tmp/src ${RPMDIR}/opt/qss-solver/
cp src/libs/*.a ${RPMDIR}/opt/qss-solver/src/libs
cp usr/libs/*.a ${RPMDIR}/opt/qss-solver/usr/libs
cp /usr/lib64/libsbml.so.5.11.2 ${RPMDIR}/opt/qss-solver/src/libs/libsbml.so.5
chmod 0644 `find ${RPMDIR}/ -iname *.cpp`
chmod 0644 `find ${RPMDIR}/ -iname *.c`
chmod 0644 `find ${RPMDIR}/ -iname *.h`
chmod 0644 `find ${RPMDIR}/ -iname *.ini`
chmod 0644 `find ${RPMDIR}/ -iname *.png`
chmod 0644 `find ${RPMDIR}/ -iname *.tex`
chmod 0644 `find ${RPMDIR}/opt/qss-solver/doc/ -type f`
chmod 0644 `find ${RPMDIR}/opt/qss-solver/src/ -type f`
chmod 0644 `find ${RPMDIR}/opt/qss-solver/packages/ -type f`
chmod 0644 `find ${RPMDIR}/opt/qss-solver/usr/ -type f`
chmod 0755 `find ${RPMDIR}/ -type d`
rpmbuild -bb ~/rpmbuild/SPECS/qss-solver.spec
mv ~/rpmbuild/RPMS/x86_64/qss-solver-3.0-1.el7.centos.x86_64.rpm ./deploy/linux/qss-solver-3.1.x86_64.rpm
rm rev version vm
rm -rf ${RPMDIR}
rm -rf tmp
cd deploy/linux
