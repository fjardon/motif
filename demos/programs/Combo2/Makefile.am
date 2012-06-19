bindir = ${prefix}/share/Xm/combo2
datadir = ${prefix}/share/Xm/combo2

bin_PROGRAMS = combo
data_DATA = Combo.ad

EXTRA_DIST = Combo.ad
SOURCES = combo.c
TOPDIR = ../../..
LDADD = $(TOPDIR)/lib/Xm/libXm.la

INCLUDES = -I$(srcdir)/$(TOPDIR)/lib ${X_CFLAGS} 
