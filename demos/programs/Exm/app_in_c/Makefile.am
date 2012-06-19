bindir = ${prefix}/share/Xm/Exm/app_in_c
datadir = ${prefix}/share/Xm/Exm/app_in_c

man_MANS = exm_in_c.man
bin_PROGRAMS = exm_in_c
exm_in_c_SOURCES = exm_in_c.c

EXTRA_DIST = exm_in_c.man

INCLUDES = -I$(srcdir)/../../../../lib \
           -I$(srcdir)/../../../lib ${X_CFLAGS} 
LDADD = ../../../../lib/Xm/libXm.la \
        ../../../lib/Exm/libExm.a
