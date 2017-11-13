# Target OS variables
ifneq ($(OS), Windows_NT)
OS = $(shell uname)
endif
DEBUG ?= False

# Compiler and Linker
CXX         := g++
ifneq ($(OS), Windows_NT)
BISON 		:= bison++
FLEX 		:= flex++
endif

# The Directories, Source, Includes, Objects, Binary 
SRCDIR      := .
ASTDIR 		:= $(SRCDIR)/ast
GENERATORDIR:= $(SRCDIR)/generator
IRDIR 		:= $(SRCDIR)/ir
PARSERDIR 	:= $(SRCDIR)/parser
UTILDIR 	:= $(SRCDIR)/util
3RDPARTYDIR := $(SRCDIR)/3rd-party
USRDIR 		:= $(SRCDIR)/usr
BUILDDIR    := $(USRDIR)/obj
LIBDIR      := $(USRDIR)/lib
BINDIR      := $(USRDIR)/bin
INCDIR      := $(USRDIR)/include
SRCEXT      := cpp
DEPEXT      := d
OBJEXT      := o

# The Target Binary Program
TARGET      := $(BINDIR)/mmoc

# Flags, Libraries and Includes
ifeq ($(OS), Windows_NT)
LIB 		:= -L/usr/lib
endif
LIB 		+= -L$(LIBDIR) -lginac -lcln -lgmp 
CXXFLAGS 	:= -Wno-write-strings -Wall -std=c++11
ifeq ($(DEBUG),True)
CXXFLAGS 	+= -DYY_MCC_Parser_DEBUG -g  
endif
INC         := -I$(SRCDIR) -I$(INCDIR)
ifeq ($(OS), Windows_NT)
INC 		+= -I/usr/include
endif
RMS 		:= rm -rf

# Source files.
ASTSRC = $(ASTDIR)/ast_builder.cpp \
				 $(ASTDIR)/ast_node.cpp \
				 $(ASTDIR)/class.cpp \
				 $(ASTDIR)/composition.cpp \
				 $(ASTDIR)/element.cpp \
				 $(ASTDIR)/imports.cpp \
				 $(ASTDIR)/equation.cpp \
				 $(ASTDIR)/expression.cpp \
				 $(ASTDIR)/modification.cpp \
				 $(ASTDIR)/statement.cpp \
				 $(ASTDIR)/stored_definition.cpp

GENERATORSRC = $(GENERATORDIR)/files.cpp  \
							 $(GENERATORDIR)/solver.cpp  \
							 $(GENERATORDIR)/generator_utils.cpp  \
							 $(GENERATORDIR)/generator.cpp 

IRSRC = $(IRDIR)/mmo_model_checker.cpp  \
				$(IRDIR)/mmo_util.cpp  \
				$(IRDIR)/expression.cpp  \
				$(IRDIR)/equation.cpp  \
				$(IRDIR)/statement.cpp  \
				$(IRDIR)/annotation.cpp  \
				$(IRDIR)/event.cpp  \
				$(IRDIR)/class.cpp \
				$(IRDIR)/stored_definition.cpp  \
				$(IRDIR)/mmo_ir.cpp  \
				$(IRDIR)/mmo_settings.cpp 

PARSERSRC = $(PARSERDIR)/mocc_parser.cpp \
						$(PARSERDIR)/mocc_scanner.cpp \
						$(PARSERDIR)/parse.cpp 

UTILSRC = $(UTILDIR)/util.cpp \
					$(UTILDIR)/ast_util.cpp \
					$(UTILDIR)/type.cpp \
					$(UTILDIR)/symbol_table.cpp \
					$(UTILDIR)/error.cpp \
					$(UTILDIR)/debug.cpp \
					$(UTILDIR)/compile_flags.cpp \
					$(UTILDIR)/derivative.cpp \
					$(UTILDIR)/ginac_interface.cpp \
					$(UTILDIR)/index.cpp \
					$(UTILDIR)/md_index.cpp \
					$(UTILDIR)/dependencies.cpp  \
					$(UTILDIR)/graph.cpp \
					$(UTILDIR)/graph_profile.cpp 

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

# Deterministic Ginac Library. 

libginac 	:= $(LIBDIR)/libginac.a 

$(libginac):
ifneq ($(OS), Darwin)
	echo "Building deterministic GiNaC" 
	cd $(3RDPARTYDIR)/ginac; tar xvjf ginac-1.7.0.tar.bz2
	cd $(3RDPARTYDIR)/ginac/ginac-1.7.0; ./configure --prefix=`pwd`/../../../usr --disable-shared; make -j 4 install
	rm -rf $(3RDPARTYDIR)/ginac/ginac-1.7.0
else
	echo $(OS)
	echo "Using system provided GiNaC library"
endif

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

mmoc: | $(libginac) $(ASTOBJ) $(GENERATOROBJ) $(IROBJ) $(PARSEROBJ) $(UTILOBJ) 
	$(CXX) $(INC) $(CXXFLAGS) main.cpp -o $(TARGET) $(ASTOBJ) $(GENERATOROBJ) $(IROBJ) $(PARSEROBJ) $(UTILOBJ) $(LIB) 

ifneq ($(OS), Windows_NT)
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

$(BUILDDIR):
	@mkdir -p $(BINDIR)
	@mkdir -p $(INCDIR)
	@mkdir -p $(LIBDIR)
	@mkdir -p $(BUILDDIR)

doc:
	@mkdir -p $(USRDIR)/doc
	@mkdir -p $(USRDIR)/doc/html
	doxygen MMOCompiler.doxyfile

-include $(DEPS)

.PHONY: clean

clean:
ifeq ($(OS), Windows_NT)
	$(RMS) $(DEPS) $(TARGET) $(ASTOBJ) $(GENERATOROBJ) $(IROBJ) $(PARSEROBJ) $(UTILOBJ)
else
	$(RMS) $(DEPS) $(TARGET) $(ASTOBJ) $(GENERATOROBJ) $(IROBJ) $(PARSEROBJ) $(UTILOBJ) ./mmocc parser/mocc_parser.cpp parser/mocc_scanner.cpp parser/mocc_parser.h usr
endif

help:
	@echo "make DEBUG=<True|False>"
	@echo "Default values:"
	@echo ""
	@echo "DEBUG=False"
