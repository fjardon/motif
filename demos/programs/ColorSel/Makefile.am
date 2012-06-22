MAINTAINERCLEANFILES = Makefile.in

datadir = ${prefix}/share/Xm/Color

bin_PROGRAMS = colordemo
colordemo_SOURCES = bxutil-c.c colordemo.c
EXTRA_DIST = Imakefile
data_DATA = $(colordemo_SOURCES) $(EXTRA_DIST)
LDADD = $(TOPDIR)/lib/Xm/libXm.la
TOPDIR = ../../..
INCLUDES = -I$(srcdir)/$(TOPDIR)/lib ${X_CFLAGS} 
