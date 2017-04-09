::===================================================================================
::
:: 				 FILE: sbml.bat  
::
:: 				USAGE: sbml.bat [OPTIONS] <FILE>
::
:: 	DESCRIPTION: Runs the SBML model translator with the provided arguments and
:: 							 exports the LD_LIBRARY_PATH variable so that the tool can load
:: 							 the corresponding dynamic library.
::
::   PARAMETERS: <FILE> xml file containing a SBML model.
::      OPTIONS: [OPTIONS] translate-sbml options (see ./translate-sbml --help).
:: REQUIREMENTS: MMOC_LIBS must point to the default library location for the
:: 							 QSS Solver.
::        NOTES: --- 
::       AUTHOR: Joaquin Fernandez, joaquin.f.fernandez@gmail.com
::      PROJECT: QSS Solver
::      VERSION: 3.0
::===================================================================================

set OPTIONS=%~1

set FILE=%2

translate-sbml.exe %OPTIONS% %FILE%
