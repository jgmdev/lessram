/**
 * @author Jefferson González
 * @license MIT
 * @link https://github.com/jgmdev/lessram Source code.
*/

#ifndef HAVE_PHP_LESSRAM_LIST_H
#define HAVE_PHP_LESSRAM_LIST_H

extern zend_class_entry *lessramList_ce;

typedef struct _php_lessram_list_t {
	DataStorageSimple* storage;
	zend_object std;
} php_lessram_list_t;

#define php_lessram_list_from(o) ((php_lessram_list_t*) ((char*) o - XtOffsetOf(php_lessram_list_t, std)))
#define php_lessram_list_fetch(z) php_lessram_list_from(Z_OBJ_P(z))

PHP_MINIT_FUNCTION(LessRam_List);

#endif