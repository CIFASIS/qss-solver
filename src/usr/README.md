# qss-solver-user-libs
QSS Solver C user libraries.

C libraries that contains helper functions used in MicroModelica packages. The Makefile included in this repo compiles the source 
files and generates the static library in the lib folder of the qss-solver. 
To compile a new user library, follow these steps:

1- Put the header file in the include directory.

2- Put the source file in the src directory.

3- Add the source file and the target library name in the Makefile.

4- Finally, run make to generate the corresponding static lib.

The generated lib is stored in the default path that the MMOC looks for, so the lib can be directly accesed from MicroModelica
functions. For examples of how the C user libraries can be used, see the packages repo 
(https://github.com/CIFASIS/qss-solver-packages). 

