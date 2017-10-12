#!/bin/bash
#===================================================================================
#
# 				 FILE: run.sh
#
# 				USAGE: run.sh  
#
# 	DESCRIPTION: Runs the QSS Solver GUI, if no previous installation is found
# 							 then the qss-solver folder is created an all the corresponding files
# 							 are copied there. If a previous installation with the same version
# 						   is found, then run the qss-solver app in the default user
# 							 folder, otherwise ask the user for an application update.
#
#    PARAMETERS: ---
#       OPTIONS: --- 
#  REQUIREMENTS: --- 
#         NOTES: ---
#        AUTHOR: Joaquin Fernandez, joaquin.f.fernandez@gmail.com
#       PROJECT: QSS Solver
#       VERSION: 3.2
#===================================================================================


if [ ! -d $HOME/qss-solver ]
then
  # No previous instalation and first run
	echo "Installing MicroModelica Solver..."
	cp -a /opt/qss-solver $HOME/qss-solver
	rm $HOME/qss-solver/bin/run.sh
  cd $HOME/qss-solver/bin
	echo "Register environment variables..."
  ./registervars.sh
	echo "Done."
  ./qss-solver
else
  VEROPT=`cat /opt/qss-solver/version`
  VERHOME=`cat $HOME/qss-solver/version`
  if [  "$VEROPT" == "$VERHOME" ];
  then
    # Previous instalation and same version
    cd $HOME/qss-solver/bin
    ./qss-solver
  else
    # Previous instalation and different version
    OW=`zenity --question --text "There is a different version of MicroModelica Solver on your home folder (ver. $VERHOME). Do you wish to overwrite it with ver. $VEROPT?\nNOTE: you will not lose your models."; echo $?`
    if [ $OW == 0 ];
    then 
	    cp -a -f /opt/qss-solver/* $HOME/qss-solver
    	rm $HOME/qss-solver/bin/run.sh
      cd $HOME/qss-solver/bin
      ./qss-solver
    else
      cd $HOME/qss-solver/bin
      ./qss-solver
    fi
  fi
fi
