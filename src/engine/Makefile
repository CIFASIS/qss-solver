# Target variables
MODE ?= Debug
ARCH= $(shell uname -m)
ifeq ($(ARCH), i686)
ARCH= i386
endif

# Compiler and Linker
CC          := gcc
FC 			:= gfortran

# The Directories, Source, Includes, Objects, Binary
SRCDIR 		:= .
QSSDIR 		:= $(SRCDIR)/qss
METHODSDIR 	:= $(QSSDIR)
CLASSICDIR 	:= $(SRCDIR)/classic
DASSLDIR 	:= $(SRCDIR)/classic/dassl
DOPRIDIR 	:= $(SRCDIR)/classic/dopri5
COMMONDIR 	:= $(SRCDIR)/common
USRDIR 		:= $(SRCDIR)/usr
PWD= $(shell pwd)
3RDPARTYDIR	:= $(SRCDIR)/3rd-party
BUILDDIR    := $(USRDIR)/obj/release
ifeq ($(MODE),Debug)
BUILDDIR    := $(USRDIR)/obj/debug
endif
LIBDIR   	:= $(USRDIR)/lib
INCDIR   	:= $(USRDIR)/include
KLULIBDIR 	:= /usr/lib/$(ARCH)-linux-gnu/
KLUINCDIR 	:= /usr/include/suitesparse/
SRCEXT      := c
DEPEXT      := d
OBJEXT      := o

# Libraries
LIBQSS      := $(LIBDIR)/libqss.a
LIBTIMESTEP := $(LIBDIR)/libtimestep.a
LIBCVODE    := $(LIBDIR)/libsundials_cvode.a
LIBIDA      := $(LIBDIR)/libsundials_ida.a
LIBGSL      := $(LIBDIR)/libgsl.a
ifeq ($(MODE),Debug)
LIBQSS      := $(LIBDIR)/libqssd.a
LIBTIMESTEP := $(LIBDIR)/libtimestepd.a
endif

# Flags, Libraries and Includes
ARCH 		:= $(shell uname -m)
CFLAGS 		:= -Wall -msse2 -mfpmath=sse -O2
ifeq ($(MODE),Debug)
CFLAGS 		:= -Wall -msse2 -mfpmath=sse -g -DDEBUG
endif
LIB         := -lm
INC         := -I$(COMMONDIR) -I$(SRCDIR) -I$(INCDIR)
RM  		:= rm -rf

vpath %.c $(SRCDIR)
.SUFFIXES: .c

# Source files.
COMMONSRC = $(COMMONDIR)/data.c \
						$(COMMONDIR)/utils.c \
						$(COMMONDIR)/tree.c \
						$(COMMONDIR)/settings.c \
						$(COMMONDIR)/random.c \
						$(COMMONDIR)/simulator.c \
						$(COMMONDIR)/integrator.c \
						$(COMMONDIR)/engine.c \
						$(COMMONDIR)/commands.c

SEQSRC = $(QSSDIR)/qss_graph.c \
				 $(QSSDIR)/qss_biorica.c \
				 $(QSSDIR)/qss_data.c \
				 $(QSSDIR)/qss_file.c \
				 $(QSSDIR)/qss_memory.c \
				 $(QSSDIR)/qss_log.c \
				 $(QSSDIR)/qss_sampled.c \
				 $(QSSDIR)/qss_step.c \
				 $(QSSDIR)/qss_binary_random.c \
				 $(QSSDIR)/qss_binary.c \
				 $(QSSDIR)/qss_linear.c \
				 $(QSSDIR)/qss_frw.c  \
				 $(QSSDIR)/qss_frw_imp.c \
				 $(METHODSDIR)/qss.c \
				 $(METHODSDIR)/qss2.c \
				 $(METHODSDIR)/qss3.c \
				 $(METHODSDIR)/qss4.c \
				 $(METHODSDIR)/liqss.c \
				 $(METHODSDIR)/liqss2.c \
				 $(METHODSDIR)/liqss_bdf.c \
				 $(METHODSDIR)/liqss3.c \
				 $(METHODSDIR)/cqss.c \
				 $(QSSDIR)/qss_bdf.c \
				 $(QSSDIR)/qss_output.c \
				 $(QSSDIR)/qss_scheduler.c \
				 $(QSSDIR)/qss_quantizer.c \
				 $(QSSDIR)/qss_integrator.c \
				 $(QSSDIR)/qss_seq_integrator.c  \
				 $(QSSDIR)/qss_hyb_integrator.c  \
				 $(QSSDIR)/qss_simulator.c \
				 $(QSSDIR)/qss_commands.c

