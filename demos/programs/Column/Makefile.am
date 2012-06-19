bindir = ${prefix}/share/Xm/Column
datadir = ${prefix}/share/Xm/Column

bin_PROGRAMS = column
column_SOURCES = column.c

EXTRA_DIST = Imakefile

TOPDIR = ../../..

LDADD = $(TOPDIR)/lib/Xm/libXm.la

INCLUDES = -I$(srcdir)/$(TOPDIR)/lib ${X_CFLAGS} 
