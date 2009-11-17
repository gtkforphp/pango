dnl
dnl $ Id: pango 1.0.1$
dnl

PHP_ARG_WITH(pango, for Cairo graphics library support,
[  --with-pango            Enable Cairo support], yes)

if test "$PHP_PANGO" != "no"; then
  export OLD_CPPFLAGS="$CPPFLAGS"
  export CPPFLAGS="$CPPFLAGS $INCLUDES -DHAVE_PANGO"

  AC_MSG_CHECKING(PHP version)
  AC_TRY_COMPILE([#include <php_version.h>], [
#if PHP_VERSION_ID < 50200
#error  this extension requires at least PHP version 5.2.0
#endif
],
[AC_MSG_RESULT(ok)],
[AC_MSG_ERROR([need at least PHP 5.2.0])])

  export CPPFLAGS="$OLD_CPPFLAGS"

  PHP_SUBST(PANGO_SHARED_LIBADD)
  AC_DEFINE(HAVE_PANGO, 1, [ ])

  PHP_NEW_EXTENSION(pango, pango.c pango_error.c pango_layout.c pango_font.c, $ext_shared)

  EXT_PANGO_HEADERS="php_pango_api.h"

  ifdef([PHP_INSTALL_HEADERS], [
    PHP_INSTALL_HEADERS(ext/pango, $EXT_PANGO_HEADERS)
  ])

  if test "$PHP_PANGO" != "no"; then
      PANGO_CHECK_DIR=$PHP_PANGO
      PANGO_TEST_FILE=/include/pango.h
      PANGO_LIBNAME=pango
  fi
  condition="$PANGO_CHECK_DIR$PANGO_TEST_FILE"

  if test -r $condition; then
   PANGO_DIR=$PANGO_CHECK_DIR
     CFLAGS="$CFLAGS -I$PANGO_DIR/include"
   LDFLAGS=`$PANGO_DIR/bin/pango-config --libs`
  else
    AC_MSG_CHECKING(for pkg-config)
  
    if test ! -f "$PKG_CONFIG"; then
      PKG_CONFIG=`which pkg-config`
    fi

      if test -f "$PKG_CONFIG"; then
        AC_MSG_RESULT(found)
        AC_MSG_CHECKING(for pango)
    
        if $PKG_CONFIG --exists pango; then
            if $PKG_CONFIG --atleast-version=1.20 pango; then
                pango_version_full=`$PKG_CONFIG --modversion pango`
                AC_MSG_RESULT([found $pango_version_full])
                PANGO_LIBS="$LDFLAGS `$PKG_CONFIG --libs pango`"
                CAIRO_LIBS="$LDFLAGS `$PKG_CONFIG --libs cairo`"
                PANGOCAIRO_LIBS="$LDFLAGS `$PKG_CONFIG --libs pangocairo`"
                PANGO_INCS="$CFLAGS `$PKG_CONFIG --cflags-only-I pango`"
                CAIRO_INCS="$CFLAGS `$PKG_CONFIG --cflags-only-I cairo`"
                PANGOCAIRO_INCS="$CFLAGS `$PKG_CONFIG --cflags-only-I pangocairo`"
                PHP_EVAL_INCLINE($PANGO_INCS)
                PHP_EVAL_INCLINE($CAIRO_INCS)
                PHP_EVAL_INCLINE($PANGOCAIRO_INCS)
                PHP_EVAL_LIBLINE($PANGO_LIBS, PANGO_SHARED_LIBADD)
                PHP_EVAL_LIBLINE($CAIRO_LIBS, PANGO_SHARED_LIBADD)
                PHP_EVAL_LIBLINE($PANGOCAIRO_LIBS, PANGO_SHARED_LIBADD)
                AC_DEFINE(HAVE_PANGO, 1, [whether pango exists in the system])
            else
                AC_MSG_RESULT(too old)
                AC_MSG_ERROR(Ooops ! You need at least pango 1.20)
            fi
        else
            AC_MSG_RESULT(not found)
            AC_MSG_ERROR(Ooops ! no pango detected in the system)
        fi
      else
        AC_MSG_RESULT(not found)
        AC_MSG_ERROR(Ooops ! no pkg-config found .... )
      fi
   fi

   AC_MSG_CHECKING(for cairo php extension)
   if test -f "$phpincludedir/ext/cairo/php_cairo_api.h"; then
       PHP_ADD_INCLUDE($phpincludedir/ext/cairo)
       PHP_DEF_HAVE(CAIRO)
       AC_MSG_RESULT(yes)
   else
       AC_MSG_RESULT(no)
       AC_MSG_ERROR(cairo php extension not found.)
   fi  
fi
