#!/bin/bash
#===================================================================================
#
# 				 FILE: install.sh
#
# 				USAGE: install.sh 
#
# 	DESCRIPTION: First install brew and then install all the dependences of 
# 							 the QSS Solver, the installed packages are: 
# 						   * libconfig
# 							 * ginac
# 							 * gnuplot
#  							 * gcc
# 							 * gsl
#    PARAMETERS: --- 
#  REQUIREMENTS: MAC command line tools and XQuartz must be installed. 
#         NOTES: ---
#        AUTHOR: Joaquin Fernandez, joaquin.f.fernandez@gmail.com
#       PROJECT: QSS Solver
#       VERSION: 3.2
#===================================================================================

ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
brew install libconfig
brew install ginac
brew install gnuplot --with-x
brew install gcc
brew install gsl
