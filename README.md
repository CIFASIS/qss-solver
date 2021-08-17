## QSS Solver 4.0

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

## Related Publications

[1] Joaquin Fernandez, Ernesto Kofman.
**A stand-alone quantized state system solver for continuous system simulation**
Simulation 

[2] Federico Bergero, Xenofon Floros, Joaquin Fernandez, Ernesto Kofman, Francois Cellier.
**Simulating Modelica models with a Stand–Alone Quantized State Systems Solver**
Proceedings of the 9th International Modelica Conference

[3] Joaquin Fernandez, Ernesto Kofman, Federico Bergero.
**A parallel Quantized State System Solver for ODEs**
Journal of Parallel and Distributed Computing

[4] Federico Bergero, Francesco Casella, Ernesto Kofman, Joaquin Fernandez.
**On the efficiency of quantization-based integration methods for building simulation**
Building Simulation

[5] Federico Bergero, Joaquin Fernandez, Ernesto Kofman, Margarita Portapila.
**Time discretization versus state quantization in the simulation of a one-dimensional advection–diffusion–reaction equation**
Simulation

[5] Gustavo Migoni, Ernesto Kofman, Federico Bergero, Joaquin Fernandez.
**Quantization-based simulation of switched mode power supplies**
Simulation

[6] Franco Di Pietro, Joaquin Fernandez, Gustavo Migoni, Ernesto Kofman.
**Mixed-mode state–time discretization in ODE numerical integration**
Journal of Computational and Applied Mathematics 

[7] Ernesto Kofman, Joaquin Fernandez, Denise Marzorati.
**Compact sparse symbolic Jacobian computation in large systems of ODEs**
Applied Mathematics and Computation

## Components

## MicroModelica Compiler

The MicroModelica compiler allows the user to describe models 
using a subset of the standard Modelica language called μ–Modelica 
(the language specification can be found in 
http://www.cifasis-conicet.gov.ar/jfernandez/micromodelica/micromodelicaspec.pdf), 
the compiler extracts all the structural information needed and
automatically generates the plain C code that contains an appropiate 
model definition needed by the 
QSS Solver engine (https://github.com/CIFASIS/qss-solver-engine)
together with all the configuration files (makefiles, config, etc.).
The compiler documentation can be found at: 
http://www.cifasis-conicet.gov.ar/jfernandez/mmoc/

## QSS Engine

The QSS Engine is a library that allows the simulation of continuous and hybrid 
systems and it's optimized for large scale model simulation. 
The solver is implemented as a set of modules coded in plain C language, where
the simulated models can contain time and state discontinuities.
Simulations can be performed using one of the following integration methods:

  * QSS methods (the entire family is supported)
  * DASSL
  * DOPRI
  * CVODE
  * IDA

In order to obtain an executable model, a plain C file that contains the 
following functions must be provided:

  * Model definition.
  * Model settings.
  * Model dependencies (for QSS methods).
  * Model Jacobian.
  * Model zero-crossing function.
  * Model event handlers.
  * A function in charge of the simulator data structure initialization.
 
The signature of these functions and the data structures that are used depends 
on the integration method used and they can ve viewed in:
http://www.cifasis-conicet.gov.ar/jfernandez/engine/

Finally, the file containing the model has to be compiled and linked with the 
engine library.

## QSS Solver GUI

The QSS Solver GUI is a simple interface that allows the end-user
of the QSS Solver to access all the developed tools in a
unified way.

The GUI has the following features:

  * It has a text editor, where models in μ-Modelica can be defined.
  * It invokes the corresponding tools to compile and run simulations.
  * It provides debug information in case of errors during the model generation.
  * It invokes GnuPlot to plot the simulation output trajectories.
  * It shows statistics about simulations (number of steps, simulation time,
etc.).

The GUI documentation can be found in:
http://www.cifasis-conicet.gov.ar/jfernandez/gui/


## Distributed MicroModelica User Packages

MicroModelica packages with math helper functions and file manipulation functions that uses the C user libraries described bellow. 

These packages are included in qss-solver, so that in order to use them no aditional annotation 
has to be used (such as package location, include paths, etc), just import the package into the corresponding model.

Available packages:

* math.mo: That contains math utility functions.
* file.mo: That contains file manipulation functions.

Both files are located in the `packages` folder in the root install location.

## C User Libraries

C libraries that contains helper functions used in MicroModelica packages. The Makefile included compiles the source 
files and generates the static library in the lib folder of the qss-solver. 
To compile a new user library, follow these steps:

1- Put the header file in the include directory.

2- Put the source file in the src directory.

3- Add the source file and the target library name in the Makefile.

4- Finally, run make to generate the corresponding static lib.

The generated lib is stored in the default path that the MMOC looks for, so the lib can be directly accesed from MicroModelica
functions.  

## Installation

These are generic installation instructions.

## Dependences

In order to be able to install and compile the QSS Solver, 
the following  dependencies must be installed: 

    * bison++
    * boost1.71
    * cmake 
    * g++
    * gfortran 
    * gnuplot
    * libatlas-base-dev
    * libcln-dev
    * libconfig-dev
    * libginac-dev
    * libgsl-dev
    * libsbml5-dev
    * libscotch-dev
    * libsuitesparse-dev
    * pkgconf
    * python
    * qt5-default
     

## Basic Installation

The simplest way to compile this package is:

  1. `cd` to the directory containing the package's source code and type
     `make` to compile the binaries and libraries.
  
  2. Type `make install` to install all the binary files and the libraries.
     The binaries are located in the bin folder and the libraries are located
     in the lib folder. 
  
  3. You can remove the program binaries and object files from the
     source code directory by typing `make clean`.  

## Makefile options

The makefile script accepts the following options:

	* MODE = <Debug|Release> 	When set to Debug (default), adds the compiler's debug flags.
	
## Makefile targets

The makefile script accepts the following targets:

	* qss-engine: 		Builds the QSS solver libraries.

	* mmo-compiler: 	Builds the MicroModelica compiler. 	

	* qss-solver-gui:	Builds the graphic interface.

	* qss-user-libs:	Builds the user packages included in this distribution.

	* mmo-interfaces:	Builds the SBML-MicroModelica translator. 

  * qss-solver-doc: Builds the documentation for the QSS solver libraries,
                    the MicroModelica compiler and the QSS solver GUI.
			   

## Changelog

## [3.2] 
### Added
- Implemented Discrete Event parallel simulation algorithm.
- Implemented new mixed mode QSS-BDF algorithm.

## Licensing

Please see the file called LICENSE.

## Bug Reporting

Report bugs to: fernandez@cifasis-conicet.gov.ar