PARSRC = $(QSSDIR)/qss_file.c \
				 $(QSSDIR)/qss_memory.c \
				 $(QSSDIR)/qss_sampled.c \
				 $(QSSDIR)/qss_step.c \
				 $(QSSDIR)/qss_binary.c \
				 $(QSSDIR)/qss_linear.c \
				 $(QSSDIR)/qss_frw_imp.c \
				 $(METHODSDIR)/qss.c \
				 $(METHODSDIR)/cqss.c \
				 $(METHODSDIR)/qss2.c \
				 $(METHODSDIR)/qss3.c \
				 $(METHODSDIR)/qss4.c \
				 $(METHODSDIR)/liqss.c \
				 $(METHODSDIR)/liqss2.c \
				 $(METHODSDIR)/liqss_bdf.c \
				 $(METHODSDIR)/liqss3.c \
				 $(METHODSDIR)/cqss.c \
				 $(QSSDIR)/qss_dt.c \
				 $(QSSDIR)/qss_lp.c \
				 $(QSSDIR)/qss_partition.c  \
				 $(QSSDIR)/qss_parallel.c \
				 $(QSSDIR)/qss_sim_steps.c \
				 $(QSSDIR)/qss_par_integrator.c

CLASSICSRC = $(CLASSICDIR)/classic_data.c \
						 $(CLASSICDIR)/classic_integrator.c \
						 $(CLASSICDIR)/classic_cvode_integrator.c \
						 $(CLASSICDIR)/classic_ida_integrator.c \
						 $(CLASSICDIR)/classic_dopri_integrator.c \
						 $(CLASSICDIR)/classic_dassl_integrator.c \
						 $(CLASSICDIR)/classic_simulator.c \
						 $(CLASSICDIR)/classic_commands.c

DASSLSRC = $(DASSLDIR)/daux.f \
					 $(DASSLDIR)/ddaskr.f \
					 $(DASSLDIR)/dlinpk.f

DOPRISRC = $(DOPRIDIR)/dopri5.c

# Objects
COMMONOBJ=$(addprefix $(BUILDDIR)/, $(notdir $(COMMONSRC:.c=.o)))

SEQOBJ=$(addprefix $(BUILDDIR)/seq_, $(notdir $(SEQSRC:.c=.o)))

PAROBJ=$(addprefix $(BUILDDIR)/par_, $(notdir $(PARSRC:.c=.o)))

CLASSICOBJ=$(addprefix $(BUILDDIR)/, $(notdir $(CLASSICSRC:.c=.o)))

DASSLOBJ=$(addprefix $(BUILDDIR)/, $(notdir $(DASSLSRC:.f=.o)))

DOPRIOBJ=$(addprefix $(BUILDDIR)/, $(notdir $(DOPRISRC:.c=.o)))

# Make dependencies
DEPS = $(COMMONOBJ:.o=.d) $(SEQOBJ:.o=.d) $(PAROBJ:.o=.d) $(CLASSICOBJ:.o=.d)

default: compile-all

$(LIBCVODE): | $(BUILDDIR)
	tar xvzf $(3RDPARTYDIR)/cvode/cvode-2.9.0.tar.gz
	mkdir -p ./cvode-2.9.0/build
	cd ./cvode-2.9.0/build; cmake .. -DLAPACK_ENABLE=ON -DCMAKE_BUILD_TYPE=Release  -DKLU_ENABLE=ON -DKLU_INCLUDE_DIR=$(KLUINCDIR) -DKLU_LIBRARY_DIR=$(KLULIBDIR)  -DCMAKE_INSTALL_PREFIX=/usr
	make -C ./cvode-2.9.0/build install DESTDIR=`pwd`
	rm -rf ./cvode-2.9.0
	rm -rf ./examples

$(LIBGSL): | $(BUILDDIR)
	tar xvzf $(3RDPARTYDIR)/gsl/gsl-2.5.tar.gz
	cd ./gsl-2.5/; ./configure --prefix=$(PWD)/usr; make; make install 
	rm -rf ./gsl-2.5

