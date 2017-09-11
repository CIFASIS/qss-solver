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

base=`pwd`
engine=src/engine
mmoc=src/mmoc
test_suite=src/test-suite
usr=src/usr
gui=src/gui
interfaces=src/interfaces/sbml
models=models
packages=packages

function git-pull {
    cd ${base}; cd $1
    git pull  
}

#git-pull ${root}
git-pull ${models}
git-pull ${packages}
git-pull ${engine}
git-pull ${mmoc}
git-pull ${gui}
git-pull ${usr}
git-pull ${test_suite}
git-pull ${interfaces}
