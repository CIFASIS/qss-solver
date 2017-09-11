# Target OS variables
OS ?= unix
DEBUG ?= False
CORES ?= 1

# The Directories, Source, Includes, Objects, Binary 
ROOT   	:= ..
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

mmo-interfaces: create-dirs | $(mmo-interfaces-repo)
	@echo BUILDING MMO-SBML INTERFACE
	@cd interfaces/sbml &&  $(MAKE) OS=$(OS) DEBUG=$(DEBUG) -j $(CORES)
	@echo Done

qss-engine: create-dirs | $(qss-engine-repo) $(qss-test-suite-repo) 
	@echo BUILDING QSS LIBRARY
	@cd engine && $(MAKE) OS=$(OS) -j $(CORES)
	@echo Done
	@echo BUILDING QSS DEBUG LIBRARY
	@cd engine && $(MAKE) DEBUG=True OS=$(OS) -j $(CORES)
	@echo Done

qss-user-libs: create-dirs qss-engine | $(qss-user-libs-repo)
	@echo BUILDING USER LIBS
	@cd usr && $(MAKE) OS=$(OS) DEBUG=$(DEBUG) -j $(CORES) ENGINE=../engine     
	@echo Done

mmo-compiler: create-dirs | $(mmo-compiler-repo) 
	@echo BUILDING MICROMODELICA C COMPILER
	@cd mmoc && $(MAKE) OS=$(OS) DEBUG=$(DEBUG) -j $(CORES) 
	@echo Done

qss-solver-gui: create-dirs | $(qss-solver-gui-repo) $(mmo-models-repo) $(mmo-packages-repo)
	@echo BUILDING MMO EDITOR
	@cd gui && $(MAKE) -f gui.makefile      
	@echo Done

qss-solver-doc: qss-engine mmo-compiler qss-solver-gui mmo-interfaces
	@echo BUILDING PROJECT DOCUMENTATION
	@cd engine && $(MAKE) doc 
	@cd mmoc && $(MAKE) doc 
	@cd gui && $(MAKE) -f gui.makefile doc 
	@cd interfaces/sbml && $(MAKE) doc 
	
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
	@rm -rf usr
	@rm -rf test-suite
	@rm -rf $(ROOT)/models
	@rm -rf $(ROOT)/packages

update-repos:
	@cd engine && git pull 
	@cd mmoc && git pull
	@cd gui && git pull
	@cd usr && git pull
	@cd test-suite && git pull
	@cd interfaces/sbml &&  git pull
	@cd $(ROOT)/models &&  git pull
	@cd $(ROOT)/packages &&  git pull

install:	
	@echo "Installing engine libraries."
	ln -s $(ROOT)/src/engine/usr/lib/* $(ROOT)/lib/
	@echo "Installing MicroModelica Compiler."
	ln -s $(ROOT)/src/mmoc/usr/bin/mmoc $(ROOT)/bin/
	@echo "Installing SBML translator."
	ln -s $(ROOT)/src/interfaces/sbml/usr/bin/translate-sbml $(ROOT)/bin/
	@echo "Installing solver GUI."
	@install $(ROOT)/src/gui/usr/bin/qss-solver $(ROOT)/bin/
	@echo "Installing user libraries."
	ln -s $(ROOT)/src/usr/lib/* $(ROOT)/lib/
ifeq ($(OS),unix)
	@install $(ROOT)/deploy/linux/scripts/* $(ROOT)/bin/
	@install $(ROOT)/deploy/linux/qss-solver.ini $(ROOT)/bin/
	@install $(ROOT)/src/engine/3rd-party/partitioners/hmetis/khmetis $(ROOT)/bin/
ifeq ($(AklRCH),i686)
	@install $(ROOT)/src/engine/3rd-party/partitioners/patoh/Linux-i386/libpatoh.a $(ROOT)/lib/
	@install $(ROOT)/src/engine/3rd-party/partitioners/metis/Linux-i386/libmetis.a $(ROOT)/lib/
	@install $(ROOT)/src/engine/3rd-party/partitioners/scotch/Linux-i386/libscotch.a $(ROOT)/lib/
	@install $(ROOT)/src/engine/3rd-party/partitioners/scotch/Linux-i386/libscotcherr.a $(ROOT)/lib/
else
	@install $(ROOT)/src/engine/3rd-party/partitioners/patoh/Linux-x86_64/libpatoh.a $(ROOT)/lib/
	@install $(ROOT)/src/engine/3rd-party/partitioners/metis/Linux-x86_64/libmetis.a $(ROOT)/lib/
	@install $(ROOT)/src/engine/3rd-party/partitioners/scotch/Linux-x86_64/libscotch.a $(ROOT)/lib/
	@install $(ROOT)/src/engine/3rd-party/partitioners/scotch/Linux-x86_64/libscotcherr.a $(ROOT)/lib/
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
