MAINTAINERCLEANFILES = Makefile.in

bindir = ${prefix}/share/Xm/xsettings
datadir = ${prefix}/share/Xm/xsettings

EXTRA_DIST = Imakefile
bin_PROGRAMS = xsettings
xsettings_SOURCES = xsettings.c Xsettings.c
noinst_HEADERS = Xsettings.h XsettingsP.h
data_DATA = $(xsettings_SOURCES) $(EXTRA_DIST)
LDADD = ../../../lib/Xm/libXm.la

INCLUDES = -I$(srcdir)/../../../lib -I$(top_builddir)/lib ${X_CFLAGS} 
