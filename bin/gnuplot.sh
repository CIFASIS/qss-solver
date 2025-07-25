#!/bin/bash
#===================================================================================
#
# 				 FILE: gnuplot.sh
#
# 				USAGE: gnuplot.sh [OPTIONS] <FILE> 
#
# 	DESCRIPTION: Runs the gnuplot script <FILE> with options defined in <OPTIONS>  
#
#    PARAMETERS: <FILE> gnuplot script.
#       OPTIONS: [OPTIONS] gnuplot options. 
#  REQUIREMENTS: --- 
#         NOTES: -persist is the default option used by the QSS Solver GUI.
#        AUTHOR: Joaquin Fernandez, joaquin.f.fernandez@gmail.com
#       PROJECT: QSS Solver
#       VERSION: 4.5.3
#===================================================================================

# Check if gnuplot is installed
if ! command -v gnuplot &> /dev/null; then
    echo "Error: gnuplot is not installed. Please install it to proceed."
    exit 1
fi

# Validate input parameters
if [ $# -lt 2 ]; then
    echo "Usage: $0 [OPTIONS] <FILE>"
    exit 1
fi

OPTIONS=$1
FILE=$2

# Check if the specified file exists
if [ ! -f "$FILE" ]; then
    echo "Error: gnuplot script file '$FILE' does not exist."
    exit 1
fi

# Run the gnuplot command
echo "Running gnuplot with file '$FILE' and options '$OPTIONS'..."
gnuplot $FILE $OPTIONS

# Check if gnuplot command was successful
if [ $? -ne 0 ]; then
    echo "Error: gnuplot command failed."
    exit 1
fi

echo "Gnuplot script executed successfully."
