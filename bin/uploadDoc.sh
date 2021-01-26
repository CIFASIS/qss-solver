#!/bin/bash
#===================================================================================
#
# 				 FILE: uploadDoc.sh
#
# 				USAGE: uploadDoc.sh 
#
# 	DESCRIPTION: Upload the project generated documentation files to the
#                CIFASIS server. 
#
#    PARAMETERS: ---  
#  REQUIREMENTS: ---
#         NOTES: ---
#        AUTHOR: Joaquin Fernandez, joaquin.f.fernandez@gmail.com
#       PROJECT: QSS Solver
#       VERSION: 3.2
#===================================================================================

cd $MMOC_SRC/engine/usr/doc/html

SSH_AUTH_SOCK=0 scp -r * jfernandez@www.cifasis-conicet.gov.ar:/home/jfernandez/public_html/engine/

cd $MMOC_SRC/mmoc/usr/doc/html

SSH_AUTH_SOCK=0 scp -r * jfernandez@www.cifasis-conicet.gov.ar:/home/jfernandez/public_html/mmoc/

cd $MMOC_SRC/gui/usr/doc/html

SSH_AUTH_SOCK=0 scp -r * jfernandez@www.cifasis-conicet.gov.ar:/home/jfernandez/public_html/gui/

cd $MMOC_SRC/interfaces/sbml/usr/doc/html

SSH_AUTH_SOCK=0 scp -r * jfernandez@www.cifasis-conicet.gov.ar:/home/jfernandez/public_html/sbml/
