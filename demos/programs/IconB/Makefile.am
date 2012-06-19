bindir = ${prefix}/share/Xm/Icon
datadir = ${prefix}/share/Xm/Icon

EXTRA_DIST = Imakefile

data_DATA = small.bm

bin_PROGRAMS = iconbuttondemo
iconbuttondemo_SOURCES = bxutil-c.c  iconbuttondemo.c small.bm

TOPDIR = ../../..
LDADD = $(TOPDIR)/lib/Xm/libXm.la

INCLUDES = -I.. \
           -I$(srcdir)/$(TOPDIR)/lib \
           ${X_CFLAGS} 
