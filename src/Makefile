# Target OS variables
ifneq ($(OS), Windows_NT)
OS = $(shell uname)
endif
DEBUG ?= False
CORES ?= 1
ARCH = $(shell uname -m)

# The Directories, Source, Includes, Objects, Binary 
ROOT   	        := ..
3RD_PARTY_DIR   := ./3rd-party
GTEST_DIR      := googletest-release-1.10.0
GTEST_CODE      := $(GTEST_DIR).tar.gz

all: mmo-compiler mmo-interfaces qss-engine qss-solver-gui qss-user-libs lib-gtest

mmo-interfaces: create-dirs 
	@echo BUILDING MMO-SBML INTERFACE
	@cd interfaces/sbml &&  $(MAKE) DEBUG=$(DEBUG) -j $(CORES)
	@echo Done

qss-engine: create-dirs  
	@echo BUILDING QSS LIBRARY
	@cd engine && $(MAKE) -j $(CORES)
	@echo Done
	@echo BUILDING QSS DEBUG LIBRARY
	@cd engine && $(MAKE) DEBUG=True -j $(CORES)
	@echo Done

qss-user-libs: create-dirs qss-engine
	@echo BUILDING USER LIBS
	@cd usr && $(MAKE) DEBUG=$(DEBUG) -j $(CORES) ENGINE=../engine     
	@echo Done

mmo-compiler: create-dirs  
	@echo BUILDING MICROMODELICA C COMPILER
	@cd mmoc && $(MAKE) DEBUG=$(DEBUG) -j $(CORES) 
	@echo Done

qss-solver-gui: create-dirs 
	@echo BUILDING MMO EDITOR
	@cd gui && $(MAKE) -f gui.makefile      
	@echo Done

qss-solver-doc: qss-engine mmo-compiler qss-solver-gui mmo-interfaces
	@echo BUILDING PROJECT DOCUMENTATION
	@cd engine && $(MAKE) doc 
	@cd mmoc && $(MAKE) doc 
	@cd gui && $(MAKE) -f gui.makefile doc 
	@cd interfaces/sbml && $(MAKE) doc 

lib-gtest: | create-dirs 
	cd $(3RD_PARTY_DIR)/gtest; tar xvzf $(GTEST_CODE)
	mkdir -p $(3RD_PARTY_DIR)/gtest/build
	cd $(3RD_PARTY_DIR)/gtest/build; cmake ../$(GTEST_DIR) -DCMAKE_INSTALL_PREFIX=$(ROOT)/usr  
	cd $(3RD_PARTY_DIR)/gtest/build; make install 
	rm -rf $(3RD_PARTY_DIR)/gtest/$(GTEST_DIR)
	rm -rf $(3RD_PARTY_DIR)/gtest/build

create-dirs:
	@mkdir -p $(ROOT)/bin
	@mkdir -p $(ROOT)/build
	@mkdir -p $(ROOT)/output
	@mkdir -p $(ROOT)/lib

install:	
	@echo "Installing engine libraries."
	ln -s $(ROOT)/src/engine/usr/lib/*.a $(ROOT)/lib/
	@echo "Installing MicroModelica Compiler."
	ln -s $(ROOT)/src/mmoc/usr/bin/mmoc $(ROOT)/bin/
	@echo "Installing SBML translator."
	ln -s $(ROOT)/src/interfaces/sbml/usr/bin/translate-sbml $(ROOT)/bin/
	@echo "Installing solver GUI."
	@install $(ROOT)/src/gui/usr/bin/qss-solver $(ROOT)/bin/
	@echo "Installing user libraries."
	ln -s $(ROOT)/src/usr/lib/* $(ROOT)/lib/
ifeq ($(OS),Linux)
	@install $(ROOT)/deploy/linux/scripts/* $(ROOT)/bin/
	@install $(ROOT)/deploy/linux/qss-solver.ini.in $(ROOT)/bin/qss-solver.ini
	@install $(ROOT)/src/engine/3rd-party/partitioners/hmetis/khmetis $(ROOT)/bin/
ifeq ($(ARCH),i686)
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
	@echo "Installing GTEST libraries."
	ln -s $(ROOT)/src/3rd-party/gtest/usr/lib/*.a $(ROOT)/lib/
else
ifeq ($(OS),Windows_NT)
	@cp $(ROOT)/deploy/windows/scripts/* $(ROOT)/bin/
	@cp $(ROOT)/deploy/windows/qss-solver.ini.in $(ROOT)/bin/qss-solver.ini
endif
endif
	
.PHONY: clean all 

clean: 
	@cd mmoc && $(MAKE) clean 
	@cd engine && $(MAKE) clean 
	@cd engine && $(MAKE) DEBUG=True clean 
	@cd usr && $(MAKE) clean
	@cd gui && $(MAKE) clean    
	@cd interfaces/sbml && $(MAKE) clean     

help:
	@echo "make DEBUG=<True|False> CORES=<n>"
	@echo "Default values:"
	@echo ""
	@echo "DEBUG=False"
	@echo "CORES=1"
