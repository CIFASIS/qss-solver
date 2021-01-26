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
#       VERSION: 3.2
#===================================================================================

OPTIONS=$1

FILE=$2


export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MMOC_LIBS

./translate-sbml $OPTIONS $FILE
