EXTRA_DIST = hellomotif.uil

noinst_PROGRAMS = hellomotif

LDADD = ../../../lib/Xm/libXm.la ../../../lib/Mrm/libMrm.la

INCLUDES = -I$(srcdir)/../../../lib

all: hellomotif.uid

hellomotif.uid: hellomotif.uil
	../../../clients/uil/uil -o $@ $< -I$(srcdir)/../../../clients/uil -I../../../clients/uil

clean-local:
	$(RM) hellomotif.uid
