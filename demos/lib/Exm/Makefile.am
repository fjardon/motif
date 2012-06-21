SUBDIRS = wml

noinst_LIBRARIES = libExm.a libExmMrm.a

INCLUDES = -I$(srcdir)/../../../lib -I$(srcdir)/.. ${X_CFLAGS} 

CLEANFILES = Grid.E
   
libExm_a_SOURCES = CommandB.c	Grid.c		Panner.c	MenuB.c \
                  Simple.c	ExmString.c	StringTrans.c	TabB.c 

libExmMrm_a_SOURCES = ExmMrm.c

noinst_HEADERS	= CommandB.h	CommandBP.h	ExmMrm.h	Grid.h \
                  GridP.h	MenuB.h		MenuBP.h	Panner.h \
                  PannerP.h	Simple.h	SimpleP.h	ExmString.h \
                  ExmStringP.h	StringTrans.h	StringTransP.h	TabB.h \
                  TabBP.h	

EXTRA_DIST = Imakefile

