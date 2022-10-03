#!/usr/bin/make -f
# Makefile to the 'iemlib' library for Pure Data.
# Needs Makefile.pdlibbuilder as helper makefile for platform-dependent build
# settings and rules (https://github.com/pure-data/pd-lib-builder).

# library name, used when compiing as multi-object library
# and as the install directory
lib.name = iemlib

## iemlib.h lives in include/
cflags  = -Iinclude

# special file that does not provide a class
lib.setup.sources = src/$(lib.name).c

# source files
class.sources = \
	src/add2_comma.c \
	src/aspeedlim.c \
	src/biquad_freq_resp.c \
	src/bpe.c \
	src/db2v.c \
	src/dollarg.c \
	src/exp_inc.c \
	src/f2note.c \
	src/fade~.c \
	src/filter~.c \
	src/FIR~.c \
	src/float24.c \
	src/for++.c \
	src/gate.c \
	src/hml_shelf~.c \
	src/iem_alisttosym.c \
	src/iem_anything.c \
	src/iem_append.c \
	src/iem_blocksize~.c \
	src/iem_cot4~.c \
	src/iem_delay~.c \
	src/iem_i_route.c \
	src/iem_pbank_csv.c \
	src/iem_pow4~.c \
	src/iem_prepend.c \
	src/iem_receive.c \
	src/iem_route.c \
	src/iem_samplerate~.c \
	src/iem_sel_any.c \
	src/iem_send.c \
	src/iem_sqrt4~.c \
	src/iem_symtoalist.c \
	src/init.c \
	src/LFO_noise~.c \
	src/list2send.c \
	src/lp1_t~.c \
	src/m2f~.c \
	src/mergefilename.c \
	src/modulo_counter.c \
	src/mov_avrg_kern~.c \
	src/para_bp2~.c \
	src/parentdollarzero.c \
	src/peakenv~.c \
	src/peakenv_AR~.c \
	src/peakenv_hold~.c \
	src/post_netreceive.c \
	src/pre_inlet.c \
	src/prepend_ascii.c \
	src/protect_against_open.c \
	src/prvu~.c \
	src/pvu~.c \
	src/receive2list.c \
	src/round_zero.c \
	src/rvu~.c \
	src/sin_freq~.c \
	src/sin_phase~.c \
	src/soundfile_info.c \
	src/sparse_FIR~.c \
	src/speedlim.c \
	src/split.c \
	src/splitfilename.c \
	src/stripfilename.c \
	src/t3_bpe.c \
	src/t3_delay.c \
	src/t3_line~.c \
	src/t3_metro.c \
	src/t3_sig~.c \
	src/t3_timer.c \
	src/toggle_mess.c \
	src/transf_fader.c \
	src/unsymbol.c \
	src/v2db.c \
	src/vcf_filter~.c \
	src/wrap.c \
	$(empty)

# install additional files which will be installed into "iemlib/"
# files are *not* searched recursively
datafiles = \
	$(wildcard *.txt) \
	$(wildcard *.pdf) \
	$(wildcard *.pd) \
	$(wildcard *.gif) \
	$(wildcard *.bat) \
	$(wildcard *.sh) \
	$(wildcard *.wav) \
	$(empty)
	

# even more datafiles (from subfolders) but will be installed into "iemlib/" as well
datafiles += \
	$(wildcard alias/*.pd) \
	$(wildcard abs/*.pd) \
	$(wildcard help/*.*) \
	$(empty)

# install additional directories which will be installed into "iemlib/"
datadirs = examples \
	$(empty)


## these days, we have g_canvas.h an most systems
## if not, call make with 'have_g_canvas='
have_g_canvas = -DHAVE_G_CANVAS_H=1
cflags += $(have_g_canvas)

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
