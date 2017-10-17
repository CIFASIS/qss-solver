#!/bin/bash
#===================================================================================
#
# 				 FILE: repos.sh
#
# 				USAGE: repos.sh <command>
#
# 	DESCRIPTION: Execute git command <command> on all the qss-solver repositories. 
#
#    PARAMETERS: <command> git command to execute.  
#  REQUIREMENTS: MMOC_PATH and MMOC_SRC environment variables must be defined.
#         NOTES: ---
#        AUTHOR: Joaquin Fernandez, joaquin.f.fernandez@gmail.com
#       PROJECT: QSS Solver
#       VERSION: 3.2
#===================================================================================

# Parameters.
GIT_COMMAND=$1
DEPLOY_PATH=$MMOC_PATH/$2

# QSS Solver repositories.
BASE_REPO=$MMOC_PATH
ENGINE_REPO=$MMOC_SRC/engine
MMOC_REPO=$MMOC_SRC/mmoc
TESTSUITE_REPO=$MMOC_SRC/test-suite
USR_REPO=$MMOC_SRC/usr
GUI_REPO=$MMOC_SRC/gui
INTERFACES_REPO=$MMOC_SRC/interfaces/sbml
MODELS_REPO=$MMOC_PATH/models
PACKAGES_REPO=$MMOC_PATH/packages

# Global variables.
VERSION=0

# QSS Solver repositories relative paths to base repo.
BASE_REPO_REL=.
ENGINE_REPO_REL=src/engine
MMOC_REPO_REL=src/mmoc
TESTSUITE_REPO_REL=src/test-suite
USR_REPO_REL=src/usr
GUI_REPO_REL=src/gui
INTERFACES_REPO_REL=src/interfaces/sbml
MODELS_REPO_REL=models
PACKAGES_REPO_REL=packages

function checkout-index 
{
    CHECKOUT_PATH=$DEPLOY_PATH/$1
    mkdir -p $CHECKOUT_PATH
    cd $2
    git checkout-index --prefix=$CHECKOUT_PATH/ -a 
}

function gen-changelog 
{
    cd $1
    ./gitchangelog.py > CHANGELOG

    if [ "${2}" != "QSS Solver" ]; then 
      cat CHANGELOG | awk -f $MMOC_PATH/deploy/common/repo-change.awk -v REPO="$2" -v SUB="$3" >> $MMOC_PATH/CHANGELOG
    fi
}

function commit-changelog 
{
    cd $1
    git add CHANGELOG
    git commit -m "Modified Changelog."
    git push
}

function tag
{
    cd $1
    vversion=`git rev-list --count HEAD`
    vversion=$2$vversion
    echo $vversion
    git tag -a ${vversion} -m "Version ${vversion}"
    git push origin ${vversion}
}

function version
{
    cd $1
    n=`git rev-list --count HEAD`
    VERSION=$(( VERSION + n ))
}

function color-echo ()
{
    echo -e "$(tput setaf $2)$1$(tput setaf 7)\n"
}


REPO_NAMES=( "QSS Solver" "QSS Solver Engine" "MicroModelica Compiler" "QSS Solver GUI" "SBML-MicroModelica Translator" "User Libraries" "Test Suite" "Models" "Packages" );

REPO_SUBS=( "==========" "=================" "======================" "==============" "=============================" "==============" "==========" "======" "========" );

REPOS=( ${BASE_REPO} ${ENGINE_REPO} ${MMOC_REPO} ${TESTSUITE_REPO} ${USR_REPO} ${GUI_REPO} ${INTERFACES_REPO} ${MODELS_REPO} ${PACKAGES_REPO} );

REPOS_REL=( ${BASE_REPO_REL} ${ENGINE_REPO_REL} ${MMOC_REPO_REL} ${TESTSUITE_REPO_REL} ${USR_REPO_REL} ${GUI_REPO_REL} ${INTERFACES_REPO_REL} ${MODELS_REPO_REL} ${PACKAGES_REPO_REL} );

if [ "$GIT_COMMAND" = "deploy" ]; then 
    for i in ${!REPOS[*]}; do
        color-echo "Running git command: $GIT_COMMAND Repository: ${REPOS_REL[$i]}" 2
        checkout-index ${REPOS_REL[$i]} ${REPOS[$i]}
        color-echo "\nDone" 2
    done
elif [ "$GIT_COMMAND" = "tag" ]; then 
    for i in ${!REPOS[*]}; do
        color-echo "Running git command: $GIT_COMMAND Repository: ${REPOS[$i]}" 2
        tag ${REPOS[$i]} $2 
        color-echo "\nDone" 2
    done
elif [ "$GIT_COMMAND" = "changelog" ]; then 
    for i in ${!REPOS[*]}; do
        color-echo "Running git command: $GIT_COMMAND Repository: ${REPOS[$i]}" 2
        gen-changelog ${REPOS[$i]} "${REPO_NAMES[$i]}" "${REPO_SUBS[$i]}"
        color-echo "\nDone" 2
    done
elif [ "$GIT_COMMAND" = "commit-changelog" ]; then 
    for i in ${!REPOS[*]}; do
        color-echo "Running git command: $GIT_COMMAND Repository: ${REPOS[$i]}" 2
        commit-changelog ${REPOS[$i]} 
        color-echo "\nDone" 2
    done
elif [ "$GIT_COMMAND" = "version" ]; then 
    for i in ${!REPOS[*]}; do
        version ${REPOS[$i]} 
    done
    echo $VERSION
else
    for i in ${REPOS[@]}; do
        color-echo "Running git command: $GIT_COMMAND Repository: $i" 2
        cd $i
        git $GIT_COMMAND
        color-echo "\nDone" 2
    done
fi
