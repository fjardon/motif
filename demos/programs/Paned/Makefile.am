datadir = ${prefix}/share/Xm/Paned2

EXTRA_DIST = Imakefile
bin_PROGRAMS = paned
paned_SOURCES = creation-c.c paned.c
data_DATA = $(EXTRA_DIST) $(paned_SOURCES)
TOPDIR = ../../..
LDADD = $(TOPDIR)/lib/Xm/libXm.la

INCLUDES = -I$(srcdir)/$(TOPDIR)/lib ${X_CFLAGS} 
