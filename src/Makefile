#Target OS variables
OS ?= unix
DEBUG ?= False
CORES ?= 1

#The Directories, Source, Includes, Objects, Binary 
ROOT   := ..
ifeq ($(OS),unix)
ARCH = $(shell $(ROOT)/deploy/linux/scripts/print-arch.sh)
endif

all: mmo-compiler qss-engine qss-gui qss-tools qss-user

qss-tools: create-dirs
	@echo BUILDING SBML TRANSLATOR
	@cd interfaces/sbml &&  $(MAKE) OS=$(OS) DEBUG=$(DEBUG) -j $(CORES)
	@echo Done

qss-engine: create-dirs
	@echo BUILDING QSS LIBRARY
	@cd engine && $(MAKE) OS=$(OS) -j $(CORES)
	@echo Done
	@echo BUILDING QSS  DEBUG LIBRARY
	@cd engine && $(MAKE) DEBUG=True OS=$(OS) -j $(CORES)
	@echo Done

qss-user: create-dirs
	@echo BUILDING USER LIBS
	@cd usr && $(MAKE) OS=$(OS) DEBUG=$(DEBUG) -j $(CORES)      
	@echo Done

qss-compiler: create-dirs
	@echo BUILDING MICROMODELICA C COMPILER
	@cd mmo && $(MAKE) OS=$(OS) DEBUG=$(DEBUG) -j $(CORES) 
	@echo Done

qss-gui: create-dirs
	@echo BUILDING MMO EDITOR
ifeq ($(OS),unix)
	@cd gui && qmake-qt4
else
	@cd gui && qmake
endif
	@cd gui && $(MAKE)      
	@echo Done

qss-doc:
	@echo BUILDING PROJECT DOCUMENTATION
	@mkdir -p $(ROOT)/doc/engine
	@mkdir -p $(ROOT)/doc/engine/html
	@mkdir -p $(ROOT)/doc/engine/latex
	@cd engine && doxygen QSSSolver.doxyfile 
	@mkdir -p $(ROOT)/doc/mmoc
	@mkdir -p $(ROOT)/doc/mmoc/html
	@mkdir -p $(ROOT)/doc/mmoc/latex
	@cd mmo && doxygen MMOCompiler.doxyfile 
	@mkdir -p $(ROOT)/doc/gui
	@mkdir -p $(ROOT)/doc/gui/html
	@mkdir -p $(ROOT)/doc/gui/latex
	@cd gui && doxygen QSSSolverGUI.doxyfile 
	@mkdir -p $(ROOT)/doc/sbml
	@mkdir -p $(ROOT)/doc/sbml/html
	@mkdir -p $(ROOT)/doc/sbml/latex
	@cd tools/sbml && doxygen sbml.doxyfile
	
create-dirs:
	@mkdir -p $(ROOT)/bin
	@mkdir -p $(ROOT)/build
	@mkdir -p $(ROOT)/output
	@mkdir -p $(ROOT)/libs
	@mkdir -p $(ROOT)/usr/libs
	
install:	
ifeq ($(OS),unix)
	@install $(ROOT)/deploy/linux/scripts/* $(ROOT)/bin/
	@install $(ROOT)/deploy/linux/qss-solver.ini $(ROOT)/bin/
	@install $(ROOT)/src/tools/partitioners/hmetis/khmetis $(ROOT)/bin/
ifeq ($(ARCH),i686)
	@install $(ROOT)/tools/external/partitioners/patoh/Linux-i386/libpatoh.a $(ROOT)/src/libs/
	@install $(ROOT)/tools/external/partitioners/metis/Linux-i386/libmetis.a $(ROOT)/src/libs/
	@install $(ROOT)/tools/external/partitioners/scotch/Linux-i386/libscotch.a $(ROOT)/src/libs/
	@install $(ROOT)/tools/external/partitioners/scotch/Linux-i386/libscotcherr.a $(ROOT)/src/libs/
else
	@install $(ROOT)/tools/external/partitioners/patoh/Linux-x86_64/libpatoh.a $(ROOT)/src/libs/
	@install $(ROOT)/tools/external/partitioners/metis/Linux-x86_64/libmetis.a $(ROOT)/src/libs/
	@install $(ROOT)/tools/external/partitioners/scotch/Linux-x86_64/libscotch.a $(ROOT)/src/libs/
	@install $(ROOT)/tools/external/partitioners/scotch/Linux-x86_64/libscotcherr.a $(ROOT)/src/libs/
endif
else
ifeq ($(OS),win)
	@cp $(ROOT)/deploy/windows/scripts/* $(ROOT)/bin/
	@cp $(ROOT)/deploy/windows/qss-solver.ini $(ROOT)/bin/
else
	@cp $(ROOT)/deploy/osx/scripts/* $(ROOT)/bin/
	@cp $(ROOT)/deploy/osx/qss-solver.ini $(ROOT)/bin/
endif
endif
	
.PHONY: clean all 

clean: 
	@cd mmo && $(MAKE) OS=$(OS) clean 
	@cd engine && $(MAKE) clean 
	@cd engine && $(MAKE) DEBUG=True clean 
	@cd usr && $(MAKE) clean
	@cd gui && $(MAKE) clean    
	@cd interfaces/sbml && $(MAKE) clean     

help:
	@echo "make DEBUG=<True|False> OS=<unix|win|osx> CORES=<n>"
	@echo "Default values:"
	@echo ""
	@echo "DEBUG=False"
	@echo "OS=unix"
	@echo "CORES=1"
