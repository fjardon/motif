bindir = ${prefix}/share/Xm/Color
datadir = ${prefix}/share/Xm/Color

bin_PROGRAMS = colordemo
colordemo_SOURCES = bxutil-c.c colordemo.c

EXTRA_DIST = Imakefile

LDADD = $(TOPDIR)/lib/Xm/libXm.la
TOPDIR = ../../..
INCLUDES = -I$(srcdir)/$(TOPDIR)/lib ${X_CFLAGS} 
