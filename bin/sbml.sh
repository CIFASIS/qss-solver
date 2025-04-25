#!/bin/bash
#===================================================================================
#
# 				 FILE: sbml.sh  
#
# 				USAGE: sbml.sh [OPTIONS] <file>
#
# 	DESCRIPTION: Runs the SBML model translator with the provided arguments and
# 							 exports the LD_LIBRARY_PATH variable so that the tool can load
# 							 the corresponding dynamic library.
#
#    PARAMETERS: <FILE> xml file containing a SBML model.
#       OPTIONS: [OPTIONS] translate-sbml options (see ./translate-sbml --help).
#  REQUIREMENTS: MMOC_LIBS must point to the default library location for the
# 							 QSS Solver.
#         NOTES: --- 
#        AUTHOR: Joaquin Fernandez, joaquin.f.fernandez@gmail.com
#       PROJECT: QSS Solver
#       VERSION: 4.5.3
#===================================================================================

if [ "$#" -ne 2 ]; then
    echo "Usage: $0 [OPTIONS] <file>"
    exit 1
fi

OPTIONS="$1"
FILE="$2"

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MMOC_LIBS

if ! ./translate-sbml $OPTIONS "$FILE"; then
    echo "Error: Translation failed."
    exit 1
fi
