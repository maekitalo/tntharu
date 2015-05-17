AC_INIT([tntharu], [0.1], [tommi@tntnet.org])
AM_INIT_AUTOMAKE([foreign])
LT_INIT([disable-static])
AC_CONFIG_MACRO_DIR([m4])

AC_PROG_CXX
AC_LANG(C++)
AX_CXX_COMPILE_STDCXX_11(noext, optional)

AC_CONFIG_HEADERS([config.h])
AC_CHECK_HEADER([cxxtools/mutex.h], , AC_MSG_ERROR([cxxtools headers not found]))
AC_CHECK_HEADER([tnt/tntnet.h], , AC_MSG_ERROR([tntnet headers not found]))
#AC_CHECK_HEADER([tntdb/connection.h], , AC_MSG_ERROR([tntdb headers not found]))
AC_CHECK_HEADER([hpdf.h], , AC_MSG_ERROR([libharu (pdf creation library) headers not found]))
AC_CHECK_PROGS(ECPPC, [ecppc])
AC_PROG_SED
AX_CHECK_COMPILE_FLAG([-Wall], [CPPFLAGS="$CPPFLAGS -Wall"])
AX_CHECK_COMPILE_FLAG([-pedantic], [CPPFLAGS="$CPPFLAGS -pedantic"])

AC_OUTPUT([
    Makefile
])
