EXTRA_DIST = dog.uil plane.uil superman.uil xmanimate.uil \
superman1.xbm \
superman2.xbm \
superman3.xbm \
superman4.xbm \
superman5.xbm \
superman6.xbm

CLEANFILES =  dog.uid plane.uid superman.uid xmanimate.uid

noinst_PROGRAMS = xmanimate

LDADD = ../../lib/Xmd/libXmd.a ../../../lib/Mrm/libMrm.la ../../../lib/Xm/libXm.la

INCLUDES = -I$(srcdir)/../../../lib -I$(srcdir)/../../lib

all: dog.uid plane.uid superman.uid xmanimate.uid

dog.uid: dog.uil
	../../../clients/uil/uil -o $@ $< -I$(srcdir)/../../../clients/uil -I../../../clients/uil

plane.uid: plane.uil
	../../../clients/uil/uil -o $@ $< -I$(srcdir)/../../../clients/uil -I../../../clients/uil

superman.uid: superman.uil
	../../../clients/uil/uil -o $@ $< -I$(srcdir)/../../../clients/uil -I../../../clients/uil

xmanimate.uid: xmanimate.uil
	../../../clients/uil/uil -o $@ $< -I$(srcdir)/../../../clients/uil -I../../../clients/uil

clean-local:
	$(RM) periodic.uid
