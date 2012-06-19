bindir = ${prefix}/share/Xm/Tabstack
datadir = ${prefix}/share/Xm/Tabstack

bin_PROGRAMS = tabstack
tabstack_SOURCES = bxutils.c  \
            callbacks.c  \
            creation.c  \
            main.c \
            creation.h
      
EXTRA_DIST = Imakefile
            
TOPDIR = ../../..
LDADD = $(TOPDIR)/lib/Xm/libXm.la

INCLUDES = -I$(srcdir)/$(TOPDIR)/lib ${X_CFLAGS} 
