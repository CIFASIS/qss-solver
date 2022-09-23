#!/bin/bash
#===================================================================================
#
# 				 FILE: run_win.sh
#
# 				USAGE: run_win.sh  
#
# 	DESCRIPTION: Runs the QSS Solver GUI for Windows installations, if no previous 
#                installation is found then the qss-solver folder is created an all 
#                the corresponding files are copied there. If a previous 
#                installation with the same version is found, then run the 
#                qss-solver app in the default user folder, otherwise ask the 
#                user for an application update.
#
#    PARAMETERS: ---
#       OPTIONS: --- 
#  REQUIREMENTS: --- 
#         NOTES: ---
#        AUTHOR: Joaquin Fernandez, joaquin.f.fernandez@gmail.com
#       PROJECT: QSS Solver
#       VERSION: 4.0.0
#===================================================================================


if [ ! -d $HOME/qss-solver ]
then
  echo "Linking QSS Solver user folder"

  cmd.exe /c "echo %USERNAME%" > user.txt
  WINDOWS_USER=`tr -d '\r' < user.txt`
  rm user.txt

  echo "Found Windows user: " $WINDOWS_USER

  LINUX_QSS_SOLVER=~/qss-solver
  LINUX_QSS_SOLVER_TMP=${LINUX_QSS_SOLVER}-tmp
  WINDOWS_USER_FOLDER=/mnt/c/Users/${WINDOWS_USER}
  WINDOWS_QSS_SOLVER=${WINDOWS_USER_FOLDER}/QSSSolver

  echo Temp folders:
  echo $LINUX_QSS_SOLVER
  echo $LINUX_QSS_SOLVER_TMP
  echo $WINDOWS_QSS_SOLVER

  if [[ -L "$LINUX_QSS_SOLVER" && -d "$LINUX_QSS_SOLVER" ]]
  then
  echo "Removing previous symbolic link to Windows user folder"
  rm $LINUX_QSS_SOLVER 
  fi

  if [ -d "$LINUX_QSS_SOLVER" ]
  then
  mv $LINUX_QSS_SOLVER $LINUX_QSS_SOLVER_TMP
  fi

  mkdir -p $WINDOWS_QSS_SOLVER

  ln -s $WINDOWS_QSS_SOLVER/ $LINUX_QSS_SOLVER
  ln -s $WINDOWS_USER_FOLDER/ ~/$WINDOWS_USER 
  ln -s /mnt/c/ ~/C 

  if [ -d "$LINUX_QSS_SOLVER_TMP" ]
  then
  cp -r $LINUX_QSS_SOLVER_TMP/* $LINUX_QSS_SOLVER/
  rm -rf $LINUX_QSS_SOLVER_TMP
  fi
  echo "Installing QSS Solver..."
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
    OW=`zenity --question --text "There is a different version of QSS Solver on your home folder (ver. $VERHOME). Do you wish to overwrite it with ver. $VEROPT?\nNOTE: you will not lose your models."; echo $?`
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
