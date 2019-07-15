/**        $static_memory_usage,
 * @author Jefferson González
 * @license MIT
 * @link https://github.com/jgmdev/lessram Source code.
*/

#ifndef PHP_LESSRAM_H
#define PHP_LESSRAM_H

extern zend_module_entry lessram_module_entry;
#define phpext_lessram_ptr &lessram_module_entry

#define PHP_LESSRAM_VERSION "0.1"

#ifdef PHP_WIN32
#	define PHP_LESSRAM_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_LESSRAM_API __attribute__ ((visibility("default")))
#else
#	define PHP_LESSRAM_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

#if defined(ZTS) && defined(COMPILE_DL_LESSRAM)
ZEND_TSRMLS_CACHE_EXTERN()
#endif

#endif	/* PHP_LESSRAM_H */