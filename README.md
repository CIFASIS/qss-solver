QSS Solver 3.2
--------------

The QSS Solver is a modeling and simulation environment for 
continuous and hybrid systems and it's optimized for large scale model simulation.
The solver is implemented as a set of modules coded in plain C language, 
simulations can be performed using one of the following integration methods:

  * QSS methods (the entire family is supported)
  * DASSL
  * DOPRI
  * CVODE
  * IDA

Models are described using a subset of the standard Modelica language called μ–Modelica 
(the language specification can be found in 
http://www.cifasis-conicet.gov.ar/jfernandez/micromodelica/micromodelicaspec.pdf), 
the compiler extracts all the structural information needed and
automatically generates the plain C code that contains an appropiate 
model definition needed by the 
QSS Solver engine together with all the configuration files (makefiles, config, etc.).

The QSS Solver has a simple GUI interface that allows the end-user
to access all the developed tools in a unified way.

The GUI has the following features:

  * It has a text editor, where models in μ-Modelica can be defined.
  * It invokes the corresponding tools to compile and run simulations.
  * It provides debug information in case of errors during the model generation.
  * It invokes GnuPlot to plot the simulation output trajectories.
  * It shows statistics about simulations (number of steps, simulation time,
etc.).

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
