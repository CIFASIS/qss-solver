#!/bin/bash
#===================================================================================
#
# 				 FILE: simulate.sh
#
# 				USAGE: simulate.sh <FILE> <PARALLEL> <FOLDER>
#
# 	DESCRIPTION: Creates the outut directory for the simulation in the default 
# 							 location, copy the configuration files in the output directory,
# 							 run the simulation executable and finally remove the
# 							 configuration files. 
#
#    PARAMETERS: <FILE> simulation executable file.
#    						 <PARALLEL> True for parallel simulations, False otherwise.
#    						 <FOLDER> Location of the weights files for graph partition algorithms.
#       OPTIONS: --- 
#  REQUIREMENTS: MMOC_OUTPUT MMOC_BUILD must point to the corresponding default 
# 							 directories used by the QSS Solver GUI.
#         NOTES: ---
#        AUTHOR: Joaquin Fernandez, joaquin.f.fernandez@gmail.com
#       PROJECT: QSS Solver
#       VERSION: 3.1
#===================================================================================

FILE=$1

PARALLEL=$2

FOLDER=$3

mkdir -p $MMOC_OUTPUT/$FILE

cp $MMOC_BIN/khmetis $MMOC_OUTPUT/$FILE/

cd $MMOC_OUTPUT/$FILE

rm -rf *.log	

cp	$MMOC_BUILD/$FILE/$FILE.ini .

if [ "$PARALLEL" == "true" ]; then
	if [ -f ${FILE}.ewgts ];
	then
  	cp ${FILE}.ewgts ${FILE}.eweights
	else
		cp $MMOC_BUILD/$FILE/${FILE}.eweights . 
	fi
	if [ -f ${FILE}.hewgts ];
	then
  	cp ${FILE}.hewgts ${FILE}.heweights
	else
		cp $MMOC_BUILD/$FILE/${FILE}.heweights . 
	fi
	cp $MMOC_BUILD/$FILE/${FILE}.graph . 
	cp $MMOC_BUILD/$FILE/${FILE}.hgraph . 
fi

$MMOC_BUILD/$FILE/$FILE

rm -rf hkmetis
rm -rf *.eweights	
rm -rf *.heweights	
rm -rf *.graph	
rm -rf *.hgraph	
rm -rf $FILE.ini  
