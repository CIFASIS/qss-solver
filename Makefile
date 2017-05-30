# Target OS variables
OS ?= unix
DEBUG ?= False

# Compiler and Linker
CXX         := g++
BISON 		:= bison++
FLEX 		:= flex++

# The Target Binary Program
TARGET      := ../../bin/mmoc

# The Directories, Source, Includes, Objects, Binary 
ROOTDIR     := ..
3RDPARTYDIR := $(ROOTDIR)/3rd-party
LIBDIR      := ../../lib
SRCDIR      := .
ASTDIR 		:= $(SRCDIR)/ast
GENERATORDIR:= $(SRCDIR)/generator
IRDIR 		:= $(SRCDIR)/ir
PARSERDIR 	:= $(SRCDIR)/parser
UTILDIR 	:= $(SRCDIR)/util
OBJDIR      := obj
BUILDDIR    := $(SRCDIR)/$(OBJDIR)
SRCEXT      := cpp
DEPEXT      := d
OBJEXT      := o


# Deterministic Ginac Library. 

libginac 	:= ../usr/lib/libginac.a 

# Flags, Libraries and Includes
LIB 		:= -L../usr/lib/ -lginac -lcln -lgmp 
ifeq ($(OS),win32)
LIB 		:= -L/local/lib -lginac -lcln -lgmp
endif
CXXFLAGS 	:= -Wno-write-strings -Wall -std=c++11
ifeq ($(DEBUG),True)
CXXFLAGS 	+= -DYY_MCC_Parser_DEBUG -g  
endif
INC         := -I$(SRCDIR) -I../usr/include
ifeq ($(OS),win32)
INC 		+= -I/local/include -I/GnuWin32/include
endif
RMS 		:= rm -rf

# Source files.
ASTSRC = $(ASTDIR)/ast_builder.cpp $(ASTDIR)/ast_node.cpp $(ASTDIR)/class.cpp $(ASTDIR)/composition.cpp $(ASTDIR)/element.cpp $(ASTDIR)/imports.cpp $(ASTDIR)/equation.cpp $(ASTDIR)/expression.cpp $(ASTDIR)/modification.cpp $(ASTDIR)/statement.cpp $(ASTDIR)/stored_definition.cpp

GENERATORSRC = $(GENERATORDIR)/files.cpp  $(GENERATORDIR)/solver.cpp  $(GENERATORDIR)/generator_utils.cpp  $(GENERATORDIR)/generator.cpp 

IRSRC = $(IRDIR)/mmo_model_checker.cpp  $(IRDIR)/mmo_util.cpp  $(IRDIR)/expression.cpp  $(IRDIR)/equation.cpp  $(IRDIR)/statement.cpp  $(IRDIR)/annotation.cpp  $(IRDIR)/event.cpp  $(IRDIR)/class.cpp  $(IRDIR)/stored_definition.cpp  $(IRDIR)/mmo_ir.cpp  $(IRDIR)/mmo_settings.cpp 

PARSERSRC = $(PARSERDIR)/mocc_parser.cpp $(PARSERDIR)/mocc_scanner.cpp $(PARSERDIR)/parse.cpp 

UTILSRC = $(UTILDIR)/util.cpp $(UTILDIR)/ast_util.cpp $(UTILDIR)/type.cpp $(UTILDIR)/symbol_table.cpp $(UTILDIR)/error.cpp $(UTILDIR)/debug.cpp $(UTILDIR)/compile_flags.cpp $(UTILDIR)/derivative.cpp $(UTILDIR)/ginac_interface.cpp $(UTILDIR)/index.cpp $(UTILDIR)/dependencies.cpp  $(UTILDIR)/graph.cpp $(UTILDIR)/graph_profile.cpp 

# Objects
ASTOBJ=$(addprefix $(BUILDDIR)/ast_, $(notdir $(ASTSRC:.cpp=.o)))

GENERATOROBJ=$(addprefix $(BUILDDIR)/gen_, $(notdir $(GENERATORSRC:.cpp=.o)))

IROBJ=$(addprefix $(BUILDDIR)/ir_, $(notdir $(IRSRC:.cpp=.o)))

PARSEROBJ=$(addprefix $(BUILDDIR)/par_, $(notdir $(PARSERSRC:.cpp=.o)))

UTILOBJ=$(addprefix $(BUILDDIR)/util_, $(notdir $(UTILSRC:.cpp=.o)))

GRAPHOBJ=$(addprefix $(GRAPHDIR)/$(OBJDIR)/grp_, $(notdir $(GRAPHSRC:.c=.o)))

