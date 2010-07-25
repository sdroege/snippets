lib_LTLIBRARIES = libsnippets.la

libsnippets_la_SOURCES = \
	linkedlist.c \
	fnv.c \
	rand.c \
	skiplist.c \
	bloomfilter.c

libsnippets_la_CFLAGS = \
	-I$(top_srcdir) \
	-I$(top_builddir)
libsnippets_la_LDFLAGS = \
	-version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE) \
	-export-symbols-regex '^snippets_.*$$' \
	-no-undefined
libsnippets_la_LIBADD = \
	$(LIBM)

libsnippetsdir = $(includedir)/snippets

libsnippets_HEADERS = \
	utils.h \
	snippets-stdint.h \
	linkedlist.h \
	fnv.h \
	rand.h \
	skiplist.h \
	bloomfilter.h

