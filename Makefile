# Makefile config variables
ifneq ($(OS), Windows_NT)
OS = $(uname)
endif
DEBUG ?= False

# Compiler and Linker
CXX         := g++

# The Directories, Source, Includes, Objects, Binary 
SRCDIR      := .
USRDIR    	:= $(SRCDIR)/usr
BINDIR    	:= $(USRDIR)/bin
BUILDDIR    := $(USRDIR)/obj
SRCEXT      := cpp
DEPEXT      := d
OBJEXT      := o

# The Target Binary Program
TARGET      := $(BINDIR)/translate-sbml

# Flags, Libraries and Includes
LIB 		 		:= -lsbml
ifeq ($(OS),Windows_NT)
LIB 				:= -L/usr/local/lib -lsbml -lxml2
endif
CXXFLAGS 			:= -Wall -Wabi-tag -lm -msse2 -mfpmath=sse 
ifeq ($(DEBUG),True)
CXXFLAGS 			:= -g
endif
INC 				:= -I. 
ifeq ($(OS),Windows_NT)
INC 				+= -I/SBML/win32/include
endif
RMS 				:= rm -rf

# Source files.
SRC = mmo_utils.cpp mmo_decl.cpp mmo_event.cpp mmo_assignment.cpp mmo_zerocrossing.cpp mmo_equation.cpp mmo_math.cpp mmo_model.cpp mmo_section.cpp mmo_convert.cpp mmo_writer.cpp biorica_writer.cpp mmo_function.cpp

# Objects
OBJ=$(addprefix $(BUILDDIR)/, $(notdir $(SRC:.cpp=.o)))

# Make dependencies
DEPS = $(OBJ:.o=.d) 

all: sbml 

$(BUILDDIR)/%.o : $(SRCDIR)/%.cpp 
	$(CXX) $(INC) $(CXXFLAGS) -MM -MT $@ -MF $(patsubst %.o,%.d,$@) $<
	$(CXX) $(INC) $(CXXFLAGS) -c $< -o $@ 

sbml: $(OBJ)
	$(CXX) convert.cpp -o $(TARGET) $(OBJ) $(INC) $(CXXFLAGS) $(LIB)

$(OBJ): | create-dirs

create-dirs:
	@mkdir -p $(BINDIR)
	@mkdir -p $(BUILDDIR)

doc:
	@mkdir -p $(USRDIR)/doc
	@mkdir -p $(USRDIR)/doc/html
	@mkdir -p $(USRDIR)/doc/latex
	doxygen sbml.doxyfile

-include $(DEPS)

.PHONY: clean

clean:
	$(RMS) $(OBJ) $(TARGET)

help:
	@echo "make DEBUG=<True|False>"
	@echo "Default values:"
	@echo ""
	@echo "DEBUG=False"
