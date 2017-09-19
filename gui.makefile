# The Directories, Source, Includes, Objects, Binary 
SRCDIR 		:= .
USRDIR 		:= $(SRCDIR)/usr
DOCDIR 		:= $(SRCDIR)/doc
BINDIR 		:= $(SRCDIR)/bin
OBJDIR 		:= $(SRCDIR)/obj

# Simple rules used to run qmake before make and build the documentation.
default: build

build: | $(BUILDDIR)
	qmake
	$(MAKE)

$(BUILDDIR):
	@mkdir -p $(OBJDIR) 
	@mkdir -p $(BINDIR) 

doc:
	@mkdir -p $(DOCDIR)
	@mkdir -p $(DOCDIR)/html
	@mkdir -p $(DOCDIR)/latex
	doxygen QSSSolverGUI.doxyfile

