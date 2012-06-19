bindir = ${prefix}/share/Xm/setDate
datadir = ${prefix}/share/Xm/setDate

data_DATA = XmdSetDate.ad README
bin_PROGRAMS = setDate
man_MANS = setDate.man

EXTRA_DIST = XmdSetDate.ad README setDate.man
SOURCES = setDate.c

LDADD = ../../../lib/Xm/libXm.la

INCLUDES = -I$(srcdir)/../../../lib ${X_CFLAGS} 