$(LIBIDA): | $(BUILDDIR)
	tar xvzf $(3RDPARTYDIR)/ida/ida-2.9.0.tar.gz
	mkdir -p ./ida-2.9.0/build
	cd ./ida-2.9.0/build; cmake .. -DLAPACK_ENABLE=ON -DCMAKE_BUILD_TYPE=Release  -DKLU_ENABLE=ON -DKLU_INCLUDE_DIR=$(KLUINCDIR) -DKLU_LIBRARY_DIR=$(KLULIBDIR) -DCMAKE_INSTALL_PREFIX=/usr
	make -C ./ida-2.9.0/build install DESTDIR=`pwd`
	rm -rf ./ida-2.9.0
	rm -rf ./examples

$(BUILDDIR)/%.o : $(COMMONDIR)/%.c
	$(CC) $(INC) $(CFLAGS) -MM -MT $@ -MF $(patsubst %.o,%.d,$@) $<
	$(CC) $(INC) -c $< -o $@ $(CFLAGS)

$(BUILDDIR)/seq_%.o : $(QSSDIR)/%.c
	$(CC) $(INC) $(CFLAGS) -MM -MT $@ -MF $(patsubst %.o,%.d,$@) $<
	$(CC) $(INC) -c $< -o $@ $(CFLAGS)

$(BUILDDIR)/par_%.o : $(QSSDIR)/%.c
	$(CC) $(INC) $(CFLAGS) -MM -MT $@ -MF $(patsubst %.o,%.d,$@) $<
	$(CC) $(INC) -c $< -o $@ $(CFLAGS) -DQSS_PARALLEL

$(BUILDDIR)/%.o : $(CLASSICDIR)/%.c
	$(CC) $(INC) $(CFLAGS) -MM -MT $@ -MF $(patsubst %.o,%.d,$@) $<
	$(CC) $(INC) -c $< -o $@ $(CFLAGS)

$(BUILDDIR)/%.o : $(DOPRIDIR)/%.c
	$(CC) $(INC) $(CFLAGS) -MM -MT $@ -MF $(patsubst %.o,%.d,$@) $<
	$(CC) $(INC) -c $< -o $@ $(CFLAGS)

$(BUILDDIR)/%.o : $(DASSLDIR)/%.f
	$(FC) $(INC) -c $< -o $@ $(CFLAGS)

$(LIBTIMESTEP): $(DASSLOBJ) $(DOPRIOBJ)
	@ar rvs $(@) $(DASSLOBJ) $(DOPRIOBJ)

$(LIBQSS): $(COMMONOBJ) $(SEQOBJ) $(PAROBJ) $(CLASSICOBJ)
	@echo $ARCH
	@ar rsc $@ $(COMMONOBJ) $(SEQOBJ) $(PAROBJ) $(CLASSICOBJ)
	$(RM) $(LIBDIR)/*.so*

$(COMMONOBJ): | $(BUILDDIR)

$(SEQOBJ): | $(BUILDDIR)

$(PAROBJ): | $(BUILDDIR)

$(CLASSICOBJ): | $(BUILDDIR)

$(BUILDDIR):
	@mkdir -p $(BUILDDIR)
	@mkdir -p $(LIBDIR)

# Rules
	
compile-all: $(LIBCVODE) $(LIBIDA) $(LIBGSL) $(LIBQSS) $(LIBTIMESTEP)

cvode: $(LIBCVODE)

ida: $(LIBIDA)

gsl: $(LIBGSL)

qss: $(LIBQSS)

time-step: $(LIBTIMESTEP)

test:
	@echo Compile and Run tests.
	@cd tests && $(MAKE) 
	@echo Done

doc:
	@mkdir -p $(USRDIR)/doc
	@mkdir -p $(USRDIR)/doc/html
	doxygen QSSSolver.doxyfile

-include $(DEPS)

.PHONY: clean

clean:
	$(RM) $(DEPS) $(TARGET) $(COMMONOBJ) $(SEQOBJ) $(PAROBJ) $(CLASSICOBJ) $(DASSLOBJ) $(DOPRIOBJ) 

help:
	@echo "make MODE=<Debug|Release>"
	@echo "Default value:"
	@echo ""
	@echo "MODE=Debug"
