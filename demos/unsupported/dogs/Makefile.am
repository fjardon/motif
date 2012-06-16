EXTRA_DIST = dogs.uil Dog.uil Square.uil bark.bm  down.bm  up.bm

noinst_HEADERS = Dog.h \
DogP.h \
Square.h \
SquareP.h

CLEANFILES = dogs.uid

noinst_PROGRAMS = dogs

dogs_SOURCES = dogs.c Dog.c Square.c

dogs_DEPENDENCIES = dogs.uid

LDADD = ../../../lib/Mrm/libMrm.la

INCLUDES = -I$(srcdir)/../../../lib

SUFFIXES = .uil .uid

.uil.uid:
	../../../clients/uil/uil $< -I$(srcdir) -o $@

#        INCLUDES = -I.
#         DEPLIBS = MrmClientDepLibs
# LOCAL_LIBRARIES = MrmClientLibs
#         DEFINES = /* -DOLD_DRAW_NEEDED */
# 
#            SRCS = dogs.c Dog.c Square.c
#            OBJS = dogs.o Dog.o Square.o
# 
# MComplexProgramTarget(dogs,$(LOCAL_LIBRARIES),NullParameter)
# MSimpleUidTarget(dogs)
