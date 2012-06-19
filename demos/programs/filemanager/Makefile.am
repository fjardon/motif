bindir = ${prefix}/share/Xm/filemanager
datadir = ${prefix}/share/Xm/filemanager

data_DATA = filemanager.help \
            XmdFilemanager.ad

man_MANS = filemanager.man

bin_PROGRAMS = filemanager

noinst_HEADERS = filemanager.h

filemanager_SOURCES = filemanager.c \
    actions.c \
    readdir.c \
    convert.c \
    interface.c

EXTRA_DIST = filemanager.man filemanager.help XmdFilemanager.ad

LDADD = ../../lib/Xmd/libXmd.a ../../../lib/Xm/libXm.la
INCLUDES = -I$(srcdir)/../../lib -I$(srcdir)/../../../lib ${X_CFLAGS} 


