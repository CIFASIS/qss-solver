#!/bin/bash
#===================================================================================
#
# 				 FILE: setRevision.sh  
#
# 				USAGE: setRevision.sh <FILE> <REV> 
#
# 	DESCRIPTION: Set the revision number in the qss-solverconfiguration file. 
#
#    PARAMETERS: ---
#       OPTIONS: ---
#  REQUIREMENTS: ---
#         NOTES: --- 
#        AUTHOR: Joaquin Fernandez, joaquin.f.fernandez@gmail.com
#       PROJECT: QSS Solver
#       VERSION: 3.1
#===================================================================================


FILE=$1

REV=$2

cat $FILE | awk -v REVISION="$REV" '{ if(index($0,"revision=")>=1) print "revision=" REVISION; else print $0;}' > tmp-cfg.ini

mv tmp-cfg.ini $FILE
