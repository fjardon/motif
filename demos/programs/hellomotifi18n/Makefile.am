SUBDIRS = C english french hebrew japan japanese swedish

EXTRA_DIST = README helloint.uil

CLEANFILES = helloint.uid

noinst_PROGRAMS = helloint

helloint_DEPENDENCIES = helloint.uid C/uid/l_strings.uid english/uid/l_strings.uid french/uid/l_strings.uid hebrew/uid/l_strings.uid japan/uid/l_strings.uid japanese/uid/l_strings.uid swedish/uid/l_strings.uid

INCLUDES = -I$(top_srcdir)/lib
LDADD = $(top_builddir)/lib/Mrm/libMrm.la

SUFFIXES = .uil .uid

.uil.uid:
	$(top_builddir)/clients/uil/uil $< -I$(srcdir) -o $@
