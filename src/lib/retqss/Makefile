# Target variables
MODE ?= Debug

# Build arch.
ARCH = $(shell uname -m)
DISTRO := $(shell lsb_release -r 2>/dev/null | grep Release | awk '{ print $$2 }')

# Compiler and Linker
CXX := g++

# The Directories, Source, Includes, Objects, Binary
SRC_DIR 	 := .
USR_DIR 		:= $(SRC_DIR)/usr
BUILD_DIR    	:= $(USR_DIR)/obj/release
ifeq ($(MODE),Debug)
BUILD_DIR    	:= $(USR_DIR)/obj/debug
endif
LIB_DIR   		:= $(USR_DIR)/lib
INCLUDE_DIR 	:= $(USR_DIR)/include
RET_QSS_DIR  := $(SRC_DIR)
3RD_PARTY_DIR  := $(SRC_DIR)/3rd-party
CGAL_DIR     := $(3RD_PARTY_DIR)/cgal
QSS_DIR := ../../engine


# Libraries
LIB_RET_QSS   := $(LIB_DIR)/libretqss.a
ifeq ($(MODE),Debug)
LIB_RET_QSS   := $(LIB_DIR)/libretqssd.a
endif

# Flags, Libraries and Includes
CFLAGS 		:= -Wall -msse2 -mfpmath=sse -O2 -DNDEBUG
ifeq ($(MODE),Debug)
CFLAGS 		:= -Wall -msse2 -mfpmath=sse -g -DDEBUG
endif
ifeq ($(RETQSS_DEBUG),True)
CFLAGS      := $(CFLAGS) -DRETQSS_DEBUG
endif
LIB         := -lm
INC         := -I$(COMMONDIR) -I$(SRCDIR) -I$(INCDIR)
RET_QSS_INC   := -I$(CGALDIR)/include
RMS 		:= rm -rf
vpath %.c $(SRCDIR)
.SUFFIXES: .c

all: lib-ret-qss

include $(3RD_PARTY_DIR)/Makefile.include

# Source files.
RET_QSS_CXX_SRC = $(RET_QSS_DIR)/retqss_geometry.cc \
                  $(RET_QSS_DIR)/retqss_particle.cc \
                  $(RET_QSS_DIR)/retqss_particle_tracker.cc \
                  $(RET_QSS_DIR)/retqss_interface.cc \
                  $(RET_QSS_DIR)/retqss_simulator_interface.cc \
                  $(RET_QSS_DIR)/retqss_qss_solver_interface.cc \
                  $(RET_QSS_DIR)/retqss_parser.cc \
                  $(RET_QSS_DIR)/retqss_volume_neighborhood.cc \
                  $(RET_QSS_DIR)/retqss_volume_neighborhood_impl.cc \
                  $(RET_QSS_DIR)/retqss_volume_neighbor.cc \
                  $(RET_QSS_DIR)/retqss_face_neighborhood.cc \
                  $(RET_QSS_DIR)/retqss_particle_neighborhood.cc \
                  $(RET_QSS_DIR)/retqss_particle_neighbor.cc \
                  $(RET_QSS_DIR)/retqss_polyhedron.cc \
                  $(RET_QSS_DIR)/retqss_volume.cc \
                  $(RET_QSS_DIR)/retqss_polyhedral_volume.cc \
                  $(RET_QSS_DIR)/retqss_open_volume.cc \
                  $(RET_QSS_DIR)/retqss_logical_face.cc \
                  $(RET_QSS_DIR)/retqss_polyhedron_face.cc \
                  $(RET_QSS_DIR)/retqss_aabb_tree.cc \
                  $(RET_QSS_DIR)/retqss_trajectory.cc \
                  $(RET_QSS_DIR)/retqss_intersection_locator.cc \
                  $(RET_QSS_DIR)/retqss_face_crossing_algorithm.cc \
                  $(RET_QSS_DIR)/retqss_utilities.cc \
                  $(RET_QSS_DIR)/retqss_model_api.cc \
                  $(RET_QSS_DIR)/retqss_exceptions.cc \
                  $(RET_QSS_DIR)/retqss_stats.cc \
                  $(RET_QSS_DIR)/retqss_debug.cc

# Make dependencies
DEPS = $(RET_QSS_CXX_OBJ:.o=.d)

# Objects
$(BUILD_DIR)/%.o: %.cc
	$(CXX) -I$(INCLUDE_DIR) -I$(QSS_DIR) $(CFLAGS) -MM -MT $@ -MF $(patsubst %.o,%.d,$@) $<
	$(CXX) -I$(INCLUDE_DIR) -I$(QSS_DIR) $(CFLAGS) -c $< -o $@ 

RET_QSS_CXX_OBJ=$(addprefix $(BUILD_DIR)/, $(RET_QSS_CXX_SRC:.cc=.o))

$(RET_QSS_CXX_OBJ): | create-folders 

$(LIB_RET_QSS): $(RET_QSS_CXX_OBJ)
	@ar rsc $@ $(RET_QSS_CXX_OBJ)

create-folders::
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(LIB_DIR)

# Rules

lib-ret-qss: cgal $(LIB_RET_QSS)

doc:
	sphinx-build -b html -Dbreathe_projects.retQSS=$(MMOC_ENGINE)/$(USRDIR)/doc/retqss/xml $(RET_QSS_DIR)/doc $(USRDIR)/doc/retqss/sphinx

-include $(DEPS)

.PHONY: clean

clean:
	$(RMS) $(USR_DIR)

help:
	@echo "make DEBUG=<True|False>"
	@echo "Default value:"
	@echo ""
	@echo "DEBUG=False"
