#!/usr/bin/make -f
# Makefile to the 'iemgui' library for Pure Data.
# Needs Makefile.pdlibbuilder as helper makefile for platform-dependent build
# settings and rules (https://github.com/pure-data/pd-lib-builder).

lib.name = iemgui

## iemlib.h lives in include/
cflags  = -Iinclude

# special file that does not provide a class
lib.setup.sources = src/$(lib.name).c

# all other C and C++ files in subdirs are source files per class
# (alternatively, enumerate them by hand)
# class.sources = $(filter-out $(lib.setup.sources),$(wildcard src/*.c))

class.sources = \
	src/room_sim_2d.c \
	src/room_sim_3d.c \
	src/cube_sphere.c \
	src/sym_dial.c \
	src/iem_image.c \
	src/iem_vu.c \
	src/hfadl_scale.c \
	src/hfadr_scale.c \
	src/vfad_scale.c \
	src/numberbox_matrix.c \
	src/iem_event.c \
	$(empty)

datafiles = \
	$(wildcard *.txt) \
	$(wildcard *.pdf) \
	$(wildcard *.pd) \
	$(wildcard *.gif) \
	$(wildcard *.bat) \
	$(wildcard *.sh) \
	$(wildcard *.wav) \
	$(empty)


## build a multi-object library
make-lib-executable=yes

## suppress "unused" warnings
#suppress-wunused=yes

################################################################################
### pdlibbuilder ###############################################################
################################################################################

# This Makefile is based on the Makefile from pd-lib-builder written by
# Katja Vetter. You can get it from:
# https://github.com/pure-data/pd-lib-builder
PDLIBBUILDER_DIR=pd-lib-builder/
include $(firstword $(wildcard $(PDLIBBUILDER_DIR)/Makefile.pdlibbuilder Makefile.pdlibbuilder))
