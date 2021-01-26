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
#       VERSION: 3.2
#===================================================================================

OPTIONS=$1

FILE=$2

gnuplot $FILE $OPTIONS
