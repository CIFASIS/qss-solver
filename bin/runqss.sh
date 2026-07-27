#!/bin/bash
#===================================================================================
#
# 				 FILE: runqss.sh
#
# 				USAGE: runqss.sh <FILE> <FOLDER> <PARALLEL>
#
# 	DESCRIPTION: Wrapper to the compile and simulate scripts. 
#
#    PARAMETERS: <FILE> MicroModelica file to be compiled.
#    						 <FOLDER> see compile.sh.
#    						 <PARALLEL> see simulate.sh.
#       OPTIONS: --- 
#  REQUIREMENTS: $MMOC_BIN must point to the default bin directory 
# 							 used by the QSS Solver GUI.
#         NOTES: ---
#        AUTHOR: Joaquin Fernandez, joaquin.f.fernandez@gmail.com
#       PROJECT: QSS Solver
#       VERSION: 6.2.0
#===================================================================================

# Check if the required parameters are provided
if [ "$#" -ne 3 ]; then
    echo "Usage: $0 <FILE> <FOLDER> <PARALLEL>"
    exit 1
fi

FILE=$1
FOLDER=$2
PARALLEL=$3

# Compile the MicroModelica file
if ! $MMOC_BIN/compile.sh "$FILE" "$FOLDER" "$PARALLEL"; then
    echo "Compilation failed for file: $FILE"
    exit 1
fi

# Simulate the compiled file
if ! $MMOC_BIN/simulate.sh "$FILE" "$PARALLEL"; then
    echo "Simulation failed for file: $FILE"
    exit 1
fi

echo "Compilation and simulation completed successfully."
