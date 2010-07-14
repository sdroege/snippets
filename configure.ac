AC_INIT([snippets], [0.0.1])
AC_CONFIG_SRCDIR([Makefile.am])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS(config.h)

AM_INIT_AUTOMAKE([dist-bzip2 -Wno-portability])
AM_MAINTAINER_MODE

LT_INIT([disable-shared static pic-only])

m4_ifdef([AM_SILENT_RULES],
  [AM_SILENT_RULES([yes])],
  [
    AM_DEFAULT_VERBOSITY=1
    AC_SUBST(AM_DEFAULT_VERBOSITY)
  ]
)

AC_PROG_CC
AC_PROG_CPP
AC_PROG_GREP
AC_PROG_INSTALL
AM_PROG_CC_C_O

AC_HEADER_STDC
AC_C_CONST
AC_C_INLINE
AC_ISC_POSIX
AC_SYS_LARGEFILE

AC_C_BIGENDIAN

AC_CHECK_LIBM
AC_SUBST(LIBM)

AX_CREATE_STDINT_H([src/snippets-stdint.h])

PKG_PROG_PKG_CONFIG

PKG_CHECK_MODULES(CHECK, check, have_check="yes", have_check="no")
AC_SUBST(CHECK_CFLAGS)
AC_SUBST(CHECK_LIBS)
AM_CONDITIONAL(HAVE_CHECK, test "x$have_check" != "xno")

AC_PATH_PROG(VALGRIND, valgrind, no)
AM_CONDITIONAL(HAVE_VALGRIND, test "x$VALGRIND" != "xno")

have_valgrind_3_4_0=no
if test "x$VALGRIND" != "xno"; then
  valgrind_version=0
  valgrind_version="`$VALGRIND --version | head -n 1 |  sed 's/^[[^0-9]]*//' | sed 's/[[^0-9]]*$//' | cut -d' ' -f1`"
  AC_MSG_CHECKING([for valgrind version $valgrind_version >= 3.4.0])
  if perl -we "exit ((v$valgrind_version ge v3.4.0) ? 0 : 1)"; then
    have_valgrind_3_4_0=yes
    AC_MSG_RESULT([yes])
  else
    AC_MSG_RESULT([no])
  fi
fi
AM_CONDITIONAL(HAVE_VALGRIND_3_4_0, test "x$have_valgrind_3_4_0" = "xyes")

AC_PATH_PROG(GDB, gdb, no)
AM_CONDITIONAL(HAVE_GDB, test ! "x$GDB" = "xno")

CFLAGS="$CFLAGS -Wall"

AC_ARG_ENABLE([coverage],
    AS_HELP_STRING([--enable-coverage],
      [enable code coverage output (default=no)])],
      [],
      [enable_coverage=no])

if test "x$enable_coverage" = "xyes"; then
  AC_PATH_PROG(LCOV, lcov, no)
  AC_PATH_PROG(GENHTML, genhtml, no)

  if test "x$LCOV" != "xno" -a "x$GENHTML" != "xno"; then
    CFLAGS="$CFLAGS -O0 -g -fprofile-arcs -ftest-coverage"
    LDFLAGS="$LDFLAGS -fprofile-arcs"
  else
    AC_MSG_ERROR([lcov is required to build coverage information])
  fi
fi
AM_CONDITIONAL(ENABLE_COVERAGE, test "x$enable_coverage" != "xno")

AC_CONFIG_FILES([
  Makefile
  src/Makefile
  tests/Makefile
  coverage/Makefile
])

AC_OUTPUT
