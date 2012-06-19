bindir = ${prefix}/share/Xm/ButtonBox
datadir = ${prefix}/share/Xm/ButtonBox

bin_PROGRAMS = bboxdemo

EXTRA_DIST = Imakefile

SOURCES = bboxdemo.c
TOPDIR = ../../..
LDADD = $(TOPDIR)/lib/Xm/libXm.la

INCLUDES = -I$(srcdir)/$(TOPDIR)/lib ${X_CFLAGS} 
