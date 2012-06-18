EXTRA_DIST = Uil.msg
#
# I don't seem to have all the pieces to make this thing
#
bin_PROGRAMS = uil

UilParser.lo: UilParser.c
	$(LTCOMPILE) -DCALLABLE -c UilParser.c

libUil_la_LIBADD = UilParser.lo ../../lib/Mrm/libMrm.la ../../lib/Xm/libXm.la 

uil_LDADD = ../../lib/Mrm/libMrm.la ../../lib/Xm/libXm.la 

INCLUDES = ${X_CFLAGS} -DINCDIR=\"@INCDIR@\" \
           -DLIBDIR=\"@LIBDIR@\" \
           -I$(srcdir)/../../include \
           -I$(srcdir)/../../lib \
           -I$(srcdir)/../../tools/wml \
           -I../../tools/wml

UilParser.c:
	test -f UilParser.c || $(LN_S) $(srcdir)/UilMain.c UilParser.c

UilLexPars.c: $(srcdir)/../../tools/wml/Uil.c
	test -f UilLexPars.c || $(LN_S) $(srcdir)/../../tools/wml/Uil.c UilLexPars.c

COMMON_SRC =	UilCmd.c	UilDiags.c	UilKeyTab.c	UilLexAna.c  \
		UilLexPars.c	UilLstLst.c	UilP2Out.c	UilP2Reslv.c \
		UilSarComp.c	UilSarExp.c	UilSarInc.c	UilSarMod.c \
		UilSarObj.c	UilSarProc.c	UilSarVal.c	UilSrcSrc.c  \
		UilSymNam.c	UilSymStor.c	UilData.c	\
		UilLstMac.c	UilSemVal.c	UilSemCSet.c	UilDB.c

SRCS =		$(COMMON_SRC)
SRCS2 =		$(COMMON_SRC)	UilMain.c

HEADERS_1 = 	Uil.h		UilSymGl.h	UilSymDef.h \
		UilDef.h	XmAppl.uil

uildir = $(includedir)/uil

uil_HEADERS = $(HEADERS_1)

noinst_HEADERS = UilCmdDef.h \
UilCompGl.h \
UilDefI.h \
UilDiagDef.h \
UilIODef.h \
UilLexDef.h \
UilMess.h \
UilMessTab.h \
UilSarDef.h \
UilSrcDef.h

lib_LTLIBRARIES = libUil.la

libUil_la_SOURCES = $(SRCS)

libUil_la_DEPENDENCIES = UilDBDef.h UilParser.lo

UilDBDef.h:
	$(LN_S) $(srcdir)/../../tools/wml/UilDBDef.h UilDBDef.h

uil_SOURCES = $(SRCS2)

if MessageCatalog

CAT_FILES = Uil.cat UilMsgCatI.h

$(CAT_FILES): Uil.msg
	$(mkinstalldirs) $(top_builddir)/localized/C/msg
	$(top_builddir)/localized/util/mkcatdefs UilMsgCatI.h $(srcdir)/Uil.msg >$(top_builddir)/localized/C/msg/Uil.msg
	gencat Uil.cat $(top_builddir)/localized/C/msg/Uil.msg

endif

clean-local:
	$(RM) UilLexPars.c UilParser.c
