#Target OS variables
OS ?= unix
DEBUG ?= False

#Compiler and Linker
CC          := gcc
FC 			:= gfortran 

#The Target Binary Program
TARGET      := libqss.a
LIBTIMESTEP := libtimestep.a
ifeq ($(DEBUG),True)
TARGET      := libqssd.a
LIBTIMESTEP := libtimestepd.a
endif

#The Directories, Source, Includes, Objects, Binary 
SRCDIR      := ../../src
ENGINEDIR 	:= ../engine
QSSDIR 		:= $(ENGINEDIR)/qss
METHODSDIR 	:= $(QSSDIR)
CLASSICDIR 	:= $(ENGINEDIR)/classic
DASSLDIR 	:= $(ENGINEDIR)/classic/dassl
DOPRIDIR 	:= $(ENGINEDIR)/classic/dopri5
COMMONDIR 	:= $(ENGINEDIR)/common
OBJDIR      := obj
BUILDDIR    := $(ENGINEDIR)/$(OBJDIR)/release
ifeq ($(DEBUG),True)
BUILDDIR    := $(ENGINEDIR)/$(OBJDIR)/debug
endif
GRAPHDIR    := $(SRCDIR)/graph
TARGETDIR   := ./
SRCEXT      := c
DEPEXT      := d
OBJEXT      := o

#Flags, Libraries and Includes
CFLAGS 		:= -Wall -msse2 -mfpmath=sse -O2 
ifeq ($(DEBUG),True)
CFLAGS 		:= -Wall -msse2 -mfpmath=sse -g -DDEBUG  
endif
LIB         := -lm 
INC         := -I$(COMMONDIR) -I$(ENGINEDIR)
ifeq ($(OS),win32)
INC         += -I/GnuWin32/include
endif
RMS 		:= rm -rf
vpath %.c $(ENGINEDIR)
.SUFFIXES: .c

# Source files.
COMMONSRC = $(COMMONDIR)/data.c $(COMMONDIR)/utils.c $(COMMONDIR)/tree.c $(COMMONDIR)/settings.c $(COMMONDIR)/random.c $(COMMONDIR)/simulator.c $(COMMONDIR)/integrator.c $(COMMONDIR)/engine.c $(COMMONDIR)/commands.c 

SEQSRC = $(QSSDIR)/qss_graph.c $(QSSDIR)/qss_biorica.c $(QSSDIR)/qss_data.c $(QSSDIR)/qss_file.c $(QSSDIR)/qss_memory.c $(QSSDIR)/qss_log.c $(QSSDIR)/qss_sampled.c $(QSSDIR)/qss_step.c $(QSSDIR)/qss_binary_random.c $(QSSDIR)/qss_binary.c $(QSSDIR)/qss_linear.c $(QSSDIR)/qss_frw.c  $(QSSDIR)/qss_frw_imp.c $(METHODSDIR)/qss.c $(METHODSDIR)/qss2.c $(METHODSDIR)/qss3.c $(METHODSDIR)/qss4.c $(METHODSDIR)/liqss.c $(METHODSDIR)/liqss2.c $(METHODSDIR)/liqss3.c $(METHODSDIR)/cqss.c $(QSSDIR)/qss_output.c $(QSSDIR)/qss_scheduler.c $(QSSDIR)/qss_quantizer.c $(QSSDIR)/qss_integrator.c $(QSSDIR)/qss_seq_integrator.c  $(QSSDIR)/qss_simulator.c $(QSSDIR)/qss_commands.c

PARSRC = $(QSSDIR)/qss_file.c $(QSSDIR)/qss_memory.c $(QSSDIR)/qss_sampled.c $(QSSDIR)/qss_step.c $(QSSDIR)/qss_binary.c $(QSSDIR)/qss_linear.c $(QSSDIR)/qss_frw_imp.c $(METHODSDIR)/qss.c $(METHODSDIR)/cqss.c $(METHODSDIR)/qss2.c $(METHODSDIR)/qss3.c $(METHODSDIR)/qss4.c $(METHODSDIR)/liqss.c $(METHODSDIR)/liqss2.c $(METHODSDIR)/liqss3.c $(METHODSDIR)/cqss.c $(QSSDIR)/qss_dt.c $(QSSDIR)/qss_lp.c $(QSSDIR)/qss_partition.c  $(QSSDIR)/qss_parallel.c $(QSSDIR)/qss_sim_steps.c $(QSSDIR)/qss_par_integrator.c  

CLASSICSRC = $(CLASSICDIR)/classic_data.c $(CLASSICDIR)/classic_integrator.c $(CLASSICDIR)/classic_dopri_integrator.c $(CLASSICDIR)/classic_dassl_integrator.c $(CLASSICDIR)/classic_simulator.c $(CLASSICDIR)/classic_commands.c

DASSLSRC = $(DASSLDIR)/daux.f $(DASSLDIR)/ddaskr.f $(DASSLDIR)/dlinpk.f

DOPRISRC = $(DOPRIDIR)/dopri5.c 

GRAPHSRC = $(GRAPHDIR)/graph_profile.c

# Objects
COMMONOBJ=$(addprefix $(BUILDDIR)/, $(notdir $(COMMONSRC:.c=.o)))

SEQOBJ=$(addprefix $(BUILDDIR)/seq_, $(notdir $(SEQSRC:.c=.o)))

PAROBJ=$(addprefix $(BUILDDIR)/par_, $(notdir $(PARSRC:.c=.o)))

CLASSICOBJ=$(addprefix $(BUILDDIR)/, $(notdir $(CLASSICSRC:.c=.o)))

DASSLOBJ=$(addprefix $(BUILDDIR)/, $(notdir $(DASSLSRC:.f=.o)))

DOPRIOBJ=$(addprefix $(BUILDDIR)/, $(notdir $(DOPRISRC:.c=.o)))

GRAPHOBJ=$(addprefix $(GRAPHDIR)/$(OBJDIR)/grp_, $(notdir $(GRAPHSRC:.c=.o)))

# Make dependencies
DEPS = $(COMMONOBJ:.o=.d) $(SEQOBJ:.o=.d) $(PAROBJ:.o=.d) $(CLASSICOBJ:.o=.d)

default: $(TARGET) $(LIBTIMESTEP) 

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

$(TARGET): $(COMMONOBJ) $(SEQOBJ) $(PAROBJ) $(CLASSICOBJ) $(GRAPHOBJ)
	@ar rsc $@ $(COMMONOBJ) $(SEQOBJ) $(PAROBJ) $(CLASSICOBJ) $(GRAPHOBJ)

$(COMMONOBJ): | $(BUILDDIR)

$(SEQOBJ): | $(BUILDDIR)

$(PAROBJ): | $(BUILDDIR)

$(CLASSICOBJ): | $(BUILDDIR)

$(GRAPHOBJ): 
	@cd $(GRAPHDIR) && $(MAKE)

$(BUILDDIR):
	@mkdir -p $(BUILDDIR)
	
-include $(DEPS)

.PHONY: clean

clean:
	$(RMS) $(DEPS) $(TARGET) $(COMMONOBJ) $(SEQOBJ) $(PAROBJ) $(CLASSICOBJ) $(DASSLOBJ) $(DOPRIOBJ)

help:
	@echo "make DEBUG=<True|False> OS=<unix|win32|mac>"
	@echo "Default value:"
	@echo ""
	@echo "DEBUG=False"
	@echo "OS=unix"
