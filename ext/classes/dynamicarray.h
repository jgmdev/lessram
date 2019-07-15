/**
 * @author Jefferson González
 * @license MIT
 * @link https://github.com/jgmdev/lessram Source code.
*/

#ifndef HAVE_PHP_LESSRAM_DYNAMIC_H
#define HAVE_PHP_LESSRAM_DYNAMIC_H

extern zend_class_entry *lessramDynamic_ce;

typedef struct _php_lessram_dynamic_t {
	zval* this;
	DataStorage* storage;
	zend_object std;
} php_lessram_dynamic_t;

#define php_lessram_dynamic_from(o) ((php_lessram_dynamic_t*) ((char*) o - XtOffsetOf(php_lessram_dynamic_t, std)))
#define php_lessram_dynamic_fetch(z) php_lessram_dynamic_from(Z_OBJ_P(z))

PHP_MINIT_FUNCTION(LessRam_DynamicArray);

#endif