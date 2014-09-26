dnl $Id$
dnl config.m4 for extension hypernode

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(hypernode, for hypernode support,
dnl Make sure that the comment is aligned:
dnl [  --with-hypernode             Include hypernode support])

dnl Otherwise use enable:

PHP_ARG_ENABLE(hypernode, whether to enable hypernode support,
dnl Make sure that the comment is aligned:
[  --enable-hypernode           Enable hypernode support])

if test "$PHP_HYPERNODE" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-hypernode -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/hypernode.h"  # you most likely want to change this
  dnl if test -r $PHP_HYPERNODE/$SEARCH_FOR; then # path given as parameter
  dnl   HYPERNODE_DIR=$PHP_HYPERNODE
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for hypernode files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       HYPERNODE_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$HYPERNODE_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the hypernode distribution])
  dnl fi

  dnl # --with-hypernode -> add include path
  dnl PHP_ADD_INCLUDE($HYPERNODE_DIR/include)

  dnl # --with-hypernode -> check for lib and symbol presence
  dnl LIBNAME=hypernode # you may want to change this
  dnl LIBSYMBOL=hypernode # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $HYPERNODE_DIR/lib, HYPERNODE_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_HYPERNODELIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong hypernode lib version or lib not found])
  dnl ],[
  dnl   -L$HYPERNODE_DIR/lib -lm
  dnl ])
  dnl
  dnl PHP_SUBST(HYPERNODE_SHARED_LIBADD)

  PHP_NEW_EXTENSION(hypernode, hypernode.c, $ext_shared)
fi
