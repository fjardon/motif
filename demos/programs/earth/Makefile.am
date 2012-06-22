datadir = ${prefix}/share/Xm/earth

bin_PROGRAMS = earth
earth_SOURCES = earth.c
EXTRA_DIST = terre.xbm earth.man Imakefile README
data_DATA = $(EXTRA_DIST) $(earth_SOURCES)
LDADD = ../../../lib/Xm/libXm.la
INCLUDES = -I$(srcdir)/../../../lib ${X_CFLAGS} 
