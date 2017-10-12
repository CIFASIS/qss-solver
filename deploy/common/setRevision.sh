#!/bin/bash
#===================================================================================
#
# 				 FILE: setRevision.sh  
#
# 				USAGE: setRevision.sh <FILE> <REV> 
#
# 	DESCRIPTION: Set the revision number in the qss-solver configuration file. 
#
#    PARAMETERS: ---
#       OPTIONS: ---
#  REQUIREMENTS: ---
#         NOTES: --- 
#        AUTHOR: Joaquin Fernandez, joaquin.f.fernandez@gmail.com
#       PROJECT: QSS Solver
#       VERSION: 3.2
#===================================================================================


FILE=$1

REV=$2

VER=$3

cat $FILE | awk -v REVISION="$REV" '{ if(index($0,"revision=")>=1) print "revision=" REVISION; else print $0;}' > tmp-cfg.ini

cat tmp-cfg.ini | awk -v REVISION="$VER" '{ if(index($0,"version=")>=1) print "version=" VERSION; else print $0;}' > $FILE

rm tmp-cfg.ini 
