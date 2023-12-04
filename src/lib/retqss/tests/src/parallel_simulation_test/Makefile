TESTS=basic

TEST_DIRS=$(foreach test, $(TESTS), $(test)/$(test))
TEST_FILES=$(addsuffix .cpp, $(TEST_DIRS))

MODELS=$(addsuffix .mo, $(TEST_DIRS))
MODEL_SRC=$(MODELS:.mo=.c)
MODEL_OBJ=$(MODELS:.mo=.o)

BUILDDIR=build
TEST_SRC_DIR=../test_common
TEST_CPP_FILES=$(shell find $(TEST_SRC_DIR)/ -name "*.cpp")
TEST_H_FILES=$(shell find $(TEST_SRC_DIR)/ -name "*.h")
TEST_OBJ_FILES=$(addprefix $(BUILDDIR)/, $(notdir $(TEST_CPP_FILES:.cpp=.o)))

GARBAGE=$(addsuffix .c, $(TEST_DIRS)) $(addsuffix .ini, $(TEST_DIRS)) $(addsuffix .sh, $(TEST_DIRS)) $(addsuffix .makefile, $(TEST_DIRS)) $(addsuffix .plt, $(TEST_DIRS))

PKG_OBJ=pkg_retQSS.o

MMOC=${MMOC_PATH}/bin/mmoc
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

all: $(TESTS)

$(TESTS) : % : %/test.cpp create_build_dir $(TEST_OBJ_FILES) $(PKG_OBJ)
	$(MMOC) $@/$@.mo -p -i $(PKGDIR) -o $@/$@ -d SD_DBG_Dt
	@sed -i 's/MOD_settings/MOD_settings_old/g' $@/$@.c
	$(GCC) $(RETQSS_INCLUDE) $@/$@.c $(CFLAGS) -c -o $@/$@.o
	$(CC) $(LIB_PATH) $< $(TEST_OBJ_FILES) $@/$@.o $(PKG_OBJ) -std=c++11 -o $@_parallel_test.exe $(CXXFLAGS) $(INCLUDE) -lqssd -lretqssd $(LIBS)
	@cp $@/$@.ini .
	@./$@_parallel_test.exe

create_build_dir:
	@mkdir -p $(BUILDDIR)

create_model_code: $(MODELS)
	$(MMOC) $(MODEL) -p -i $(PKGDIR) -o $< -d SD_DBG_Dt
	@sed -i 's/MOD_settings/MOD_settings_old/g' $<

$(BUILDDIR)/%.o : $(TEST_SRC_DIR)/%.cpp
	$(CC) $(RETQSS_INCLUDE) $(CXXFLAGS) -MM -MT $@ -MF $(patsubst %.o,%.d,$@) $<
	$(CC) $(RETQSS_INCLUDE) -c $< -o $@ $(CXXFLAGS)

$(MODEL_OBJ) : $(MODEL_SRC)
	$(GCC) $(RETQSS_INCLUDE) $< $(CFLAGS) -c -o $@
	
$(PKG_OBJ) : $(PKGDIR)/pkg_retQSS.c
	$(GCC) $(RETQSS_INCLUDE) $< $(CFLAGS) -c -o $@

clean:
	@$(RM) $(BIN_FILE) *.log *.dat retQSS_* *.part *.ini *.exe
	@$(RM) $(BUILDDIR) $(GARBAGE)
	@find . -name "*.o" -type f -delete 
