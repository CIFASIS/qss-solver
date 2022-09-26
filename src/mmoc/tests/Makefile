# The Directories, Source, Includes, Objects, Binary 
ROOT_DIR       := ../..
MMOC_DIR       := ..
MMOC_USR_DIR   := $(MMOC_DIR)/usr
MMOC_OBJ_DIR   := $(MMOC_USR_DIR)/obj
MMOC_LIB_DIR   := $(MMOC_USR_DIR)/lib
SRC_DIR        := .
INT_DIR	   	   := $(SRC_DIR)/integration
SYS_DIR   	   := $(SRC_DIR)/system
DATA_DIR   	   := $(SYS_DIR)/test_data
PKG_DATA_DIR   := $(DATA_DIR)/packages
PKG_GT_DATA_DIR:= $(SYS_DIR)/gt_data/packages
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
INT_TEST     	:= $(BIN_DIR)/int-test-mmoc
MODEL_SYS_TEST	:= $(BIN_DIR)/model-sys-test-mmoc
PACKAGE_SYS_TEST:= $(BIN_DIR)/package-sys-test-mmoc
RUN_TESTS   := int-test-mmoc model-sys-test-mmoc package-sys-test-mmoc

# Source files.
MAIN_SRC = $(SRC_DIR)/main.cpp \
		   $(SRC_DIR)/test_utils.cpp

INT_SRC  = $(INT_DIR)/algebraics_test.cpp \
		   $(INT_DIR)/array_use_test.cpp \
		   $(INT_DIR)/autonomous_test.cpp \
		   $(INT_DIR)/called_functions_test.cpp \
		   $(INT_DIR)/convert_cont_red_test.cpp	

MODEL_SYS_SRC = $(SYS_DIR)/models_test.cpp

PACKAGE_SYS_SRC = $(SYS_DIR)/packages_test.cpp

# Objects
INT_OBJ=$(addprefix $(BUILD_DIR)/int_, $(notdir $(INT_SRC:.cpp=.o)))
MODEL_SYS_OBJ=$(addprefix $(BUILD_DIR)/sys_, $(notdir $(MODEL_SYS_SRC:.cpp=.o)))
PACKAGE_SYS_OBJ=$(addprefix $(BUILD_DIR)/sys_, $(notdir $(PACKAGE_SYS_SRC:.cpp=.o)))
MMOC_OBJ=$(wildcard $(MMOC_OBJ_DIR)/*/*.o $(MMOC_OBJ_DIR)/*/*/*.o)
MAIN_OBJ=$(addprefix $(BUILD_DIR)/, $(notdir $(MAIN_SRC:.cpp=.o)))

$(BUILD_DIR)/int_%.o : $(INT_DIR)/%.cpp
	$(G++) $(G++_FLAGS) $< -o $@

$(BUILD_DIR)/sys_%.o : $(SYS_DIR)/%.cpp
	$(G++) $(G++_FLAGS) $< -o $@

$(BUILD_DIR)/%.o : $(SRC_DIR)/%.cpp
	$(G++) $(G++_FLAGS) $< -o $@ 

all: $(INT_TEST) $(MODEL_SYS_TEST) $(PACKAGE_SYS_TEST) $(RUN_TESTS)

$(INT_TEST): $(MAIN_OBJ) $(INT_OBJ) $(MMOC_OBJ) $(BUILD_DIR)
		g++ -o $(INT_TEST) $(MAIN_OBJ) $(INT_OBJ) $(MMOC_OBJ) $(LD_FLAGS) $(LIB)

$(MODEL_SYS_TEST): $(MAIN_OBJ) $(MODEL_SYS_OBJ) $(MMOC_OBJ) $(BUILD_DIR)
		g++ -o $(MODEL_SYS_TEST) $(MAIN_OBJ) $(MODEL_SYS_OBJ) $(MMOC_OBJ) $(LD_FLAGS) $(LIB)

$(PACKAGE_SYS_TEST): $(MAIN_OBJ) $(PACKAGE_SYS_OBJ) $(MMOC_OBJ) $(BUILD_DIR)
		g++ -o $(PACKAGE_SYS_TEST) $(MAIN_OBJ) $(PACKAGE_SYS_OBJ) $(MMOC_OBJ) $(LD_FLAGS) $(LIB)

$(RUN_TESTS): $(INT_TEST) $(MODEL_SYS_TEST) $(PACKAGE_SYS_TEST) $(BUILD_DIR)
		@echo Setup test data dir.
		@rm -rf $(DATA_DIR)/*
		@mkdir -p $(PKG_DATA_DIR)
		@cp $(PKG_GT_DATA_DIR)/*.mo $(PKG_DATA_DIR)
		@echo Done
		@echo Running integration tests.
		$(INT_TEST)
		@echo Done.
		@echo Running model system tests.
		$(MODEL_SYS_TEST)
		@echo Done.
		@echo Running package system tests.
		$(PACKAGE_SYS_TEST)
		@echo Done.


$(INT_OBJ): | $(BUILD_DIR)

$(MODEL_SYS_OBJ): | $(BUILD_DIR)

$(PACKAGE_SYS_OBJ): | $(BUILD_DIR)

$(MAIN_OBJ): | $(BUILD_DIR)

$(BUILD_DIR):
	@mkdir -p $(BIN_DIR)
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(DATA_DIR)

clean:
	$(RM) $(USR_DIR) 
                    
.PHONY: all clean