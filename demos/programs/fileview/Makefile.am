bindir = ${prefix}/share/Xm/fileview
datadir = ${prefix}/share/Xm/fileview

data_DATA = French.uid German.uid English.uid \
            English.df French.df German.df 
         

bin_PROGRAMS = fileview
fileview_SOURCES = main.c text.c file.c

EXTRA_DIST = $(UILS) English.df French.df German.df \
        file.h \
        fileE.h \
        fileview.h \
        main.h \
        mainE.h \
        text.h \
        textE.h
        
UILS= French.uil German.uil English.uil

LDADD = ../../../lib/Xm/libXm.la ../../../lib/Mrm/libMrm.la

INCLUDES = ${X_CFLAGS} -I$(srcdir)/../../../lib

all: French.uid German.uid English.uid

French.uid: French.uil
	../../../clients/uil/uil -o $@ $< -I$(srcdir)/../../../clients/uil -I../../../clients/uil

German.uid: German.uil
	../../../clients/uil/uil -o $@ $< -I$(srcdir)/../../../clients/uil -I../../../clients/uil

English.uid: English.uil
	../../../clients/uil/uil -o $@ $< -I$(srcdir)/../../../clients/uil -I../../../clients/uil

clean-local:
	$(RM) French.uid German.uid English.uid
