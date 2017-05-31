# Target OS variables
OS ?= unix
DEBUG ?= False
CORES ?= 1

# The Directories, Source, Includes, Objects, Binary 
ROOT   := ..
ifeq ($(OS),unix)
ARCH = $(shell $(ROOT)/deploy/linux/scripts/print-arch.sh)
endif

# Repositories location.

mmo-compiler-repo 			:= ./mmoc 
mmo-interfaces-repo 		:= ./interfaces/sbml
mmo-models-repo 			:= $(ROOT)/models
mmo-packages-repo 			:= $(ROOT)/packages
qss-engine-repo 			:= ./engine 
qss-test-suite-repo			:= ./testsuite 
qss-solver-gui-repo			:= ./gui 
qss-user-libs-repo 			:= ./usr
qss-solver-3rd-party-repo 	:= ./3rd-party

all: mmo-compiler mmo-interfaces qss-engine qss-solver-gui qss-user-libs

# Repositories rules.

$(mmo-compiler-repo):
	$(shell git clone https://github.com/CIFASIS/qss-solver-mmoc.git mmoc)

$(mmo-interfaces-repo):
	$(shell git clone https://github.com/CIFASIS/sbml-mmo.git interfaces/sbml)

$(mmo-models-repo):
	$(shell git clone https://github.com/CIFASIS/qss-solver-models.git $(ROOT)/models)

$(mmo-packages-repo):
	$(shell git clone https://github.com/CIFASIS/qss-solver-packages.git $(ROOT)/packages)

$(qss-engine-repo):
	$(shell git clone https://github.com/CIFASIS/qss-solver-engine.git engine)

$(qss-test-suite-repo):
	$(shell git clone https://github.com/CIFASIS/qss-solver-test-suite.git test-suite)

$(qss-solver-gui-repo):
	$(shell git clone https://github.com/CIFASIS/qss-solver-gui.git gui)

$(qss-user-libs-repo):
	$(shell git clone https://github.com/CIFASIS/qss-solver-user-libs.git usr)

$(qss-solver-3rd-party-repo):
	$(shell git clone https://github.com/CIFASIS/qss-solver-external-tools.git 3rd-party)


mmo-interfaces: create-dirs | $(mmo-interfaces-repo)
	@echo BUILDING MMO-SBML INTERFACE
	@cd interfaces/sbml &&  $(MAKE) OS=$(OS) DEBUG=$(DEBUG) -j $(CORES)
	@echo Done

qss-engine: create-dirs | $(qss-engine-repo) $(qss-test-suite-repo) $(qss-solver-3rd-party-repo)
	@echo BUILDING QSS LIBRARY
	@cd engine && $(MAKE) OS=$(OS) -j $(CORES)
	@echo Done
	@echo BUILDING QSS DEBUG LIBRARY
	@cd engine && $(MAKE) DEBUG=True OS=$(OS) -j $(CORES)
	@echo Done

qss-user-libs: create-dirs | $(qss-user-libs-repo)
	@echo BUILDING USER LIBS
	@cd usr && $(MAKE) OS=$(OS) DEBUG=$(DEBUG) -j $(CORES)      
	@echo Done

mmo-compiler: create-dirs | $(mmo-compiler-repo) $(qss-solver-3rd-party-repo)
	@echo BUILDING MICROMODELICA C COMPILER
	@cd mmoc && $(MAKE) OS=$(OS) DEBUG=$(DEBUG) -j $(CORES) 
	@echo Done

qss-solver-gui: create-dirs | $(qss-solver-gui-repo) $(mmo-models-repo) $(mmo-packages-repo)
	@echo BUILDING MMO EDITOR
	@cd gui && qmake
	@cd gui && $(MAKE)      
	@echo Done

qss-solver-doc: qss-engine mmo-compiler qss-solver-gui mmo-interfaces
	@echo BUILDING PROJECT DOCUMENTATION
	@mkdir -p $(ROOT)/doc/engine
	@mkdir -p $(ROOT)/doc/engine/html
	@mkdir -p $(ROOT)/doc/engine/latex
	@cd engine && doxygen QSSSolver.doxyfile 
	@mkdir -p $(ROOT)/doc/mmoc
	@mkdir -p $(ROOT)/doc/mmoc/html
	@mkdir -p $(ROOT)/doc/mmoc/latex
	@cd mmoc && doxygen MMOCompiler.doxyfile 
	@mkdir -p $(ROOT)/doc/gui
	@mkdir -p $(ROOT)/doc/gui/html
	@mkdir -p $(ROOT)/doc/gui/latex
	@cd gui && doxygen QSSSolverGUI.doxyfile 
	@mkdir -p $(ROOT)/doc/sbml
	@mkdir -p $(ROOT)/doc/sbml/html
	@mkdir -p $(ROOT)/doc/sbml/latex
	@cd interfaces/sbml && doxygen sbml.doxyfile
	
create-dirs:
	@mkdir -p $(ROOT)/bin
	@mkdir -p $(ROOT)/build
	@mkdir -p $(ROOT)/output
	@mkdir -p $(ROOT)/lib

clean-repos:
	@rm -rf mmoc
	@rm -rf engine
	@rm -rf gui
	@rm -rf interfaces
	@rm -rf 3rd-party
	@rm -rf usr
	@rm -rf test-suite
	@rm -rf $(ROOT)/models
	@rm -rf $(ROOT)/packages

update-repos:
	@cd engine && $(shell git pull)
	@cd mmoc && $(shell git pull)
	@cd gui && $(shell git pull)
	@cd usr && $(shell git pull)
	@cd interfaces/sbml &&  $(shell git pull)
	@cd 3rd-party &&  $(shell git pull)
	@cd $(ROOT)/models &&  $(shell git pull)
	@cd $(ROOT)/packages &&  $(shell git pull)

install:	
ifeq ($(OS),unix)
	@install $(ROOT)/deploy/linux/scripts/* $(ROOT)/bin/
	@install $(ROOT)/deploy/linux/qss-solver.ini $(ROOT)/bin/
	@install $(ROOT)/src/3rd-party/partitioners/hmetis/khmetis $(ROOT)/bin/
ifeq ($(AklRCH),i686)
	@install $(ROOT)/src/3rd-party/partitioners/patoh/Linux-i386/libpatoh.a $(ROOT)/lib/
	@install $(ROOT)/src/3rd-party/partitioners/metis/Linux-i386/libmetis.a $(ROOT)/lib/
	@install $(ROOT)/src/3rd-party/partitioners/scotch/Linux-i386/libscotch.a $(ROOT)/lib/
	@install $(ROOT)/src/3rd-party/partitioners/scotch/Linux-i386/libscotcherr.a $(ROOT)/lib/
else
	@install $(ROOT)/src/3rd-party/partitioners/patoh/Linux-x86_64/libpatoh.a $(ROOT)/lib/
	@install $(ROOT)/src/3rd-party/partitioners/metis/Linux-x86_64/libmetis.a $(ROOT)/lib/
	@install $(ROOT)/src/3rd-party/partitioners/scotch/Linux-x86_64/libscotch.a $(ROOT)/lib/
	@install $(ROOT)/src/3rd-party/partitioners/scotch/Linux-x86_64/libscotcherr.a $(ROOT)/lib/
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
	@cd mmoc && $(MAKE) OS=$(OS) clean 
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
