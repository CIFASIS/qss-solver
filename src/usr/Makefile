MODE ?= Debug
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

ifeq ($(MODE),Debug)
	CFLAGS += -g   
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
	echo "make MODE=<Debug|Release> ENGINE=<qss-engine-root-dir>"
	echo "Default values:"
	echo ""
	echo "MODE=Debug"
	echo "ENGINE=."
