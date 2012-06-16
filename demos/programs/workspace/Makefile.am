noinst_PROGRAMS = wsm

noinst_HEADERS = \
command_ui.h \
wsm_cb.h \
wsm_create.h \
wsmData.h \
wsmDebug.h \
wsm.h \
wsmSend.h \
wsmStruct.h \
wsm_ui.h \
xrmLib.h

wsm_SOURCES = client_win.c command_ui.c wsm.c wsmData.c wsmDebug.c \
		   wsmSend.c wsmStruct.c wsm_cb.c wsm_create.c xrmLib.c

INCLUDES = -I$(srcdir)/../../../lib -I$(srcdir)/../../lib

LDADD = ../../../lib/Xm/libXm.la -L../../lib/Wsm -lWsm
