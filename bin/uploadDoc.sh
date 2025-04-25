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
#       VERSION: 4.5.3
#===================================================================================

# Check if MMOC_SRC is set
if [ -z "$MMOC_SRC" ]; then
    echo "Error: MMOC_SRC is not set."
    exit 1
fi

upload_docs() {
    local dir=$1
    local dest=$2
    cd "$dir" || { echo "Error: Cannot change to directory $dir"; exit 1; }

    SSH_AUTH_SOCK=0 scp -r * jfernandez@www.cifasis-conicet.gov.ar:"$dest"
}

upload_docs "$MMOC_SRC/engine/usr/doc/html" "/var/www/jfernandez/public_html/engine/"
upload_docs "$MMOC_SRC/mmoc/usr/doc/html" "/var/www/jfernandez/public_html/mmoc/"
upload_docs "$MMOC_SRC/gui/usr/doc/html" "/var/www/jfernandez/public_html/gui/"
upload_docs "$MMOC_SRC/interfaces/sbml/usr/doc/html" "/var/www/jfernandez/public_html/sbml/"