::===================================================================================
::
:: 				 FILE: exportvars.bat
::
:: 				USAGE: exportvars.bat
::
:: 	DESCRIPTION: Exports all the environment variables that the MicroModelica C  
:: 							 Compiler uses. 
::
::   PARAMETERS: ---
::      OPTIONS: --- 
:: REQUIREMENTS: --- 
::        NOTES: ---
::       AUTHOR: Joaquin Fernandez, joaquin.f.fernandez@gmail.com
::      PROJECT: QSS Solver
::      VERSION: 3.2
::===================================================================================

cd ..
echo Set environment variables...
set MMOC_PATH=%CD% 
set MMOC_PATH=%MMOC_PATH:~0,-1%
set MMOC_SRC=%MMOC_PATH%\src
set MMOC_LIBS=%MMOC_PATH%\lib
set MMOC_BUILD=%MMOC_PATH%\build
set MMOC_MODELS=%MMOC_PATH%\models
set MMOC_OUTPUT=%MMOC_PATH%\output
set MMOC_BIN=%MMOC_PATH%\bin
set MMOC_PACKAGES=%MMOC_PATH%\packages
set MMOC_LIBRARIES=%MMOC_PATH%\lib
set MMOC_TESTS=%MMOC_SRC%\test-suite
set MMOC_ENGINE=%MMOC_SRC%\engine\
set MMOC_INCLUDE=%MMOC_SRC%\usr\include
cd bin
echo Done.
