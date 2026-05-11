#!/bin/bash
#===================================================================================
#
# 				 FILE: createLog.sh
#
# 				USAGE: createLog.sh <MODEL> 
#
# 	DESCRIPTION: Appends log information of different LPS for parallel simulations.
#
#    PARAMETERS: <MODEL> Name of the simulated model.  
#       OPTIONS: --- 
#  REQUIREMENTS: Environment variables MMOC_OUTPUT and MMOC_BIN must point to the 
#                corresponding default directories used by the QSS Solver GUI.
#         NOTES: --- 
#        AUTHOR: Joaquin Fernandez, joaquin.f.fernandez@gmail.com
#       PROJECT: QSS Solver
#       VERSION: 6.0.2
#===================================================================================

# Check for required environment variables
if [ -z "$MMOC_OUTPUT" ]; then
    echo "Error: MMOC_OUTPUT environment variable is not set."
    exit 1
fi

if [ -z "$MMOC_BIN" ]; then
    echo "Error: MMOC_BIN environment variable is not set."
    exit 1
fi

# Check for required parameters
if [ $# -lt 1 ]; then
    echo "Usage: $0 <MODEL>"
    exit 1
fi

MODEL=$1

# Change to the specified directory
if ! cd "$MMOC_OUTPUT/$MODEL"; then
    echo "Error: Directory $MMOC_OUTPUT/$MODEL does not exist."
    exit 1
fi

# Check for log files
if ! ls ${MODEL}-*.log 1> /dev/null 2>&1; then
    echo "No log files found for model '$MODEL'."
    exit 0
fi

# Concatenate log files
for m in ${MODEL}-*.log; do
    cat "$m" >> "${MODEL}.log"
done

# Change back to the bin directory
cd "$MMOC_BIN" 

echo "Log files for model '$MODEL' have been concatenated into '${MODEL}.log'."
