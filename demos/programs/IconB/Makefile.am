datadir = ${prefix}/share/Xm/Icon

EXTRA_DIST = Imakefile small.bm
bin_PROGRAMS = iconbuttondemo
iconbuttondemo_SOURCES = misc.c  iconbuttondemo.c
data_DATA = $(iconbuttondemo_SOURCES) $(EXTRA_DIST)
TOPDIR = ../../..
LDADD = $(TOPDIR)/lib/Xm/libXm.la

INCLUDES = -I.. \
           -I$(srcdir)/$(TOPDIR)/lib \
           ${X_CFLAGS} 
