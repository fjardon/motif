bindir = ${prefix}/share/Xm/draw
datadir = ${prefix}/share/Xm/draw

bin_PROGRAMS = draw

EXTRA_DIST = Imakefile

SOURCES = draw.c
LDADD = ../../lib/Xmd/libXmd.a ../../../lib/Xm/libXm.la
INCLUDES = -I$(srcdir)/../../lib -I$(srcdir)/../../../lib
