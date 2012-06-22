MAINTAINERCLEANFILES = Makefile.in

datadir = ${prefix}/share/Xm/tooltips

EXTRA_DIST = Imakefile
bin_PROGRAMS = tooltips
tooltips_SOURCES = tooltips.c
data_DATA = $(tooltips_SOURCES) $(EXTRA_DIST)
LDADD = ../../../lib/Xm/libXm.la

INCLUDES = -I$(srcdir)/../../../lib ${X_CFLAGS} 
