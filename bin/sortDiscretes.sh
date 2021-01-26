#!/bin/bash
#===================================================================================
#
# 				 FILE: sortDiscretes.sh
#
# 				USAGE: sortDiscretes.sh 
#
# 	DESCRIPTION: Creates a unique sorted file containing the output values of the
# 				 different LPs that calculates the corresponding discrete 
# 				 variable in a parallel simulation.
#
#    PARAMETERS: --- 
#       OPTIONS: --- 
#  REQUIREMENTS: Run this script in the output folder of the simulation. 
#         NOTES: ---
#        AUTHOR: Joaquin Fernandez, joaquin.f.fernandez@gmail.com
#       PROJECT: QSS Solver
#       VERSION: 3.2
#===================================================================================

if test -n "$(find . -maxdepth 1 -name '*-discrete-*.dat' -print -quit)"; then
	for p in $(ls *-discrete-*.dat | cut -d '-' -f 1 | sort -u); do
  		FNAME=$(printf '%q' "$p")
  		sort -k1n ${FNAME}-discrete-*.dat > ${p}.dat
	done
fi
