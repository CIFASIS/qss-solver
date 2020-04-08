# Target OS variables
ifneq ($(OS), Windows_NT)
OS = $(shell uname)
endif

# The Directories, Source, Includes, Objects, Binary 
ROOT_DIR       := ../..
MMOC_DIR       := ..
MMOC_USR_DIR   := $(MMOC_DIR)/usr
MMOC_OBJ_DIR   := $(MMOC_USR_DIR)/obj
MMOC_LIB_DIR   := $(MMOC_USR_DIR)/lib
SRC_DIR        := .
UNIT_DIR	   := $(SRC_DIR)/unit
INT_DIR   	   := $(SRC_DIR)/integration
DATA_DIR   	   := $(INT_DIR)/test_data
MOCK_DIR 	   := $(SRC_DIR)/mock
USR_DIR        := $(SRC_DIR)/usr
BUILD_DIR  	   := $(USR_DIR)/obj
BIN_DIR        := $(USR_DIR)/bin

# Flags, Libraries and Includes
GOOGLE_TEST_INSTALL = $(ROOT_DIR)/3rd-party/gtest
GOOGLE_TEST_LIB = gtest
GOOGLE_MOCK_LIB = gmock
GOOGLE_TEST_INCLUDE = $(GOOGLE_TEST_INSTALL)/usr/local/include
ifeq ($(OS), Windows_NT)
LIB 		:= -L/usr/lib
endif
LIB 		+= -L$(MMOC_LIB_DIR) -lginac -lcln -lgmp  
G++ = g++
G++_FLAGS = -c -Wall -I $(GOOGLE_TEST_INCLUDE) -std=c++11 -I $(MMOC_DIR)
LD_FLAGS = -L $(GOOGLE_TEST_INSTALL)/usr/lib -l $(GOOGLE_TEST_LIB) -l $(GOOGLE_MOCK_LIB) -l pthread 
RM = rm -rf

# The Target Binary Program
TARGET      := $(BIN_DIR)/test-mmoc
RUN_TESTS   := test-mmoc

# Source files.
MAIN_SRC = $(SRC_DIR)/main.cpp
UNIT_SRC = $(UNIT_DIR)/algebraics_test.cpp
INT_SRC = $(INT_DIR)/models_test.cpp
#MOCK_SRC = $(MOCK_DIR)/dummy.cpp

# Objects
UNIT_OBJ=$(addprefix $(BUILD_DIR)/unit_, $(notdir $(UNIT_SRC:.cpp=.o)))
INT_OBJ=$(addprefix $(BUILD_DIR)/int_, $(notdir $(INT_SRC:.cpp=.o)))
MMOC_OBJ=$(wildcard $(MMOC_OBJ_DIR)/*.o)

MAIN_OBJ=$(addprefix $(BUILD_DIR)/, $(notdir $(MAIN_SRC:.cpp=.o)))

$(BUILD_DIR)/unit_%.o : $(UNIT_DIR)/%.cpp
	$(G++) $(G++_FLAGS) $< -o $@

$(BUILD_DIR)/int_%.o : $(INT_DIR)/%.cpp
	$(G++) $(G++_FLAGS) $< -o $@

$(BUILD_DIR)/%.o : $(SRC_DIR)/%.cpp
	$(G++) $(G++_FLAGS) $< -o $@ 

all: $(TARGET) $(RUN_TESTS)

$(TARGET): $(MAIN_OBJ) $(UNIT_OBJ) $(INT_OBJ) $(MMOC_OBJ) $(BUILD_DIR)
		g++ -o $(TARGET) $(MAIN_OBJ) $(UNIT_OBJ) $(INT_OBJ) $(MMOC_OBJ) $(LD_FLAGS) $(LIB)

$(RUN_TESTS): $(TARGET)
		@echo Clean up test data dir.
		@rm -rf $(DATA_DIR)/*
		@echo Done
		@echo Running tests.
		$(TARGET)
		@echo Done.

$(UNIT_OBJ): | $(BUILD_DIR)

$(INT_OBJ): | $(BUILD_DIR)

$(MAIN_OBJ): | $(BUILD_DIR)

$(BUILD_DIR):
	@mkdir -p $(BIN_DIR)
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(DATA_DIR)

clean:
	$(RM) $(USR_DIR) 
                    
.PHONY: all clean