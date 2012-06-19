bindir = ${prefix}/share/Xm/getsubres
datadir = ${prefix}/share/Xm/getsubres

bin_PROGRAMS = getsubres
data_DATA = README getsubres.help
man_MANS = getsubres.man

EXTRA_DIST = getsubres.help getsubres.man README Imakefile
SOURCES = getsubres.c
LDADD = ../../lib/Xmd/libXmd.a ../../../lib/Xm/libXm.la
INCLUDES = -I$(srcdir)/../../lib -I$(srcdir)/../../../lib ${X_CFLAGS} 
