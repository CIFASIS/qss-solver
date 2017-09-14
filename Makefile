OS = $(uname)
DEBUG ?= False
ENGINE ?= .

# Source Path.
LIBDIR=./lib/
SRCDIR=./src/
INCDIR=./include

# Source files.
SRC = $(SRCDIR)mmo_math.c $(SRCDIR)mmo_file.c 

TARGET = $(LIBDIR)libmmo_math.a $(LIBDIR)libmmo_file.a

RMS = rm -rf

OBJ = $(SRC:.c=.o)

CFLAGS = -Wall -I$(INCDIR) -I$(ENGINE)/common -lm -msse2 -mfpmath=sse -O2   

ifeq ($(DEBUG),True)
	CFLAGS += -g   
endif

ifeq ($(OS),win32)
	CFLAGS += -I/GnuWin32/include   
endif

CC=gcc

.SUFFIXES: .c

default: $(TARGET) 

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

$(TARGET): $(OBJ)
	@mkdir -p $(LIBDIR)
	ar rsc $@ $(OBJ)  

clean:
	$(RMS) $(OBJ) $(TARGET)  

help:
	echo "make DEBUG=<True|False> ENGINE=<qss-engine-root-dir>"
	echo "Default values:"
	echo ""
	echo "OS=unix"
	echo "ENGINE=."
