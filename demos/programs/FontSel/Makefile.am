bindir = ${prefix}/share/Xm/fontsel
datadir = ${prefix}/share/Xm/fontsel

bin_PROGRAMS = fontsel
fontsel_SOURCES = callbacks-c.c \
                  creation-c.c \
                  fontsel.c \
                  fontsel.h

LDADD = $(TOPDIR)/lib/Xm/libXm.la
TOPDIR = ../../..
INCLUDES = -I$(srcdir)/$(TOPDIR)/lib ${X_CFLAGS} 
