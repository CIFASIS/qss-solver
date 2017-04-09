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
::      VERSION: 3.0
::===================================================================================

cd ..
echo Set environment variables...
set MMOC_PATH=%CD% 
set MMOC_PATH=%MMOC_PATH:~0,-1%
echo %MMOC_PATH%
echo %CD%
set MMOC_ENGINE=%MMOC_PATH%\src\engine\
set MMOC_LIBS=%MMOC_PATH%\src\libs
set MMOC_BUILD=%MMOC_PATH%\build
set MMOC_MODELS=%MMOC_PATH%\models
set MMOC_OUTPUT=%MMOC_PATH%\output
set MMOC_BIN=%MMOC_PATH%\bin
set MMOC_PACKAGES=%MMOC_PATH%\testsuite
set MMOC_LIBRARIES=%MMOC_PATH%\usr\libs
set MMOC_INCLUDE=%MMOC_PATH%\usr\include
set MMOC_TESTS=%MMOC_PATH%\tests
set MMOC_SRC=%MMOC_PATH%\src
cd bin
echo Done.
