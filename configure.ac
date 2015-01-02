#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([OSMesa gl print test], [0.1.0], [andy.weber.aw@gmail.com])

# Checks for programs.
AC_PROG_CXX
AC_LANG(C++)

AC_CHECK_PROG([HAVE_MKOCTFILE], [mkoctfile], [yes], [no])
if [test $HAVE_MKOCTFILE = "no"]; then
  AC_MSG_ERROR([mkoctfile required to install $PACKAGE_NAME])
fi

# Checks for OSMesa library.
AC_CHECK_LIB(OSMesa, OSMesaCreateContext, [], [
  AC_MSG_ERROR([unable to find the OSMesaCreateContext() function])
])

# check for OSMesa headers
AC_CHECK_HEADERS([GL/osmesa.h],[],[
  AC_MSG_ERROR([unable to find the needed OSMesa headers])
])

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE
AC_FUNC_REALLOC
AC_CHECK_FUNCS([munmap select strerror])

LIBS+="-lm"

AC_CONFIG_FILES([Makefile])
AC_OUTPUT

AC_MSG_NOTICE([

$PACKAGE_NAME is now configured with

   LIBS:   $LIBS
])
