TESTS = \
	test-fnv \
	test-rand \
	test-linkedlist \
	test-skiplist

noinst_PROGRAMS = $(TESTS)

TESTS_CFLAGS = \
	$(CHECK_CFLAGS) \
	-I$(top_srcdir) \
	-I$(top_builddir)
TESTS_LDADD = \
	$(CHECK_LIBS) \
	$(top_builddir)/snippets/libsnippets.la

test_fnv_SOURCES = fnv.c
test_fnv_CFLAGS = $(TESTS_CFLAGS)
test_fnv_LDADD = $(TESTS_LDADD)

test_rand_SOURCES = rand.c
test_rand_CFLAGS = $(TESTS_CFLAGS)
test_rand_LDADD = $(TESTS_LDADD)

test_linkedlist_SOURCES = linkedlist.c
test_linkedlist_CFLAGS = $(TESTS_CFLAGS)
test_linkedlist_LDADD = $(TESTS_LDADD)

test_skiplist_SOURCES = skiplist.c
test_skiplist_CFLAGS = $(TESTS_CFLAGS)
test_skiplist_LDADD = $(TESTS_LDADD)

include $(top_srcdir)/check.mk

