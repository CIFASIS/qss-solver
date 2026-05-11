#!/bin/bash
#===================================================================================
#
# 				 FILE: exportvars.sh
#
# 				USAGE: exportvars.sh 
#
# 	DESCRIPTION: Exports all the environment variables that the MicroModelica C  
# 							 Compiler uses. 
#
#    PARAMETERS: ---
#       OPTIONS: --- 
#  REQUIREMENTS: --- 
#         NOTES: ---
#        AUTHOR: Joaquin Fernandez, joaquin.f.fernandez@gmail.com
#       PROJECT: QSS Solver
#       VERSION: 6.0.2
#===================================================================================

# Check for the HOME directory
if [ -z "$HOME" ]; then
    echo "Error: HOME environment variable is not set."
    exit 1
fi

# Change to the parent directory and check for success
if ! cd ..; then
    echo "Error: Failed to change directory to the parent directory."
    exit 1
fi

echo "Exporting environment variables..."

# Export environment variables if they are not already set
export MMOC_PATH="${HOME}/qss-solver"
export MMOC_SRC="${MMOC_PATH}/src"
export MMOC_DOC="${MMOC_PATH}/doc"
export MMOC_LIBS="${MMOC_PATH}/lib"
export MMOC_BUILD="${MMOC_PATH}/build"
export MMOC_MODELS="${MMOC_PATH}/models"
export MMOC_OUTPUT="${MMOC_PATH}/output"
export MMOC_BIN="${MMOC_PATH}/bin"
export MMOC_PACKAGES="${MMOC_PATH}/packages"
export MMOC_LIBRARIES="${MMOC_PATH}/lib"
export MMOC_TESTS="${MMOC_SRC}/test-suite"
export MMOC_ENGINE="${MMOC_SRC}/engine/"
export MMOC_INCLUDE="${MMOC_SRC}/usr/include"
export LD_LIBRARY_PATH="${LD_LIBRARY_PATH}:${MMOC_BIN}/lib"

# Change back to the bin directory
if ! cd bin; then
    echo "Error: Failed to change directory back to bin."
    exit 1
fi

echo "Done."
