bindir = ${prefix}/share/Xm/autopopups
datadir = ${prefix}/share/Xm/autopopups

data_DATA = README
man_MANS = autopopups.man
bin_PROGRAMS = autopopups
autopopups_SOURCES = autopopups.c

EXTRA_DIST = README autopopups.man Imakefile
LDADD = ../../../lib/Xm/libXm.la

INCLUDES = -I$(srcdir)/../../../lib ${X_CFLAGS} 
