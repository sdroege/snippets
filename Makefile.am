ACLOCAL_AMFLAGS = -I m4

if HAVE_CHECK
tests_dir = tests
else
tests_dir =
endif

SUBDIRS = snippets $(tests_dir) benchmark coverage
DIST_SUBDIRS = snippets tests benchmark coverage

clean-local:
	-find $(top_builddir) -name *.gcda | xargs rm
	-find $(top_builddir) -name *.gcno | xargs rm

check-valgrind:
	$(MAKE) -C tests check-valgrind

if ENABLE_COVERAGE
coverage: clean all check
	$(MAKE) -C coverage coverage
endif

