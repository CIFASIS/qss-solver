MODE ?= Debug

# The Directories, Source, Includes, Objects, Binary 
SRCDIR 		:= .
USRDIR 		:= $(SRCDIR)/usr
DOCDIR 		:= $(USRDIR)/doc
BINDIR 		:= $(USRDIR)/bin
OBJDIR 		:= $(USRDIR)/obj

# Simple rules used to run qmake before make and build the documentation.
default: build

build: | $(BUILDDIR)
	qmake
	$(MAKE)

$(BUILDDIR):
	@mkdir -p $(OBJDIR) 
	@mkdir -p $(BINDIR) 

test:
	@echo Compile and Run tests.
	@cd tests && $(MAKE) 
	@echo Done

doc:
	@mkdir -p $(DOCDIR)
	@mkdir -p $(DOCDIR)/html
	doxygen QSSSolverGUI.doxyfile

help:
	@echo "make MODE=<Debug|Release>"
	@echo "Default values:"
	@echo ""
	@echo "MODE=Debug"
