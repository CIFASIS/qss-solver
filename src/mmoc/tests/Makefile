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
PKG_DATA_DIR   := $(DATA_DIR)/packages
PKG_GT_DATA_DIR:= $(INT_DIR)/gt_data/packages
MOCK_DIR 	   := $(SRC_DIR)/mock
USR_DIR        := $(SRC_DIR)/usr
BUILD_DIR  	   := $(USR_DIR)/obj
BIN_DIR        := $(USR_DIR)/bin

# Flags, Libraries and Includes
GOOGLE_TEST_INSTALL = $(ROOT_DIR)/3rd-party/gtest
GOOGLE_TEST_LIB = gtest
GOOGLE_MOCK_LIB = gmock
GOOGLE_TEST_INCLUDE = $(GOOGLE_TEST_INSTALL)/usr/include
LIB 		+= -L$(MMOC_LIB_DIR) -lginac -lcln -lgmp  
G++ = g++
G++_FLAGS = -c -Wall -I $(GOOGLE_TEST_INCLUDE) -std=c++17 -I $(MMOC_DIR)
LD_FLAGS = -L $(GOOGLE_TEST_INSTALL)/usr/lib -l $(GOOGLE_TEST_LIB) -l $(GOOGLE_MOCK_LIB) -l pthread 
RM = rm -rf

# The Target Binary Program
UNIT_TEST   	:= $(BIN_DIR)/unit-test-mmoc
MODEL_INT_TEST	:= $(BIN_DIR)/model-int-test-mmoc
PACKAGE_INT_TEST:= $(BIN_DIR)/package-int-test-mmoc
RUN_TESTS   := unit-test-mmoc model-int-test-mmoc package-int-test-mmoc

# Source files.
MAIN_SRC = $(SRC_DIR)/main.cpp \
		   $(SRC_DIR)/test_utils.cpp

UNIT_SRC = $(UNIT_DIR)/algebraics_test.cpp \
		   $(UNIT_DIR)/array_use_test.cpp \
		   $(UNIT_DIR)/autonomous_test.cpp \
		   $(UNIT_DIR)/called_functions_test.cpp \
		   $(UNIT_DIR)/convert_cont_red_test.cpp	

MODEL_INT_SRC = $(INT_DIR)/models_test.cpp

PACKAGE_INT_SRC = $(INT_DIR)/packages_test.cpp

# Objects
UNIT_OBJ=$(addprefix $(BUILD_DIR)/unit_, $(notdir $(UNIT_SRC:.cpp=.o)))
MODEL_INT_OBJ=$(addprefix $(BUILD_DIR)/int_, $(notdir $(MODEL_INT_SRC:.cpp=.o)))
PACKAGE_INT_OBJ=$(addprefix $(BUILD_DIR)/int_, $(notdir $(PACKAGE_INT_SRC:.cpp=.o)))
MMOC_OBJ=$(wildcard $(MMOC_OBJ_DIR)/*/*.o $(MMOC_OBJ_DIR)/*/*/*.o)
MAIN_OBJ=$(addprefix $(BUILD_DIR)/, $(notdir $(MAIN_SRC:.cpp=.o)))

$(BUILD_DIR)/unit_%.o : $(UNIT_DIR)/%.cpp
	$(G++) $(G++_FLAGS) $< -o $@

$(BUILD_DIR)/int_%.o : $(INT_DIR)/%.cpp
	$(G++) $(G++_FLAGS) $< -o $@

$(BUILD_DIR)/%.o : $(SRC_DIR)/%.cpp
	$(G++) $(G++_FLAGS) $< -o $@ 

all: $(UNIT_TEST) $(MODEL_INT_TEST) $(PACKAGE_INT_TEST) $(RUN_TESTS)

$(UNIT_TEST): $(MAIN_OBJ) $(UNIT_OBJ) $(MMOC_OBJ) $(BUILD_DIR)
		g++ -o $(UNIT_TEST) $(MAIN_OBJ) $(UNIT_OBJ) $(MMOC_OBJ) $(LD_FLAGS) $(LIB)

$(MODEL_INT_TEST): $(MAIN_OBJ) $(MODEL_INT_OBJ) $(MMOC_OBJ) $(BUILD_DIR)
		g++ -o $(MODEL_INT_TEST) $(MAIN_OBJ) $(MODEL_INT_OBJ) $(MMOC_OBJ) $(LD_FLAGS) $(LIB)

$(PACKAGE_INT_TEST): $(MAIN_OBJ) $(PACKAGE_INT_OBJ) $(MMOC_OBJ) $(BUILD_DIR)
		g++ -o $(PACKAGE_INT_TEST) $(MAIN_OBJ) $(PACKAGE_INT_OBJ) $(MMOC_OBJ) $(LD_FLAGS) $(LIB)

$(RUN_TESTS): $(UNIT_TEST) $(MODEL_INT_TEST) $(PACKAGE_INT_TEST) $(BUILD_DIR)
		@echo Setup test data dir.
		@rm -rf $(DATA_DIR)/*
		@mkdir -p $(PKG_DATA_DIR)
		@cp $(PKG_GT_DATA_DIR)/*.mo $(PKG_DATA_DIR)
		@echo Done
		@echo Running unit tests.
		$(UNIT_TEST)
		@echo Done.
		@echo Running model integration tests.
		$(MODEL_INT_TEST)
		@echo Done.
		@echo Running package integration tests.
		$(PACKAGE_INT_TEST)
		@echo Done.


$(UNIT_OBJ): | $(BUILD_DIR)

$(MODEL_INT_OBJ): | $(BUILD_DIR)

$(PACKAGE_INT_OBJ): | $(BUILD_DIR)

$(MAIN_OBJ): | $(BUILD_DIR)

$(BUILD_DIR):
	@mkdir -p $(BIN_DIR)
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(DATA_DIR)

clean:
	$(RM) $(USR_DIR) 
                    
.PHONY: all clean