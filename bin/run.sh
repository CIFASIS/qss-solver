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
#       VERSION: 4.5.3
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
    # Previous installation and different version
    OW=$(zenity --question --text "A different version of QSS Solver is found in your home folder (version: $VERHOME). Do you want to overwrite it with version: $VEROPT?\nNOTE: Your models will not be lost."; echo $?)
    
    if [ $OW == 0 ]; then 
      cp -a -f /opt/qss-solver/* "$HOME/qss-solver"
      rm "$HOME/qss-solver/bin/run.sh"
      cd "$HOME/qss-solver/bin" || exit
      ./qss-solver
    else
      cd $HOME/qss-solver/bin
      ./qss-solver
    fi
  fi
fi
