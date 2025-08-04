#!/bin/bash
#===================================================================================
#
# 				 FILE: build.sh
#
# 				USAGE: build.sh <file>
#
# 	DESCRIPTION: Move to the $MMOC_BUILD directory and run the MicroModelica 
# 							 C Compiler generated makefile. 
#
#    PARAMETERS: <file> must be the name of a previously compiled 
#  							 MicroModelica file.  
#  REQUIREMENTS: $MMOC_BUILD must point to the default build directory 
# 							 used by the QSS Solver GUI.
#         NOTES: ---
#        AUTHOR: Joaquin Fernandez, joaquin.f.fernandez@gmail.com
#       PROJECT: QSS Solver
#       VERSION: 5.0.0
#===================================================================================

# Check if a parameter is provided
if [ -z "$1" ]; then
    echo "Usage: $0 <file>"
    exit 1
fi

FILE=$1

# Check if the MMOC_BUILD environment variable is set
if [ -z "$MMOC_BUILD" ]; then
    echo "Error: MMOC_BUILD environment variable is not set."
    exit 1
fi

# Change to the specified directory
if ! cd "$MMOC_BUILD/$FILE"; then
    echo "Error: Directory $MMOC_BUILD/$FILE does not exist."
    exit 1
fi

# Run the make command
if ! make -f "$FILE.makefile"; then
    echo "Error: Make command failed."
    exit 1
fi

echo "Build completed successfully."
