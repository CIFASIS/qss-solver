#!/bin/bash
#===================================================================================
#
# 				 FILE: compile.sh
#
# 				USAGE: compile.sh <FILE> [FOLDER]
#
# 	DESCRIPTION: Creates the default build directory for the QSS Solver GUI 
# 							 and runs the MicroModelica compiler 
#
#    PARAMETERS: <FILE> MicroModelica file to be compiled.
#       OPTIONS: [FOLDER] path where the MicroModelica file is located.
#				 [FLAGS] flags passed to the compiler. 
#  REQUIREMENTS: $MMOC_BUILD must point to the default build directory 
# 							 used by the QSS Solver GUI.
#								 $MMOC_BIN must point to the default bin directory 
# 							 used by the QSS Solver GUI.
#         NOTES: ---
#        AUTHOR: Joaquin Fernandez, joaquin.f.fernandez@gmail.com
#       PROJECT: QSS Solver
#       VERSION: 6.0.2
#===================================================================================

set -e  # Exit immediately if a command exits with a non-zero status.

# Check for required environment variables
if [ -z "$MMOC_BUILD" ]; then
    echo "Error: MMOC_BUILD environment variable is not set."
    exit 1
fi

if [ -z "$MMOC_BIN" ]; then
    echo "Error: MMOC_BIN environment variable is not set."
    exit 1
fi

# Check for required parameters
if [ $# -lt 1 ]; then
    echo "Usage: $0 <FILE> [FOLDER] [FLAGS]"
    exit 1
fi

FILE="$1"
FOLDER="$2"
FLAGS="$3"

# Check if the MicroModelica file exists
if [ -z "$FOLDER" ]; then
    MO_FILE="$FILE.mo"
else
    MO_FILE="$FOLDER/$FILE.mo"
fi

if [ ! -f "$MO_FILE" ]; then
    echo "Error: MicroModelica file '$MO_FILE' does not exist."
    exit 1
fi

# Create the build directory
BUILD_DIR="$MMOC_BUILD/$FILE"
mkdir -p "$BUILD_DIR"
echo "Created build directory: $BUILD_DIR"

# Compile the MicroModelica file
echo "Compiling $MO_FILE..."
if ! "$MMOC_BIN/mmoc" $FLAGS -o "$BUILD_DIR/$FILE" "$MO_FILE"; then
    echo "Error: Compilation failed."
    exit 1
fi

# Change to the build directory
cd "$BUILD_DIR"

# Clean and build
echo "Cleaning and building..."
make -f "$FILE.makefile" clean 
if ! make -f "$FILE.makefile"; then
    echo "Error: Build failed."
    exit 1
fi

echo "Compilation and build completed successfully."
