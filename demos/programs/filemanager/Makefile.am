bin_PROGRAMS = filemanager

filemanager_SOURCES = filemanager.c actions.c readdir.c convert.c interface.c

LDADD = ../../lib/Xmd/libXmd.a ../../../lib/Xm/libXm.la

INCLUDES = -I$(srcdir)/../../lib -I$(srcdir)/../../../lib ${X_CFLAGS} 

noinst_HEADERS = filemanager.h
