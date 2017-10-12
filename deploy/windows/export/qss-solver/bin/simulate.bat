::===================================================================================
::
:: 				 FILE: simulate.bat
::
:: 				USAGE: simulate.bat <FILE> <FOLDER> <PARALLEL>
::
:: 	DESCRIPTION: Creates the outut directory for the simulation in the default 
:: 							 location, copy the configuration files in the output directory,
:: 							 run the simulation executable and finally remove the
:: 							 configuration files. 
::
::   PARAMETERS: <FILE> simulation executable file.
::    						 <FOLDER> output folder name.
::    						 <PARALLEL> True for parallel simulations, False otherwise.
::      OPTIONS: --- 
:: REQUIREMENTS: MMOC_OUTPUT MMOC_BUILD must point to the corresponding default 
::							 directories used by the QSS Solver GUI.
::        NOTES: ---
::       AUTHOR: Joaquin Fernandez, joaquin.f.fernandez@gmail.com
::      PROJECT: QSS Solver
::      VERSION: 3.2
::===================================================================================

set FILE=%~1

set FOLDER=%~2 

set PARALLEL=%~3

set PREV_PATH=%PATH%

set PATH=%PATH%;%MMOC_BIN%;%MMOC_BIN%\MinGW\bin

cd %MMOC_BUILD%

set MMOC_BUILD=%CD%

cd %MMOC_OUTPUT%\%FILE%

copy	%MMOC_BUILD%\%FILE%\%FILE%.ini .

%MMOC_BUILD%\%FILE%\%FILE%.exe

del %FILE%.ini  

set PATH=%PREV_PATH%
