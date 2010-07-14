ACLOCAL_AMFLAGS = -I m4

if HAVE_CHECK
tests_dir = tests
else
tests_dir =
endif

SUBDIRS = src $(tests_dir) coverage
DIST_SUBDIRS = src tests coverage

clean-local:
	-find $(top_builddir) -name *.gcda | xargs rm
	-find $(top_builddir) -name *.gcno | xargs rm

