datadir = ${prefix}/share/Xm/panner

bin_PROGRAMS = panner
data_DATA = README
panner_SOURCES = panner.c
EXTRA_DIST = README panner.man Imakefile
data_DATA = $(EXTRA_DIST) $(panner_SOURCES)
LDADD = ../../../lib/Xm/libXm.la
INCLUDES = -I$(srcdir)/../../../lib ${X_CFLAGS} 
