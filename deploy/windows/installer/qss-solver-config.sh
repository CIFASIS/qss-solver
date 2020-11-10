echo "Adding X server display environment variables"
echo 'export DISPLAY=:0.0' >> ~/.bashrc
echo 'export LIBGL_ALWAYS_INDIRECT=1' >> ~/.bashrc

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
