bindir = ${prefix}/share/Xm/airport
datadir = ${prefix}/share/Xm/airport

EXTRA_DIST = Imakefile

bin_PROGRAMS = airport
airport_SOURCES = main.c dragsource.c dropsite.c
noinst_HEADERS = airport.h dragsource.h dropsite.h main.h

LDADD = ../../../lib/Xm/libXm.la
INCLUDES = -I$(srcdir)/../../../lib ${X_CFLAGS} 

