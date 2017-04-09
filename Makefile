# Makefile config variables
OS ?= unix
DEBUG ?= False

#Compiler and Linker
CXX         := g++

#The Target Binary Program
TARGET      := ../../../bin/translate-sbml

#The Directories, Source, Includes, Objects, Binary 
SRCDIR      := .
BUILDDIR    := $(SRCDIR)/obj
SRCEXT      := cpp
DEPEXT      := d
OBJEXT      := o

#Flags, Libraries and Includes
LIB 		 		:= -lsbml
ifeq ($(OS),win32)
LIB 				:= -L/usr/local/lib -lsbml -lxml2
endif
CXXFLAGS 		:= -Wall -Wabi-tag -lm -msse2 -mfpmath=sse -D_GLIBCXX_USE_CXX11_ABI=0
ifeq ($(DEBUG),True)
CXXFLAGS 		+= -g
endif
INC 				:= -I.
ifeq ($(OS),win32)
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

$(OBJ): | $(BUILDDIR)

$(BUILDDIR):
	@mkdir -p $(BUILDDIR)
	
-include $(DEPS)

.PHONY: clean

clean:
	$(RMS) $(OBJ) $(TARGET)

help:
	@echo "make DEBUG=<True|False> OS=<unix|win32|mac>"
	@echo "Default values:"
	@echo ""
	@echo "DEBUG=False"
	@echo "OS=unix"
