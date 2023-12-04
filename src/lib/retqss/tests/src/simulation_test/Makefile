BIN_FILE=simulation_test

BUILDDIR=build
TEST_SRC_DIR=../test_common
TEST_CPP_FILES=$(shell find $(TEST_SRC_DIR)/ -name "*.cpp")
TEST_H_FILES=$(shell find $(TEST_SRC_DIR)/ -name "*.h")
TEST_OBJ_FILES=$(addprefix $(BUILDDIR)/, $(notdir $(TEST_CPP_FILES:.cpp=.o)))

MODEL_SRC=helix.c
MODEL_OBJ=helix.o
PKG_OBJ=pkg_retQSS.o

GCC=gcc
CC=g++
RM=rm -rf
CXXFLAGS=-g -w -std=c++11 -include sstream
CFLAGS=-g -w

SRCDIR=../../../..
PKGDIR=$(SRCDIR)/../../packages
INCLUDE_DIR=$(SRCDIR)/usr/include
RETQSS_LIBDIR=$(SRCDIR)/../../lib
GTEST_LIBDIR=../../gtest

GTEST_INCLUDE_PATH=../../gtest/include

RETQSS_INCLUDE=-I$(SRCDIR) -I$(GTEST_INCLUDE_PATH) -I../ -I$(INCLUDE_DIR) -I$(PKGDIR)

INCLUDE=$(RETQSS_INCLUDE)
LIB_PATH= -L$(GTEST_LIBDIR) -L$(RETQSS_LIBDIR) -Wl,-rpath=$(RETQSS_LIBDIR)
LIBS=-lgtest -lm -lgsl -lconfig -lgfortran -lpthread -lmetis -lscotch -lscotcherr -lpatoh -lrt -lsundials_cvode -lsundials_ida -lsundials_nvecserial -lgslcblas -lCGAL -ldl


test_geometry: create_build_dir $(BIN_FILE)
	@./$(BIN_FILE)

create_build_dir:
	mkdir -p $(BUILDDIR)
	
$(BIN_FILE): simulation_test.cpp $(TEST_OBJ_FILES) $(MODEL_OBJ) $(PKG_OBJ)
	$(CC) $(LIB_PATH) $< $(TEST_OBJ_FILES) $(MODEL_OBJ) $(PKG_OBJ) -std=c++11 -o $(BIN_FILE) $(CXXFLAGS) $(INCLUDE) -lqssd -lretqssd $(LIBS)

$(BUILDDIR)/%.o : $(TEST_SRC_DIR)/%.cpp
	$(CC) $(RETQSS_INCLUDE) $(CXXFLAGS) -MM -MT $@ -MF $(patsubst %.o,%.d,$@) $<
	$(CC) $(RETQSS_INCLUDE) -c $< -o $@ $(CXXFLAGS)

$(MODEL_OBJ) : $(MODEL_SRC)
	$(GCC) $(RETQSS_INCLUDE) $< $(CFLAGS) -c -o $@
	
$(PKG_OBJ) : $(PKGDIR)/pkg_retQSS.c
	$(GCC) $(RETQSS_INCLUDE) $< $(CFLAGS) -c -o $@

clean:
	@$(RM) $(BIN_FILE) *.log *.dat retQSS_*
	@$(RM) $(BUILDDIR)
	@find . -name "*.o" -type f -delete 
