# Target variables
MODE ?= Debug
DISTRO := $(shell lsb_release -r 2>/dev/null | grep Release | awk '{ print $$2 }')
sbg_branch ?= sb-graph-dev
build_sbg ?= True
repo_checkout ?= ssh

# Build cores.
CORES = $(shell nproc)

# The Directories, Source, Includes, Objects, Binary 
ROOT   	        := ..
3RD_PARTY_DIR   := ./3rd-party
GTEST_DIR       := googletest-release-1.10.0
GTEST_CODE      := $(GTEST_DIR).tar.gz

all: mmo-compiler mmo-interfaces qss-engine qss-retqss-lib qss-solver-gui qss-user-libs

mmo-interfaces: create-folders
	@echo BUILDING MMO-SBML INTERFACE
	@cd interfaces/sbml &&  $(MAKE) MODE=$(MODE) -j $(CORES)
	@echo Done

qss-engine: create-folders  lib-gtest
	@echo BUILDING QSS LIBRARY DEPS
	@cd engine && $(MAKE) cvode
	@cd engine && $(MAKE) ida
	@cd engine && $(MAKE) gsl
	@echo BUILDING QSS LIBRARY
	@cd engine && $(MAKE) MODE=Release
	@echo Done
	@echo BUILDING QSS DEBUG LIBRARY
	@cd engine && $(MAKE) MODE=Debug
	@echo Done

qss-retqss-lib: create-folders
	@echo BUILDING QSS RETQSS LIBRARY
	@cd lib/retqss && $(MAKE) MODE=Release
	@echo Done
	@echo BUILDING QSS RETQSS DEBUG LIBRARY
	@cd lib/retqss && $(MAKE) MODE=Debug RETQSS_DEBUG=True
	@echo Done

qss-user-libs: create-folders qss-engine
	@echo BUILDING USER LIBS
	@cd usr && $(MAKE) MODE=$(MODE) -j $(CORES) ENGINE=../engine     
	@echo Done

mmo-compiler: create-folders lib-gtest  
	@echo BUILDING MICROMODELICA C COMPILER DEPS
	@cd mmoc && $(MAKE) MODE=$(MODE) lib_sbg sbg_branch=$(sbg_branch) build_sbg=$(build_sbg) repo_checkout=$(repo_checkout)
	@echo Done
	@echo BUILDING MICROMODELICA C COMPILER
	@cd mmoc && $(MAKE) MODE=$(MODE) -j $(CORES) 
	@echo Done

qss-solver-gui: create-folders lib-gtest
	@echo BUILDING MMO EDITOR
	@cd gui && $(MAKE) -f gui.makefile MODE=$(MODE) -j $(CORES)     
	@echo Done

qss-solver-doc: qss-engine mmo-compiler qss-solver-gui mmo-interfaces
	@echo BUILDING PROJECT DOCUMENTATION
	@cd engine && $(MAKE) doc 
	@cd mmoc && $(MAKE) doc 
	@cd gui && $(MAKE) -f gui.makefile doc 
	@cd interfaces/sbml && $(MAKE) doc 

lib-gtest: | create-folders 
ifeq ("$(wildcard $(3RD_PARTY_DIR)/gtest/usr/lib)","")
	mkdir -p $(3RD_PARTY_DIR)/gtest/usr
	cp -r $(3RD_PARTY_DIR)/gtest/ubuntu-$(DISTRO)/* $(3RD_PARTY_DIR)/gtest/usr/
endif

test:
	@echo COMPILE AND RUN TESTS
	@cd mmoc && $(MAKE) test
	@cd engine && $(MAKE) test
	@cd gui && $(MAKE) -f gui.makefile test
	@echo Done

create-folders:
	@mkdir -p $(ROOT)/bin
	@mkdir -p $(ROOT)/build
	@mkdir -p $(ROOT)/output
	@mkdir -p $(ROOT)/lib

install:	
	@echo "Installing engine libraries."
	@install $(ROOT)/src/engine/usr/lib/*.a $(ROOT)/lib/
	@echo "Installing MicroModelica Compiler."
	@install $(ROOT)/src/mmoc/usr/bin/mmoc $(ROOT)/bin/
	@echo "Installing SBML translator."
	@install $(ROOT)/src/interfaces/sbml/usr/bin/translate-sbml $(ROOT)/bin/
	@echo "Installing solver GUI."
	@install $(ROOT)/src/gui/usr/bin/qss-solver $(ROOT)/bin/
	@echo "Installing user libraries."
	@install $(ROOT)/src/usr/lib/* $(ROOT)/lib/
	@install $(ROOT)/deploy/linux/qss-solver.ini.in $(ROOT)/bin/qss-solver.ini
	@install $(ROOT)/src/engine/3rd-party/partitioners/hmetis/khmetis $(ROOT)/bin/
	@install $(ROOT)/src/engine/3rd-party/partitioners/patoh/Linux-x86_64/libpatoh.a $(ROOT)/lib/
	@install $(ROOT)/src/engine/3rd-party/partitioners/metis/Linux-x86_64/libmetis.a $(ROOT)/lib/
	@echo "Installing RET QSS libraries."
	@install $(ROOT)/src/lib/retqss/usr/lib/* $(ROOT)/lib/
	@echo "Installing GTEST libraries."
	@install $(ROOT)/src/3rd-party/gtest/usr/lib/*.a $(ROOT)/lib/
	
.PHONY: clean all 

clean: 
	@cd mmoc && $(MAKE) clean 
	@cd engine && $(MAKE) clean 
	@cd usr && $(MAKE) clean
	@cd gui && $(MAKE) clean    
	@cd interfaces/sbml && $(MAKE) clean     
	@cd lib/retqss && $(MAKE) clean     

help:
	@echo "make MODE=<Debug|Release> sbg_branch=<BRANCH_NAME> build_sbg=<True|False> repo_checkout=<ssh|https>"
	@echo "Default values:"
	@echo ""
	@echo "MODE=Debug"
	@echo "sbg_branch=sb-graph-dev"
	@echo "build_sbg=True"
	@echo "repo_checkout=ssh"