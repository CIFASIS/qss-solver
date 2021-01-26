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
#       VERSION: 3.2
#===================================================================================

MODEL=$1

cd $MMOC_OUTPUT/$MODEL 

for m in `ls ${MODEL}-*.log`; do
	cat $m >> ${MODEL}.log
done

cd $MMOC_BIN 
