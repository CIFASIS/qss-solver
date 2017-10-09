        QSS Solver 3.1
--------------

The QSS Solver is a modeling and simulation environment for 
continuous and hybrid systems.

Models are described using a subset of the Modelica language 
called MicroMoledica (http://www.fceia.unr.edu.ar/
control/modelica/micromodelicaspec.pdf).

Simulations can be performed using one of the following integration methods:

	* QSS methods (the entire family is supported)
	* DASSL
	* DOPRI

New Features
------------

	* Parallel simulation is supported for Linux platform.

	* Modelica User Defined Functions (UDF) are now supported (including external C functions).
	
	* The Modelica Package class containing UDF's is now supported.
	
	* 'for' statements can be used in "event handler" definitions, i.e. inside 'when' and
	  'elsewhen' statements.
	
	* Modelica array element-wise operations are supported (.+,.-,.*,./).
	
	* Modelica reduction operations for arrays are supported:
			- sum
			- product
			- min
			- max
			- *
	
	* Classic numerical integration methods DASSL and DOPRI can be used to run simulations.

Installation
------------

Please see the file called INSTALL. 
			   
Licensing
---------

Please see the file called LICENSE.


ChangeLog
----------

Please see the file called CHANGELOG.

Bug Reporting
-------------

Report bugs to: fernandez@cifasis-conicet.gov.ar

System-specific Notes
---------------------

	The binary file installation for MAC users requires the following additional steps:

		1- Install XCode Command Line Tools according to your OS X version.
		2- Download and install XQuartz (http://xquartz.macosforge.org/landing/).
		3- Unpack the QSS Solver image into your peferred location (we will assume that the application is unpacked into 
		   the Applications  folder => QSS-SOLVER-PATH=/Applications/qss-solver.app).  
		4- Open a terminal and execute the installation script: QSS-SOLVER-PATH/Contents/MacOS/scripts/install.sh
		5- Finally, go to System Preferences -> Security & Privacy and in the "General" tab, 
		   check "Allow applications downloaded from Anywhere".
