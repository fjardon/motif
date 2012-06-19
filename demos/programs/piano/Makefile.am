bindir = ${prefix}/share/Xm/piano
datadir = ${prefix}/share/Xm/piano

man_MANS = piano.man
data_DATA  = piano.images README Inv8_part1.piano Inv8_part2.piano

bin_PROGRAMS = piano
piano_SOURCES = piano.c

EXTRA_DIST = piano.images \
             README \
             Inv8_part1.piano \
             Inv8_part2.piano \
             piano.man
             
LDADD = ../../../lib/Xm/libXm.la -lm

INCLUDES = -I$(srcdir)/../../../lib ${X_CFLAGS} 
