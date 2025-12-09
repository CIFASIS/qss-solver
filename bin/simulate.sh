#!/bin/bash
#===================================================================================
#
# 				 FILE: simulate.sh
#
# 				USAGE: simulate.sh <FILE> <PARALLEL> <WEIGHTS>
#
# 	DESCRIPTION: Creates the output directory for the simulation in the default 
# 							 location, copies the configuration files in the output directory,
# 							 runs the simulation executable, and finally removes the
# 							 configuration files. 
#
#    PARAMETERS: <FILE> simulation executable file.
#    			 <PARALLEL> True for parallel simulations, False otherwise.
#    			 <WEIGHTS> Use semi-static weights file.
#       OPTIONS: --- 
#  REQUIREMENTS: MMOC_OUTPUT MMOC_BUILD must point to the corresponding default 
# 							 directories used by the QSS Solver GUI.
#         NOTES: ---
#        AUTHOR: Joaquin Fernandez, joaquin.f.fernandez@gmail.com
#       PROJECT: QSS Solver
#       VERSION: 6.0.0
#===================================================================================

set -e  # Exit immediately if a command exits with a non-zero status.

FILE="$1"
PARALLEL="$2"
WEIGHTS="$4"

# Create output directory and check for errors
OUTPUT_DIR="$MMOC_OUTPUT/$FILE"
mkdir -p "$OUTPUT_DIR" || { echo "Failed to create output directory: $OUTPUT_DIR"; exit 1; }

# Copy khmetis executable
cp "$MMOC_BIN/khmetis" "$OUTPUT_DIR/"

# Change to output directory
cd "$OUTPUT_DIR"

# Clean up old log files
rm -f *.log	

# Function to copy files if they exist
copy_if_exists() {
    local src="$1"
    local dest="$2"
    
    if [[ -e "$src" ]]; then
        cp "$src" "$dest"
    fi
}

# Copy configuration files and check for errors
copy_if_exists "$MMOC_BUILD/$FILE/$FILE.ini" . || { echo "Failed to copy $FILE.ini"; exit 1; }
copy_if_exists "$MMOC_BUILD/$FILE/$FILE.part" . || { echo "Failed to copy $FILE.part"; exit 1; }
copy_if_exists "$MMOC_BUILD/$FILE/${FILE}_BDF.part" . || { echo "Failed to copy ${FILE}_BDF.part"; exit 1; }
copy_if_exists "$MMOC_BUILD/$FILE/${FILE}.graph" . || { echo "Failed to copy ${FILE}.graph"; exit 1; }
copy_if_exists "$MMOC_BUILD/$FILE/${FILE}.hgraph" . || { echo "Failed to copy ${FILE}.hgraph"; exit 1; }
copy_if_exists "$MMOC_BUILD/$FILE/${FILE}.vweights" . || { echo "Failed to copy ${FILE}.vweights"; exit 1; }
copy_if_exists "$MMOC_BUILD/$FILE/${FILE}.eweights" . || { echo "Failed to copy ${FILE}.eweights"; exit 1; }
copy_if_exists "$MMOC_BUILD/$FILE/${FILE}.heweights" . || { echo "Failed to copy ${FILE}.heweights"; exit 1; }

# Copy weights if specified
if [[ "$WEIGHTS" == "true" ]]; then
    copy_if_exists "${FILE}.vwgts" "${FILE}.vweights"	
    copy_if_exists "${FILE}.ewgts" "${FILE}.eweights"
    copy_if_exists "${FILE}.hewgts" "${FILE}.heweights"
fi

# Run the simulation executable and check for errors
"$MMOC_BUILD/$FILE/$FILE" || { echo "Simulation executable failed to run."; exit 1; }

# Handle parallel simulations
if [[ "$PARALLEL" == "true" ]]; then
    if compgen -G '*-discrete-*.dat' > /dev/null; then
        for p in $(ls *-discrete-*.dat | cut -d '-' -f 1 | sort -u); do
            FNAME=$(printf '%q' "$p")
            sort -k1n "${FNAME}-discrete-*.dat" >> "${FNAME}.dat"
        done
        rm *-discrete-*.dat
    fi
fi

# Clean up
rm -f khmetis
rm -f "$FILE.ini"
