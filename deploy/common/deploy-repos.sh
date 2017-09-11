#!/bin/bash
#===================================================================================
#
# 				 FILE: update-repos.sh
#
# 				USAGE: update-repos.sh <folder>
#
# 	DESCRIPTION: Pull all the repositopries needed by the qss-solver and generate
#                the appropiate folder structure.
#
#    PARAMETERS: <folder> root folder where the qss-solver repositories are pulled.  
#  REQUIREMENTS: ---
#         NOTES: ---
#        AUTHOR: Joaquin Fernandez, joaquin.f.fernandez@gmail.com
#       PROJECT: QSS Solver
#       VERSION: 3.1
#===================================================================================

# Root folder parameter.
root=$1

deploy_path=`pwd`
cd ../../
base=`pwd`
root_path=${base}/${root}
engine=src/engine
mmoc=src/mmoc
test_suite=src/test-suite
usr=src/usr
gui=src/gui
interfaces=src/interfaces/sbml
models=models
packages=packages

function checkout-index {
    mkdir -p ${root}/$1
    cd ${base}/$1
    git checkout-index --prefix=${root_path}/$1/ -a 
}

git checkout-index --prefix=${root}/ -a 
checkout-index ${engine} 
checkout-index ${mmoc}
checkout-index ${test_suite}
checkout-index ${usr}
checkout-index ${gui}
checkout-index ${interfaces}
checkout-index ${models}
checkout-index ${packages}
cd ${deploy_path}
