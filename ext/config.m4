PHP_ARG_WITH(lessram,    for lessram support,
[  --with-lessram        Include lessram support])

if test "$PHP_LESSRAM" != "no"; then
  PHP_ADD_BUILD_DIR($ext_builddir/src, 1)
  PHP_ADD_BUILD_DIR($ext_builddir/classes, 1)
  PHP_ADD_INCLUDE($ext_builddir)

  PHP_SUBST(LESSRAM_SHARED_LIBADD)

  PHP_NEW_EXTENSION(
    lessram,
    lessram.c \
      src/datastore.c \
      classes/storage.c,
    $ext_shared,,-DZEND_ENABLE_STATIC_TSRMLS_CACHE=1)
fi