# Make dependencies
DEPS = $(ASTOBJ:.o=.d) $(GENERATOROBJ:.o=.d) $(IROBJ:.o=.d) $(PARSEROBJ:.o=.d) $(UTILOBJ:.o=.d)

default: mmoc 

$(libginac):
	echo "Building deterministic GiNaC" 
	cd $(3RDPARTYDIR)/ginac; tar xvjf ginac-1.7.0.tar.bz2
	cd $(3RDPARTYDIR)/ginac/ginac-1.7.0; ./configure --prefix=`pwd`/../../../usr --disable-shared; make -j 4 install
	cp ../usr/lib/*.a $(LIBDIR)
	rm -rf $(3RDPARTYDIR)/ginac/ginac-1.7.0
  
$(BUILDDIR)/ast_%.o : $(ASTDIR)/%.cpp $(ASTDIR)/%.h $(PARSERDIR)/mocc_parser.cpp $(PARSERDIR)/mocc_scanner.cpp
	$(CXX) $(INC) $(CXXFLAGS) -MM -MT $@ -MF $(patsubst %.o,%.d,$@) $<
	$(CXX) $(INC) -c $< -o $@ $(CXXFLAGS)

$(BUILDDIR)/gen_%.o : $(GENERATORDIR)/%.cpp
	$(CXX) $(INC) $(CXXFLAGS) -MM -MT $@ -MF $(patsubst %.o,%.d,$@) $<
	$(CXX) $(INC) -c $< -o $@ $(CXXFLAGS)

$(BUILDDIR)/ir_%.o : $(IRDIR)/%.cpp
	$(CXX) $(INC) $(CXXFLAGS) -MM -MT $@ -MF $(patsubst %.o,%.d,$@) $<
	$(CXX) $(INC) -c $< -o $@ $(CXXFLAGS) 

$(BUILDDIR)/par_%.o : $(PARSERDIR)/%.cpp 
	$(CXX) $(INC) $(CXXFLAGS) -MM -MT $@ -MF $(patsubst %.o,%.d,$@) $<
	$(CXX) $(INC) -c $< -o $@ $(CXXFLAGS) 

$(BUILDDIR)/util_%.o : $(UTILDIR)/%.cpp 
	$(CXX) $(INC) $(CXXFLAGS) -MM -MT $@ -MF $(patsubst %.o,%.d,$@) $<
	$(CXX) $(INC) -c $< -o $@ $(CXXFLAGS) 

mmoc: $(ASTOBJ) $(GENERATOROBJ) $(IROBJ) $(PARSEROBJ) $(UTILOBJ) | $(libginac)
	$(CXX) $(INC) $(CXXFLAGS) main.cpp -o $(TARGET) $(ASTOBJ) $(GENERATOROBJ) $(IROBJ) $(PARSEROBJ) $(UTILOBJ) $(LIB) 

ifneq ($(OS),win32)
$(PARSERDIR)/mocc_parser.cpp: parser/mocc.y
	$(BISON) -t -d -v -hparser/mocc_parser.h -o parser/mocc_parser.cpp parser/mocc.y

$(PARSERDIR)/mocc_scanner.cpp: parser/mocc.lex parser/mocc_parser.cpp
	$(FLEX) -d -oparser/mocc_scanner.cpp parser/mocc.lex
endif

$(ASTOBJ): | $(BUILDDIR)

$(GENERATOROBJ): | $(BUILDDIR)

$(IROBJ): | $(BUILDDIR)

$(PARSEROBJ): | $(BUILDDIR)

$(UTILOBJ): | $(BUILDDIR)

$(GRAPHOBJ): 
	@cd $(GRAPHDIR) && $(MAKE) 

$(BUILDDIR):
	@mkdir -p $(BUILDDIR)

-include $(DEPS)

.PHONY: clean

clean:
ifeq ($(OS),win32)
	$(RMS) $(DEPS) $(TARGET) $(ASTOBJ) $(GENERATOROBJ) $(IROBJ) $(PARSEROBJ) $(UTILOBJ)
else
	$(RMS) $(DEPS) $(TARGET) $(ASTOBJ) $(GENERATOROBJ) $(IROBJ) $(PARSEROBJ) $(UTILOBJ) ./mmocc parser/mocc_parser.cpp parser/mocc_scanner.cpp parser/mocc_parser.h -rf usr
endif

help:
	@echo "make DEBUG=<True|False> OS=<unix|win32|osx>"
	@echo "Default values:"
	@echo ""
	@echo "DEBUG=False"
	@echo "OS=unix"
