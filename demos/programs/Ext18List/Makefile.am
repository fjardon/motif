bindir = ${prefix}/share/Xm/Ext18list
datadir = ${prefix}/share/Xm/Ext18list

bin_PROGRAMS = ext18list
ext18list_SOURCES = bxutil-c.c  \
         callbacks-c.c  \
         creation.c  \
         ext18list.c \
         extlist.h
         
TOPDIR = ../../..
LDADD = $(TOPDIR)/lib/Xm/libXm.la

INCLUDES = -I.. -I$(srcdir)/$(TOPDIR)/lib ${X_CFLAGS} 
