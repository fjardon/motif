bindir = ${prefix}/share/Xm/panner
datadir = ${prefix}/share/Xm/panner

bin_PROGRAMS = panner
data_DATA = README
panner_SOURCES = panner.c
man_MANS = panner.man

EXTRA_DIST = README panner.man
LDADD = ../../../lib/Xm/libXm.la
INCLUDES = -I$(srcdir)/../../../lib ${X_CFLAGS} 
