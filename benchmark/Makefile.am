noinst_PROGRAMS = \
	fnv \
	rand

fnv_SOURCES = fnv.c
fnv_CFLAGS = -I$(top_srcdir) -I$(top_builddir)
fnv_LDADD = $(top_builddir)/snippets/libsnippets.la $(LIBM)

rand_SOURCES = rand.c
rand_CFLAGS = -I$(top_srcdir) -I$(top_builddir)
rand_LDADD = $(top_builddir)/snippets/libsnippets.la $(LIBM)
