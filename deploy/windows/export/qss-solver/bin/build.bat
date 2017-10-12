::===================================================================================
::
:: 				 FILE: build.bat
::
:: 				USAGE: build.bat <file>
::
:: 	DESCRIPTION: Move to the $MMOC_BUILD directory and run the MicroModelica 
:: 							 C Compiler generated makefile. 
::
::   PARAMETERS: <file> must be the name of a previously compiled 
::  							 MicroModelica file.  
:: REQUIREMENTS: MMOC_BUILD must point to the default build directory 
:: 							 used by the QSS Solver GUI.
::        NOTES: ---
::       AUTHOR: Joaquin Fernandez, joaquin.f.fernandez@gmail.com
::      PROJECT: QSS Solver
::      VERSION: 3.2
::===================================================================================

set NAME=%~1

cd %MMOC_BUILD%\%NAME%

%MMOC_BIN%\MinGW\bin\mingw32-make -e CC=%MMOC_BIN%\minGW\bin\gcc -f %NAME%.makefile 

