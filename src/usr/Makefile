MODE ?= Debug
ROOT ?= ..

# Source Path.
LIBDIR=./lib/
SRCDIR=./src/
INCDIR=./include
ENGINE ?= $(ROOT)/engine

# Source files.
SRC = $(SRCDIR)mmo_math.c $(SRCDIR)mmo_file.c 
SRCCPP = $(SRCDIR)retqss_pic.cc $(SRCDIR)retqss_boids.cc $(SRCDIR)retqss_md.cc $(SRCDIR)retqss_indinf.cc $(SRCDIR)retqss_covid19.cc $(SRCDIR)retqss_test.cc

TARGET = $(LIBDIR)libmmo_math.a $(LIBDIR)libmmo_file.a $(LIBDIR)libpic.a $(LIBDIR)libboids.a $(LIBDIR)libmd.a $(LIBDIR)libindinf.a $(LIBDIR)libcovid19.a $(LIBDIR)libtest.a

RMS = rm -rf

OBJ = $(SRC:.c=.o)
OBJCC = $(SRCCPP:.cc=.o)

CFLAGS = -Wall -I$(INCDIR) -I$(ENGINE)/common -I$(ENGINE) -I$(ROOT)/lib/retqss/3rd-party/cgal/include -I$(ROOT)/lib/retqss -I$(ENGINE)/usr/include -lm -msse2 -mfpmath=sse -O2 -DNDEBUG   

ifeq ($(MODE),Debug)
	CFLAGS += -g   
endif

CC=gcc

.SUFFIXES: .c .cc

default: $(TARGET) 

%.o: %.cc
	$(CXX) -std=c++11 -c $< -o $@ $(CFLAGS)

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

$(TARGET): $(OBJ) $(OBJCC)
	@mkdir -p $(LIBDIR)
	ar rsc $@ $(OBJ) $(OBJCC) 

clean:
	$(RMS) $(OBJ) $(OBJCC) $(TARGET)  

help:
	echo "make MODE=<Debug|Release> ROOT=<source-code-root-dir>"
	echo "Default values:"
	echo ""
	echo "MODE=Debug"
	echo "ROOT=.."
