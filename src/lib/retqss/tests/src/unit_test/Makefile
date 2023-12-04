GEOMETRY_TEST=geometry_test
PARSER_TEST=parser_test
NEIGHBORHOOD_TEST=neighborhood_test
VOLUME_CROSSING_TEST=volume_crossing_test
PROPERTIES_TEST=properties_test
INTERFACE_TEST=interface_test

TARGETS=$(GEOMETRY_TEST) $(PARSER_TEST) $(NEIGHBORHOOD_TEST) $(VOLUME_CROSSING_TEST) $(PROPERTIES_TEST) $(INTERFACE_TEST)

BUILDDIR=build
TEST_SRC_DIR=../test_common
TEST_CPP_FILES=$(shell find $(TEST_SRC_DIR)/ -name "*.cpp")
TEST_H_FILES=$(shell find $(TEST_SRC_DIR)/ -name "*.h")
TEST_OBJ_FILES=$(addprefix $(BUILDDIR)/, $(notdir $(TEST_CPP_FILES:.cpp=.o)))

GCC=gcc
CC=g++
RM=rm -rf
CFLAGS=-g -w -std=c++11 -include sstream -DQSS_TEST_NO_MODEL -DRETQSS_DEBUG

SRCDIR=../../../..
INCLUDE_DIR=$(SRCDIR)/usr/include
RETQSS_LIBDIR=$(SRCDIR)/../../lib
GTEST_LIBDIR=../../gtest

GTEST_INCLUDE_PATH=../../gtest/include

RETQSS_INCLUDE=-I$(SRCDIR) -I$(GTEST_INCLUDE_PATH) -I../ -I$(INCLUDE_DIR)

INCLUDE=$(RETQSS_INCLUDE)
LIB_PATH= -L$(GTEST_LIBDIR) -L$(RETQSS_LIBDIR) -Wl,-rpath=$(RETQSS_LIBDIR)
LIBS=-rdynamic -lgtest -lm -lgsl -lconfig -lgfortran -lpthread -lmetis -lscotch -lscotcherr -lpatoh -lrt -lsundials_cvode -lsundials_ida -lsundials_nvecserial -lgslcblas -lCGAL -ldl


all: $(TARGETS)

create_build_dir:
	mkdir -p $(BUILDDIR)

$(BUILDDIR)/%.o : $(TEST_SRC_DIR)/%.cpp
	$(CC) $(RETQSS_INCLUDE) $(CFLAGS) -MM -MT $@ -MF $(patsubst %.o,%.d,$@) $<
	$(CC) $(RETQSS_INCLUDE) -c $< -o $@ $(CFLAGS)

$(TARGETS) : % : %.cpp create_build_dir $(TEST_OBJ_FILES)
	$(CC) $(LIB_PATH) -lqssd -lretqssd $< $(TEST_OBJ_FILES) -std=c++11 -o $@ $(CFLAGS) $(INCLUDE) $(LIBS)
	@./$@

clean:
	@$(RM) $(TARGETS) *.log retQSS_*
	@$(RM) $(BUILDDIR)
	@find . -name "*.o" -type f -delete 
