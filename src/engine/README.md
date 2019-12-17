QSS Engine 3.2
--------------

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
  * Model Jacobian (for IDA and CVODE).
  * Model zero-crossing function.
  * Model event handlers.
  * A function in charge of the simulator data structure initialization.
 
The signature of these functions and the data structures that are used depends 
on the integration method used and they can ve viewed in:
http://www.cifasis-conicet.gov.ar/jfernandez/engine/

Finally, the file containing the model has to be compiled and linked with the 
engine library.

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
