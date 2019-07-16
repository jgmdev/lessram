/**
 * @author Jefferson González
 * @license MIT
 * @link https://github.com/jgmdev/lessram Source code.
*/

#ifndef HAVE_PHP_LESSRAM_STORAGE_H
#define HAVE_PHP_LESSRAM_STORAGE_H

extern zend_class_entry *lessramStorage_ce;

typedef struct _php_lessram_storage_t {
	zval* this;
	DataStorage* storage;
	zend_object std;
} php_lessram_storage_t;

#define php_lessram_storage_from(o) ((php_lessram_storage_t*) ((char*) o - XtOffsetOf(php_lessram_storage_t, std)))
#define php_lessram_storage_fetch(z) php_lessram_storage_from(Z_OBJ_P(z))

PHP_MINIT_FUNCTION(LessRam_Storage);

#endif