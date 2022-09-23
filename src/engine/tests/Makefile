# The Directories, Source, Includes, Objects, Binary 
ROOT_DIR       := ../..
SRC_DIR        := .
SYSTEM_DIR     := $(SRC_DIR)/system
DATA_DIR   	   := $(SYSTEM_DIR)/test_data
USR_DIR        := $(SRC_DIR)/usr
BUILD_DIR  	   := $(USR_DIR)/obj
BIN_DIR        := $(USR_DIR)/bin

# Compilation flags
GOOGLE_TEST_INSTALL = $(ROOT_DIR)/3rd-party/gtest
GOOGLE_TEST_LIB = gtest
GOOGLE_MOCK_LIB = gmock
GOOGLE_TEST_INCLUDE = $(GOOGLE_TEST_INSTALL)/usr/include

G++ = g++
G++_FLAGS = -c -Wall -I $(GOOGLE_TEST_INCLUDE) -std=c++11
LD_FLAGS = -L $(GOOGLE_TEST_INSTALL)/usr/lib -l $(GOOGLE_TEST_LIB) -l $(GOOGLE_MOCK_LIB) -l pthread
RM = rm -rf

# The Target Binary Program
TARGET      := $(BIN_DIR)/test-engine
RUN_TESTS   := test-engine

# Source files.
MAIN_SRC = $(SRC_DIR)/main.cpp
SYSTEM_SRC = $(SYSTEM_DIR)/models_test.cpp

# Objects
SYSTEM_OBJ=$(addprefix $(BUILD_DIR)/sys_, $(notdir $(SYSTEM_SRC:.cpp=.o)))

MAIN_OBJ=$(addprefix $(BUILD_DIR)/, $(notdir $(MAIN_SRC:.cpp=.o)))

$(BUILD_DIR)/sys_%.o : $(SYSTEM_DIR)/%.cpp
	$(G++) $(G++_FLAGS) $< -o $@

$(BUILD_DIR)/%.o : $(SRC_DIR)/%.cpp
	$(G++) $(G++_FLAGS) $< -o $@ 

all: $(TARGET) $(RUN_TESTS)

$(TARGET): $(MAIN_OBJ) $(SYSTEM_OBJ)
		g++ -o $(TARGET) $(MAIN_OBJ) $(SYSTEM_OBJ) $(LD_FLAGS)

$(RUN_TESTS): $(TARGET)
		@echo Setup test data dir.
		@rm -rf $(DATA_DIR)/*
		@echo Running tests.
		$(TARGET)
		@echo Done.

$(SYSTEM_OBJ): | $(BUILD_DIR)

$(MAIN_OBJ): | $(BUILD_DIR)

$(BUILD_DIR):
	@mkdir -p $(BIN_DIR)
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(DATA_DIR)

clean:
	$(RM) $(USR_DIR) 
                    
.PHONY: all clean