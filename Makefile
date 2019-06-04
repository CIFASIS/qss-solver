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
ASTDIR 		  := $(SRCDIR)/ast
GENERATORDIR:= $(SRCDIR)/generator
IRDIR 		  := $(SRCDIR)/ir
PARSERDIR 	:= $(SRCDIR)/parser
UTILDIR 	  := $(SRCDIR)/util
VISITORDIR 	:= $(UTILDIR)/visitors
3RDPARTYDIR := $(SRCDIR)/3rd-party
DEPSDIR     := $(SRCDIR)/deps
DEPSBUILDERDIR := $(DEPSDIR)/builders
DEPSGRAPHDIR := $(DEPSDIR)/graph
USRDIR 		  := $(SRCDIR)/usr
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
							 $(GENERATORDIR)/model_instance.cpp  \
							 $(GENERATORDIR)/writer.cpp  \
							 $(GENERATORDIR)/function.cpp  \
							 $(GENERATORDIR)/package.cpp  \
							 $(GENERATORDIR)/macros.cpp  \
							 $(GENERATORDIR)/generator.cpp 

IRSRC = $(IRDIR)/mmo_model_checker.cpp  \
				$(IRDIR)/expression.cpp  \
				$(IRDIR)/equation.cpp  \
				$(IRDIR)/statement.cpp  \
				$(IRDIR)/helpers.cpp  \
				$(IRDIR)/built_in_functions.cpp  \
				$(IRDIR)/class.cpp  \
				$(IRDIR)/index.cpp  \
				$(IRDIR)/annotation.cpp  \
				$(IRDIR)/event.cpp  \
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
					$(UTILDIR)/graph.cpp \
					$(UTILDIR)/graph_profile.cpp \

VISITORSRC = $(VISITORDIR)/array_use.cpp \
						 $(VISITORDIR)/autonomous.cpp \
						 $(VISITORDIR)/called_functions.cpp \
						 $(VISITORDIR)/convert_condition.cpp \
					     $(VISITORDIR)/convert_expression.cpp \
						 $(VISITORDIR)/convert_equation.cpp \
						 $(VISITORDIR)/convert_statement.cpp \
						 $(VISITORDIR)/discrete_assignment.cpp \
						 $(VISITORDIR)/eval_exp.cpp \
						 $(VISITORDIR)/eval_init_exp.cpp \
						 $(VISITORDIR)/expression_printer.cpp \
						 $(VISITORDIR)/occurs.cpp \
						 $(VISITORDIR)/partial_eval_exp.cpp \
						 $(VISITORDIR)/replace_der.cpp \
						 $(VISITORDIR)/replace_index.cpp \
						 $(VISITORDIR)/replace_inner_product.cpp \
						 $(VISITORDIR)/variable_lookup.cpp 

DEPSSRC = $(DEPSDIR)/dependency.cpp \
		  $(DEPSDIR)/model_dependencies.cpp  \
		  $(DEPSDIR)/dependency_matrix.cpp

DEPSGRAPHSRC = $(DEPSGRAPHDIR)/graph.cpp \
			   $(DEPSGRAPHDIR)/graph_helpers.cpp	

DEPSBUILDERSRC = $(DEPSBUILDERDIR)/sd_graph_builder.cpp \
				 $(DEPSBUILDERDIR)/so_graph_builder.cpp \
				 $(DEPSBUILDERDIR)/ds_graph_builder.cpp \
				 $(DEPSBUILDERDIR)/dh_graph_builder.cpp 

# Objects
ASTOBJ=$(addprefix $(BUILDDIR)/ast_, $(notdir $(ASTSRC:.cpp=.o)))

GENERATOROBJ=$(addprefix $(BUILDDIR)/gen_, $(notdir $(GENERATORSRC:.cpp=.o)))

IROBJ=$(addprefix $(BUILDDIR)/ir_, $(notdir $(IRSRC:.cpp=.o)))

PARSEROBJ=$(addprefix $(BUILDDIR)/par_, $(notdir $(PARSERSRC:.cpp=.o)))

