MAINTAINERCLEANFILES = Makefile.in

bindir = ${prefix}/share/Xm/ColorSelBox
datadir = ${prefix}/share/Xm/ColorSelBox

bin_PROGRAMS = colorselboxdemo
colorselboxdemo_SOURCES = colorselboxdemo.c
EXTRA_DIST = Imakefile
data_DATA = $(colorselboxdemo_SOURCES) $(EXTRA_DIST)
LDADD = $(TOPDIR)/lib/Xm/libXm.la
TOPDIR = ../../..
INCLUDES = -I$(TOPDIR)/lib -I$(srcdir)/$(TOPDIR)/lib ${X_CFLAGS} 
