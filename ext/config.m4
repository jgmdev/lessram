PHP_ARG_WITH(lessram,    for lessram support,
[  --with-lessram        Include lessram support])

PHP_ARG_ENABLE(lessram-stdmalloc, to use standard malloc instead of php,
[  --enable-lessram-stdmalloc     Use standard malloc], no, no)

PHP_ARG_ENABLE(lessram-rpmalloc, to use rpmalloc,
[  --enable-lessram-rpmalloc     Enable rpmalloc], no, no)

if test "$PHP_LESSRAM" != "no"; then
  PHP_ADD_BUILD_DIR($ext_builddir/src, 1)
  PHP_ADD_BUILD_DIR($ext_builddir/classes, 1)
  PHP_ADD_INCLUDE($ext_builddir)

  PHP_SUBST(LESSRAM_SHARED_LIBADD)

  RPMALLOC=""
  if test "$PHP_LESSRAM_STDMALLOC" != "no"; then
    AC_DEFINE(USE_C_MALLOC,1,[ ])
  elif test "$PHP_LESSRAM_RPMALLOC" != "no"; then
    AC_DEFINE(USE_RP_MALLOC,1,[ ])
    RPMALLOC=src/rpmalloc/rpmalloc.c
  fi

  PHP_NEW_EXTENSION(
    lessram,
    lessram.c $RPMALLOC \
      src/datastore.c \
      src/datastoresimple.c \
      classes/storage.c \
      classes/list.c,
    $ext_shared,,-DZEND_ENABLE_STATIC_TSRMLS_CACHE=1)
fi