UTILOBJ=$(addprefix $(BUILDDIR)/util_, $(notdir $(UTILSRC:.cpp=.o)))

VISITOROBJ=$(addprefix $(BUILDDIR)/util_, $(notdir $(VISITORSRC:.cpp=.o)))

DEPSOBJ=$(addprefix $(BUILDDIR)/deps_, $(notdir $(DEPSSRC:.cpp=.o)))

DEPSGRAPHOBJ=$(addprefix $(BUILDDIR)/deps_, $(notdir $(DEPSGRAPHSRC:.cpp=.o)))

DEPSBUILDEROBJ=$(addprefix $(BUILDDIR)/deps_, $(notdir $(DEPSBUILDERSRC:.cpp=.o)))

# Make dependencies
DEPS = $(ASTOBJ:.o=.d) \
 	   $(GENERATOROBJ:.o=.d) \
 	   $(IROBJ:.o=.d) \
 	   $(PARSEROBJ:.o=.d) \
 	   $(UTILOBJ:.o=.d) \
 	   $(VISITOROBJ:.o=.d) \
 	   $(DEPSOBJ:.o=.d) \
 	   $(DEPSGRAPHOBJ:.o=.d) \
 	   $(DEPSBUILDEROBJ:.o=.d)

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

$(BUILDDIR)/util_%.o : $(VISITORDIR)/%.cpp 
	$(CXX) $(INC) $(CXXFLAGS) -MM -MT $@ -MF $(patsubst %.o,%.d,$@) $<
	$(CXX) $(INC) -c $< -o $@ $(CXXFLAGS) 

$(BUILDDIR)/deps_%.o : $(DEPSDIR)/%.cpp 
	$(CXX) $(INC) $(CXXFLAGS) -MM -MT $@ -MF $(patsubst %.o,%.d,$@) $<
	$(CXX) $(INC) -c $< -o $@ $(CXXFLAGS) 

$(BUILDDIR)/deps_%.o : $(DEPSGRAPHDIR)/%.cpp 
	$(CXX) $(INC) $(CXXFLAGS) -MM -MT $@ -MF $(patsubst %.o,%.d,$@) $<
	$(CXX) $(INC) -c $< -o $@ $(CXXFLAGS) 

$(BUILDDIR)/deps_%.o : $(DEPSBUILDERDIR)/%.cpp 
	$(CXX) $(INC) $(CXXFLAGS) -MM -MT $@ -MF $(patsubst %.o,%.d,$@) $<
	$(CXX) $(INC) -c $< -o $@ $(CXXFLAGS) 

mmoc: | $(libginac) $(ASTOBJ) $(GENERATOROBJ) $(IROBJ) $(PARSEROBJ) $(UTILOBJ) $(VISITOROBJ) $(DEPSOBJ) $(DEPSGRAPHOBJ) $(DEPSBUILDEROBJ)
	$(CXX) $(INC) $(CXXFLAGS) main.cpp -o $(TARGET) $(ASTOBJ) $(GENERATOROBJ) $(IROBJ) $(PARSEROBJ) $(UTILOBJ) $(VISITOROBJ) $(DEPSOBJ) $(DEPSGRAPHOBJ) $(DEPSBUILDEROBJ) $(LIB) 

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

$(VISITOROBJ): | $(BUILDDIR)

$(DEPSOBJ): | $(BUILDDIR)

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
	$(RMS) $(DEPS) $(TARGET) $(ASTOBJ) $(GENERATOROBJ) $(IROBJ) $(PARSEROBJ) $(UTILOBJ) $(DEPSOBJ) ./mmocc parser/mocc_parser.cpp parser/mocc_scanner.cpp parser/mocc_parser.h 
endif

#	$(RMS) $(DEPS) $(TARGET) $(ASTOBJ) $(GENERATOROBJ) $(IROBJ) $(PARSEROBJ) $(UTILOBJ) ./mmocc parser/mocc_parser.cpp parser/mocc_scanner.cpp parser/mocc_parser.h usr

help:
	@echo "make DEBUG=<True|False>"
	@echo "Default values:"
	@echo ""
	@echo "DEBUG=False"
