bindir = ${prefix}/share/Xm/Tree
datadir = ${prefix}/share/Xm/Tree

bin_PROGRAMS = tree
tree_SOURCES = bxutil-c.c  tree.c

TOPDIR = ../../..
LDADD = $(TOPDIR)/lib/Xm/libXm.la

INCLUDES = -I.. -I$(srcdir)/$(TOPDIR)/lib ${X_CFLAGS} 
