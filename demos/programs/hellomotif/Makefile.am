bindir = ${prefix}/share/Xm/hellomotif
datadir = ${prefix}/share/Xm/hellomotif

bin_PROGRAMS = hellomotif
data_DATA = README hellomotif.uid

EXTRA_DIST = hellomotif.uil Imakefile
LDADD = ../../../lib/Xm/libXm.la ../../../lib/Mrm/libMrm.la
INCLUDES = -I$(srcdir)/../../../lib ${X_CFLAGS} 

all: hellomotif.uid

hellomotif.uid: hellomotif.uil
	../../../clients/uil/uil -o $@ $< -I$(srcdir)/../../../clients/uil -I../../../clients/uil

clean-local:
	$(RM) hellomotif.uid
