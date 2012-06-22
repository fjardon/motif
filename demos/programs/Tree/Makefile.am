datadir = ${prefix}/share/Xm/Tree

EXTRA_DIST = Imakefile
bin_PROGRAMS = tree
tree_SOURCES = bxutil-c.c  tree.c
data_DATA = $(EXTRA_DIST) $(tree_SOURCES)
TOPDIR = ../../..
LDADD = $(TOPDIR)/lib/Xm/libXm.la

INCLUDES = -I.. -I$(srcdir)/$(TOPDIR)/lib ${X_CFLAGS} 
