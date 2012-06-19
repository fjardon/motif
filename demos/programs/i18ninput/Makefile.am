bindir = ${prefix}/share/Xm/i18ninput
datadir = ${prefix}/share/Xm/i18ninput

bin_PROGRAMS = i18ninput

i18ninput_SOURCES = i18ninput.c input.c 
man_MANS = i18ninput.man
data_DATA = XmdI18nInput.ad README i18ninput.help

EXTRA_DIST = XmdI18nInput.ad README i18ninput.man i18ninput.help \
             Imakefile
LDADD = ../../lib/Xmd/libXmd.a ../../../lib/Xm/libXm.la

INCLUDES = -I$(srcdir)/../../lib -I$(srcdir)/../../../lib ${X_CFLAGS} 
