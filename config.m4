dnl $Id$
dnl config.m4 for extension hypernode

PHP_ARG_ENABLE(hypernode, whether to enable hypernode support,
[  --enable-hypernode           Enable hypernode support])

if test "$PHP_HYPERNODE" != "no"; then
  dnl Write more examples of tests here...

  PHP_NEW_EXTENSION(hypernode, hypernode.c, $ext_shared)
fi
