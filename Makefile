OS ?= unix
DEBUG ?= False

# Source Path.
libraries=../lib/
source=./src/
include=./include
random=../

# Source files.
SRC = $(source)mmo_math.c $(source)mmo_file.c 

TARGET = $(libraries)libmmo_math.a $(libraries)libmmo_file.a

RMS = rm -rf

OBJ = $(SRC:.c=.o)

CFLAGS = -Wall -I$(include) -lm -msse2 -mfpmath=sse -O2   

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
	ar rsc $@ $(OBJ)  

clean:
	$(RMS) $(OBJ) $(TARGET)  

help:
	echo "make DEBUG=<True|False> SO=<unix|win32|mac>"
	echo "Default values:"
	echo ""
	echo "DEBUG=False"
	echo "OS=unix"
