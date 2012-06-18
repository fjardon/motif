bin_PROGRAMS = todo

noinst_HEADERS = page.h

todo_SOURCES = todo.c io.c actions.c

INCLUDES = -I$(srcdir)/../../../lib -I$(srcdir)/../../lib ${X_CFLAGS} 

LDADD = ../../../lib/Xm/libXm.la -L../../lib/Xmd -lXmd -L../../lib/Exm -lExm
