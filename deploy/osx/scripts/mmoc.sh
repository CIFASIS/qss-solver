#!/bin/bash
#===================================================================================
#
# 				 FILE: mmoc.sh
#
# 				USAGE: mmoc.sh [OPTIONS] <FILE> 
#
# 	DESCRIPTION: Runs the MicroModelica compiler on file <FILE> 
#                with options defined in <OPTIONS>  
#
#    PARAMETERS: <FILE> MicroModelica file.
#       OPTIONS: [OPTIONS] MicroModelica compiler options (see ./mmoc --help). 
#  REQUIREMENTS: The environment variable MMOC_BIN must be defined.  
#         NOTES: --- 
#        AUTHOR: Joaquin Fernandez, joaquin.f.fernandez@gmail.com
#       PROJECT: QSS Solver
#       VERSION: 3.2
#===================================================================================

OPTIONS=$1

FILE=$2

$MMOC_BIN/mmoc $OPTIONS $FILE
