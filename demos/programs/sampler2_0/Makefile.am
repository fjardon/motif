bindir = ${prefix}/share/Xm/sampler2_0
datadir = ${prefix}/share/Xm/sampler2_0


EXTRA_DIST = bee.xpm bomb.xpm cherry.xpm \
    da.xpm \
    girlface.xpm \
    k7.xpm labir.xpm \
    main.xpm pile.xpm \
    prev.xpm \
    spir.xpm \
    stack.xpm \
    text.xpm \
    README  \
    sampler2_0.man \
    sampler2_0.help
    
bin_PROGRAMS = sampler2_0
data_DATA = README sampler2_0.help

man_MANS = sampler2_0.man

sampler2_0_SOURCES = sampler2_0.c

LDADD = ../../lib/Xmd/libXmd.a ../../../lib/Xm/libXm.la

INCLUDES = -I$(srcdir)/../../lib -I$(srcdir)/../../../lib ${X_CFLAGS} 
