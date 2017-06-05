# The Directories, Source, Includes, Objects, Binary 
SRCDIR 		:= .
USRDIR 		:= $(SRCDIR)/usr

# Simple rules used to run qmake before make and build the documentation.
default: build

build:
	qmake
	$(MAKE)

doc:
	@mkdir -p $(USRDIR)/doc
	@mkdir -p $(USRDIR)/doc/html
	@mkdir -p $(USRDIR)/doc/latex
	doxygen QSSSolverGUI.doxyfile

