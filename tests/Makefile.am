TESTS = \
	test-fnv

noinst_PROGRAMS = $(TESTS)

TESTS_CFLAGS = \
	$(CHECK_CFLAGS) \
	-I$(top_srcdir)/src
TESTS_LDADD = \
	$(CHECK_LIBS) \
	$(top_builddir)/src/libsnippets.la

test_fnv_SOURCES = fnv.c
test_fnv_CFLAGS = $(TESTS_CFLAGS)
test_fnv_LDADD = $(TESTS_LDADD)

include $(top_srcdir)/check.mk

