# The Directories, Source, Includes, Objects, Binary 
ROOT_DIR       := ../..
SRC_DIR        := .
UNIT_DIR	   := $(SRC_DIR)/unit
INT_DIR   	   := $(SRC_DIR)/integration
MOCK_DIR 	   := $(SRC_DIR)/mock
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
TARGET      := $(BIN_DIR)/test-mmoc
RUN_TESTS   := test-mmoc

# Source files.
MAIN_SRC = $(SRC_DIR)/main.cpp
UNIT_SRC = $(UNIT_DIR)/dummy.cpp
#INT_SRC = $(INT_DIR)/dummy.cpp
#MOCK_SRC = $(MOCK_DIR)/dummy.cpp

# Objects
UNIT_OBJ=$(addprefix $(BUILD_DIR)/unit_, $(notdir $(UNIT_SRC:.cpp=.o)))

MAIN_OBJ=$(addprefix $(BUILD_DIR)/, $(notdir $(MAIN_SRC:.cpp=.o)))

$(BUILD_DIR)/unit_%.o : $(UNIT_DIR)/%.cpp
	$(G++) $(G++_FLAGS) $< -o $@

$(BUILD_DIR)/%.o : $(SRC_DIR)/%.cpp
	$(G++) $(G++_FLAGS) $< -o $@ 

all: $(TARGET) $(RUN_TESTS)

$(TARGET): $(MAIN_OBJ) $(UNIT_OBJ)
		g++ -o $(TARGET) $(MAIN_OBJ) $(UNIT_OBJ) $(LD_FLAGS)

$(RUN_TESTS): $(TARGET)
		@echo Running tests.
		$(TARGET)
		@echo Done.

$(UNIT_OBJ): | $(BUILD_DIR)

$(MAIN_OBJ): | $(BUILD_DIR)

$(BUILD_DIR):
	@mkdir -p $(BIN_DIR)
	@mkdir -p $(BUILD_DIR)

clean:
	$(RM) $(USR_DIR) 
                    
.PHONY: all clean