datadir = ${prefix}/share/Xm/getsubres

bin_PROGRAMS = getsubres
EXTRA_DIST = getsubres.help getsubres.man README Imakefile
getsubres_SOURCES = getsubres.c
data_DATA = $(EXTRA_DIST) $(getsubres_SOURCES)
LDADD = ../../lib/Xmd/libXmd.a ../../../lib/Xm/libXm.la
INCLUDES = -I$(srcdir)/../../lib -I$(srcdir)/../../../lib ${X_CFLAGS} 
