bindir = ${prefix}/share/Xm/todo
datadir = ${prefix}/share/Xm/todo
man_MANS = todo.man

data_DATA = README XmdTodo.ad example.todo todo.help

EXTRA_DIST = README XmdTodo.ad example.todo todo.man todo.help Imakefile


bin_PROGRAMS = todo

noinst_HEADERS = page.h

todo_SOURCES = todo.c io.c actions.c

INCLUDES = -I$(srcdir)/../../../lib -I$(srcdir)/../../lib ${X_CFLAGS} 
LDADD = -L../../lib/Xmd -lXmd -L../../lib/Exm -lExm ../../../lib/Xm/libXm.la 
