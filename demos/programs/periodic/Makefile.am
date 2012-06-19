
bindir = ${prefix}/share/Xm/periodic
datadir = ${prefix}/share/Xm/periodic

bin_PROGRAMS = periodic
data_DATA = periodic.uid XmdPeriodic.ad README

EXTRA_DIST = periodic.uil periodic_l.uil XmdPeriodic.ad
LDADD = ../../../lib/Xm/libXm.la ../../../lib/Mrm/libMrm.la

INCLUDES = -I$(srcdir)/../../../lib ${X_CFLAGS} 

all: periodic.uid

periodic.uid: periodic.uil
	../../../clients/uil/uil -o $@ $< -I$(srcdir)/../../../clients/uil -I../../../clients/uil

clean-local:
	$(RM) periodic.uid

    
