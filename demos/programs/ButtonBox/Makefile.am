MAINTAINERCLEANFILES = Makefile.in

datadir = ${prefix}/share/Xm/ButtonBox

bin_PROGRAMS = bboxdemo
EXTRA_DIST = Imakefile
bboxdemo_SOURCES = bboxdemo.c
data_DATA = $(EXTRA_DIST) $(bboxdemo_SOURCES)
TOPDIR = ../../..
LDADD = $(TOPDIR)/lib/Xm/libXm.la

INCLUDES = -I$(srcdir)/$(TOPDIR)/lib ${X_CFLAGS} 
