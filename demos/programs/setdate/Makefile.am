MAINTAINERCLEANFILES = Makefile.in

datadir = ${prefix}/share/Xm/setDate

bin_PROGRAMS = setDate
EXTRA_DIST = XmdSetDate.ad README setDate.man Imakefile
setDate_SOURCES = setDate.c
data_DATA = $(EXTRA_DIST) $(setDate_SOURCES)
LDADD = ../../../lib/Xm/libXm.la

INCLUDES = -I$(srcdir)/../../../lib ${X_CFLAGS} 
