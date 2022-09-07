# Target variables
MODE ?= Debug

# Compiler and Linker
CC := gcc
FC := gfortran

# The Directories, Source, Includes, Objects, Binary
SRC_DIR 		:= .
USR_DIR 		:= $(SRC_DIR)/usr
PWD= $(shell pwd)
BUILD_DIR    	:= $(USR_DIR)/obj/release
ifeq ($(MODE),Debug)
BUILD_DIR    	:= $(USR_DIR)/obj/debug
endif
LIB_DIR   		:= $(USR_DIR)/lib
INCLUDE_DIR 	:= $(USR_DIR)/include

# Libraries
LIB_QSS       := $(LIB_DIR)/libqss.a
LIB_TIME_STEP := $(LIB_DIR)/libtimestep.a
ifeq ($(MODE),Debug)
LIB_QSS       := $(LIB_DIR)/libqssd.a
LIB_TIME_STEP := $(LIB_DIR)/libtimestepd.a
endif

vpath %.c $(SRC_DIR)
.SUFFIXES: .c

all: cvode ida gsl qss time-step

include 3rd-party/Makefile.include
include common/Makefile.include
include qss/Makefile.include
include classic/Makefile.include

# Flags, Libraries and Includes
CFLAGS 		:= -Wall -msse2 -mfpmath=sse -O2
ifeq ($(MODE),Debug)
CFLAGS 		:= -Wall -msse2 -mfpmath=sse -g -DDEBUG
endif
LIB         := -lm
INCLUDE     := -I$(COMMON_DIR) -I$(SRC_DIR) -I$(INCLUDE_DIR)
RM  		:= rm -rf

# Make dependencies
DEPS = $(COMMON_OBJ:.o=.d) $(SEQ_OBJ:.o=.d) $(PAR_OBJ:.o=.d) $(CLASSIC_OBJ:.o=.d)

# Build Objects.

$(BUILD_DIR)/$(QSS_PAR_DIR)/%.o: $(QSS_DIR)/%.c
	$(CC) $(INCLUDE) $(CFLAGS) -MM -MT $@ -MF $(patsubst %.o,%.d,$@) $<
	$(CC) $(INCLUDE) $(CFLAGS) -DQSS_PARALLEL -c $< -o $@ 

$(BUILD_DIR)/%.o: %.c
	$(CC) $(INCLUDE) $(CFLAGS) -MM -MT $@ -MF $(patsubst %.o,%.d,$@) $<
	$(CC) $(INCLUDE) $(CFLAGS) -c $< -o $@ 

$(BUILD_DIR)/%.o : %.f
	$(FC) $(INC) -c $< -o $@ $(CFLAGS)

$(LIB_TIME_STEP): $(DASSL_OBJ) $(DOPRI_OBJ)
	@ar rvs $(@) $(DASSL_OBJ) $(DOPRI_OBJ)

$(LIB_QSS): $(QSS_OBJS)
	@ar rsc $@ $(QSS_OBJS)
	$(RM) $(LIB_DIR)/*.so*

create-folders::
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(LIB_DIR)

# Rules

qss: $(LIB_QSS)

time-step: $(LIB_TIME_STEP)

test:
	@echo Compile and Run tests.
	@cd tests && $(MAKE) 
	@echo Done

doc:
	@mkdir -p $(USR_DIR)/doc
	@mkdir -p $(USR_DIR)/doc/html
	doxygen QSSSolver.doxyfile

-include $(DEPS)

.PHONY: clean

clean:
	$(RM) $(USR_DIR) 

help:
	@echo "make MODE=<Debug|Release>"
	@echo "Default value:"
	@echo ""
	@echo "MODE=Debug"
