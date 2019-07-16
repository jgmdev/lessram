/**
 * @author Jefferson González
 * @license MIT
 * @link https://github.com/jgmdev/lessram Source code.
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <src/datastore.h>

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_lessram.h"

#include <classes/storage.h>

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(lessram)
{
    PHP_MINIT(LessRam_Storage)(INIT_FUNC_ARGS_PASSTHRU);

    return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(lessram)
{
    return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(lessram)
{
    php_info_print_table_start();
    php_info_print_table_header(2, "lessram support", "enabled");
    php_info_print_table_end();
}
/* }}} */

/* {{{ lessram_module_entry
 */
zend_module_entry lessram_module_entry = {
    STANDARD_MODULE_HEADER,
    "lessram",
    NULL,
    PHP_MINIT(lessram),
    PHP_MSHUTDOWN(lessram),
    NULL,
    NULL,
    PHP_MINFO(lessram),
    PHP_LESSRAM_VERSION,
    STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_LESSRAM
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
#endif
ZEND_GET_MODULE(lessram)
#endif