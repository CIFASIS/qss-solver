# Target OS variables
MODE ?= Debug

# Compiler and Linker
CXX     := g++
BISON 	:= bison++
FLEX 	:= flex++

# The Directories, Source, Includes, Objects, Binary 
SRC_DIR        	:= .
USR_DIR		:= $(SRC_DIR)/usr
BUILD_DIR	:= $(USR_DIR)/obj
LIB_DIR		:= $(USR_DIR)/lib
BIN_DIR		:= $(USR_DIR)/bin
INC_DIR		:= $(USR_DIR)/include

# The Target Binary Program
TARGET      := $(BIN_DIR)/mmoc

# Flags, Libraries and Includes
LIB 	 += -L$(LIB_DIR) -lginac -lcln -lgmp   
CXXFLAGS := -Wno-write-strings -Wall -std=c++17
ifeq ($(MODE),Debug)
CXXFLAGS += -DYY_MCC_Parser_DEBUG -g  
endif
INCLUDE  := -I$(SRC_DIR) -I$(INC_DIR)
RM	 := rm -rf

default: mmoc 

include ast/Makefile.include
include deps/Makefile.include
include generator/Makefile.include
include ir/Makefile.include
include util/Makefile.include

# Build objects.

$(BUILD_DIR)/%.o : %.cpp
	$(CXX) $(INCLUDE) $(CXXFLAGS) -MM -MT $@ -MF $(patsubst %.o,%.d,$@) $<
	$(CXX) $(INCLUDE) -c $< -o $@ $(CXXFLAGS)

# Make dependencies
DEPS	=\
	$(AST_OBJ:.o=.d)\
	$(PARSER_OBJ:.o=.d)\
	$(GENERATOR_OBJ:.o=.d)\
	$(IR_OBJ:.o=.d)\
	$(UTIL_OBJ:.o=.d)\
	$(VISITOR_OBJ:.o=.d)\
	$(DEPS_OBJ:.o=.d)\
	$(DEPS_GRAPH_OBJ:.o=.d)\
	$(DEPS_BUILDER_OBJ:.o=.d)

mmoc: | $(AST_OBJ) $(UTIL_OBJ) $(DEPS_OBJ) $(IR_OBJ) $(GENERATOR_OBJ) 
	$(CXX) $(INCLUDE) $(CXXFLAGS) main.cpp -o $(TARGET) $(AST_OBJ) $(PARSER_OBJ) $(GENERATOR_OBJ) $(IR_OBJ) $(UTIL_OBJ) $(VISITOR_OBJ) $(DEPS_OBJ) $(DEPS_GRAPH_OBJ) $(DEPS_BUILDER_OBJ) $(LIB) 

test: 
	@echo Running MMOC tests
	@cd tests && $(MAKE)
	@echo Done

create-folders::
	@mkdir -p $(BIN_DIR)
	@mkdir -p $(INC_DIR)
	@mkdir -p $(LIB_DIR)
	@mkdir -p $(BUILD_DIR)

doc:
	@mkdir -p $(USR_DIR)/doc
	@mkdir -p $(USR_DIR)/doc/html
	doxygen MMOCompiler.doxyfile

-include $(DEPS)

.PHONY: clean

clean:
	$(RM) $(USR_DIR)

help:
	@echo "make MODE=<Debug|Release>"
	@echo "Default values:"
	@echo ""
	@echo "MODE=Debug"
