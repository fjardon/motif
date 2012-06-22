datadir = ${prefix}/share/Xm/combo2

bin_PROGRAMS = combo
EXTRA_DIST = Combo.ad Imakefile
combo_SOURCES = combo.c
data_DATA = $(EXTRA_DIST) $(combo_SOURCES)
TOPDIR = ../../..
LDADD = $(TOPDIR)/lib/Xm/libXm.la
INCLUDES = -I$(srcdir)/$(TOPDIR)/lib ${X_CFLAGS} 
