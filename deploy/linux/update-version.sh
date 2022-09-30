#!/bin/bash
#===================================================================================
#
# 				 FILE: update-version.sh  
#
# 				USAGE: update-version.sh
#
# 	DESCRIPTION: Helper script to update version info in all project related files.
#
#    PARAMETERS: ---
#       OPTIONS: ---
#  REQUIREMENTS: ---
#         NOTES: --- 
#        AUTHOR: Joaquin Fernandez, joaquin.f.fernandez@gmail.com
#       PROJECT: QSS Solver
#===================================================================================

VER=`cat ./version`

# Update deploy files.
sed -i "s/VERSION:.*/VERSION: $VER/" ./build.sh

# Update version in bin scripts.
for s in `ls ../../bin/*.sh`; do
    sed -i "s/VERSION:.*/VERSION: $VER/" $s
done

# Update doxyfiles.
sed -i "s/PROJECT_NUMBER =.*/PROJECT_NUMBER = $VER/" ../../src/engine/QSSSolver.doxyfile
sed -i "s/PROJECT_NUMBER =.*/PROJECT_NUMBER = $VER/" ../../src/gui/QSSSolverGUI.doxyfile
sed -i "s/PROJECT_NUMBER =.*/PROJECT_NUMBER = $VER/" ../../src/mmoc/MMOCompiler.doxyfile

# Update mmoc version info file.
sed -i "s/MicroModelica C Compiler.*/MicroModelica C Compiler $VER\" << endl;/" ../../src/mmoc/main.cpp
