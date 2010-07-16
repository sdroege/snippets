AC_INIT([snippets], [0.0.1])
AC_CONFIG_SRCDIR([Makefile.am])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS(config.h)

AM_INIT_AUTOMAKE([dist-bzip2 -Wno-portability])
AM_MAINTAINER_MODE

LT_INIT([pic-only])

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

# set libtool versioning
# +1 :  0 : +1   == new interface that does not break old one.
# +1 :  0 :  0   == changed/removed an interface. Breaks old apps.
#  ? : +1 :  ?   == internal changes that doesn't break anything.
# CURRENT : REVISION : AGE
LT_CURRENT=0
LT_REVISION=0
LT_AGE=0

AC_SUBST(LT_CURRENT)
AC_SUBST(LT_REVISION)
AC_SUBST(LT_AGE)

AX_CREATE_STDINT_H([snippets/snippets-stdint.h])

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

AC_ARG_ENABLE([coverage],
    AS_HELP_STRING([--enable-coverage],
      [enable code coverage output (default=no)])],
      [],
      [enable_coverage=no])

if test "x$enable_coverage" = "xyes"; then
  if test "x$enable_shared" != "xno"; then
    AC_MSG_ERROR([--disable-shared is required for coverage reporting])
  fi

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

CFLAGS="$CFLAGS -Wall"

AC_ARG_ENABLE(more-warnings,
AC_HELP_STRING([--enable-more-warnings], [Enable more compiler warnings]),
set_more_warnings="$enableval", set_more_warnings=no)

AC_MSG_CHECKING(for more warnings)
if test "$GCC" = "yes" -a "$set_more_warnings" != "no"; then
  AC_MSG_RESULT(yes)
  
  for option in -Wchar-subscripts -Wmissing-declarations -Wmissing-prototypes -Wnested-externs -Wpointer-arith -Wcast-align -Wredundant-decls -Wundef -Wwrite-strings -Wformat-nonliteral -Wformat-security -Wold-style-definition -Winit-self -Wmissing-include-dirs -Waddress -Waggregate-return -Wno-multichar; do
    SAVE_CFLAGS="$CFLAGS"
    CFLAGS="$CFLAGS $option"
    AC_MSG_CHECKING([whether gcc understands $option])
    AC_TRY_COMPILE([], [],
      has_option=yes,
      has_option=no,)
    if test $has_option = no; then
      CFLAGS="$SAVE_CFLAGS"
    fi
    AC_MSG_RESULT($has_option)
    unset has_option
    unset SAVE_CFLAGS
  done
  unset option
else
  AC_MSG_RESULT(no)
fi

AC_ARG_ENABLE(warnings-as-errors,
AC_HELP_STRING([--enable-warnings-as-errors], [Handle compiler warnings as errors]),
set_werror="$enableval", set_werror=no)

AC_MSG_CHECKING(for handling compiler warnings as errors)
if test "$GCC" = "yes" -a "$set_werror" != "no"; then
  AC_MSG_RESULT(yes)
  CFLAGS="$CFLAGS -Werror"
else
  AC_MSG_RESULT(no)
fi

AC_CONFIG_FILES([
  Makefile
  snippets/Makefile
  tests/Makefile
  coverage/Makefile
])

AC_OUTPUT
