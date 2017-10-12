::===================================================================================
::
:: 				 FILE: mmoc.bat
::
:: 				USAGE: mmoc.bat [OPTIONS] <FILE> 
::
:: 	DESCRIPTION: Runs the MicroModelica compiler on file <FILE> 
::                with options defined in <OPTIONS>  
::
::   PARAMETERS: <FILE> MicroModelica file.
::      OPTIONS: [OPTIONS] MicroModelica compiler options (see ./mmoc --help). 
:: REQUIREMENTS: The environment variable MMOC_BIN must be defined.  
::        NOTES: --- 
::       AUTHOR: Joaquin Fernandez, joaquin.f.fernandez@gmail.com
::      PROJECT: QSS Solver
::      VERSION: 3.2
::===================================================================================

set OPTIONS=%~1

set FILE=%2

%MMOC_BIN%\mmoc.exe %OPTIONS% %FILE%
