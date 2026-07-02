#!/bin/bash
#===================================================================================
#
# 				 FILE: mmoc.sh
#
# 				USAGE: mmoc.sh <FILE> [OPTIONS]
#
# 	DESCRIPTION: Runs the MicroModelica compiler on file <FILE> 
#                with options defined in [OPTIONS]  
#
#    PARAMETERS: <FILE> MicroModelica file.
#       OPTIONS: [OPTIONS] MicroModelica compiler options (see ./mmoc --help). 
#  REQUIREMENTS: The environment variable MMOC_BIN must be defined.  
#         NOTES: --- 
#        AUTHOR: Joaquin Fernandez, joaquin.f.fernandez@gmail.com
#       PROJECT: QSS Solver
#       VERSION: 6.0.2
#===================================================================================

# Check if MMOC_BIN is set
if [ -z "$MMOC_BIN" ]; then
    echo "Error: MMOC_BIN environment variable is not set."
    exit 1
fi

# Validate input parameters
if [ $# -lt 2 ]; then
    echo "Usage: $0 <FILE> [OPTIONS]"
    exit 1
fi

FILE=$1
OPTIONS=$2

# Check if the specified file exists
if [ ! -f "$FILE" ]; then
    echo "Error: MicroModelica file '$FILE' does not exist."
    exit 1
fi

# Run the MicroModelica compiler
echo "Running MicroModelica compiler on file '$FILE' with options '$OPTIONS'..."
export LD_LIBRARY_PATH="${LD_LIBRARY_PATH}:${MMOC_BIN}/lib"
$MMOC_BIN/mmoc $OPTIONS $FILE

# Check if the mmoc command was successful
if [ $? -ne 0 ]; then
    echo "Error: MicroModelica compiler command failed."
    exit 1
fi

echo "MicroModelica compilation completed successfully."
