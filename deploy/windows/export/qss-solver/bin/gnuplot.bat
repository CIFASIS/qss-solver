::===================================================================================
::
:: 				 FILE: gnuplot.bat
::
:: 				USAGE: gnuplot.bat [OPTIONS] <FILE> 
::
:: 	DESCRIPTION: Runs the gnuplot script <FILE> with options defined in <OPTIONS>  
::
::   PARAMETERS: <FILE> gnuplot script.
::      OPTIONS: [OPTIONS] gnuplot options. 
:: REQUIREMENTS: --- 
::        NOTES: -persist is the default option used by the QSS Solver GUI.
::       AUTHOR: Joaquin Fernandez, joaquin.f.fernandez@gmail.com
::      PROJECT: QSS Solver
::      VERSION: 3.0
::===================================================================================

set OPTIONS=%~1
set NAME=%2

.\gnuplot\bin\gnuplot.exe %OPTIONS% %NAME%
