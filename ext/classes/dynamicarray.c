/**
 * @author Jefferson González
 * @license MIT
 * @link https://github.com/jgmdev/lessram Source code.
*/

#ifndef HAVE_PHP_LESSRAM_DYNAMIC
#define HAVE_PHP_LESSRAM_DYNAMIC

#include <src/datastore.h>

#include "php.h"
#include "php/ext/json/php_json.h"
#include "php/Zend/zend_smart_str.h"

#include <classes/dynamicarray.h>

static DataString SEP_CHAR = {"~", 1, 0};
static DataString SEPR_CHAR = {"\\~", 2, 0};

static enum savings {
    LOWEST = 0,
    MODERATE = 1,
    HIGHEST = 2
};

zend_object_handlers php_lessram_dynamic_handlers;

zend_class_entry* lessramDynamic_ce;

zend_object* php_lessram_dynamic_create(zend_class_entry* ce) {
    php_lessram_dynamic_t* w = (php_lessram_dynamic_t*)
        ecalloc(
            1,
            sizeof(php_lessram_dynamic_t) + zend_object_properties_size(ce)
        )
    ;

    zend_object_std_init(&w->std, ce);

    object_properties_init(&w->std, ce);

    w->std.handlers = &php_lessram_dynamic_handlers;

    return &w->std;
}

ZEND_BEGIN_ARG_INFO_EX(php_lessram_dynamic_construct_info, 0, 0, 0)
    ZEND_ARG_TYPE_INFO(0, savings, IS_LONG, 0)
ZEND_END_ARG_INFO()

/* {{{ proto DynamicArray DynamicArray::__construct(savings=DynamicArray::LOWEST) */
PHP_METHOD(DynamicArray, __construct)
{
    php_lessram_dynamic_t* d = php_lessram_dynamic_fetch(getThis());

    d->this = getThis();

    zend_long savings = 0;

    if(ZEND_NUM_ARGS() > 0)
    {
        if (
            zend_parse_parameters(
                ZEND_NUM_ARGS(), "L", &savings
            ) 
            != 
            SUCCESS
        ) 
        {
            savings = 0;
        }
    }

    switch(savings)
    {
        case HIGHEST:
            d->storage = data_storage_new();
            break;
        case MODERATE: 
            d->storage = data_storage_new_with_index();
            break;
        default:
            d->storage = data_storage_new_with_index_and_list();
    }
    
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_lessram_dynamic_destruct_info, 0, 0, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void DynamicArray::__destruct() */
PHP_METHOD(DynamicArray, __destruct)
{
    php_lessram_dynamic_t* d = php_lessram_dynamic_fetch(getThis());

    data_storage_free(d->storage);
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_lessram_dynamic_append_info, 0, 0, 1)
    ZEND_ARG_INFO(0, data)
ZEND_END_ARG_INFO()

/* {{{ proto void DynamicArray::append(mixed data) */
PHP_METHOD(DynamicArray, append)
{
    php_lessram_dynamic_t* d = php_lessram_dynamic_fetch(getThis());
    zval* data = NULL;
    zend_string* data_str = NULL;

    if(
        zend_parse_parameters_ex(
            ZEND_PARSE_PARAMS_QUIET, 
            ZEND_NUM_ARGS(), 
            "z", 
            &data
        ) 
        != 
        SUCCESS
    )
    {
        return;
    }

    if(Z_TYPE_P(data) == IS_ARRAY || Z_TYPE_P(data) == IS_OBJECT)
    {
        smart_str result = {0};
        php_json_encode(&result, data, 0);
        data_str = smart_str_extract(&result);

        DataString* ds = data_string_new("", ZSTR_LEN(data_str), 20);
        memcpy(ds->string, ZSTR_VAL(data_str), ZSTR_LEN(data_str));
        data_string_prepend_string(ds, "\0:", 2, 0);

        data_storage_append(d->storage, ds);

        smart_str_free(&result);
        zend_string_efree(data_str);
        data_string_free(ds);
    }
    else // TODO: throw exception if adding resources.
    {
        convert_to_string(data);
        data_str = Z_STR_P(data);

        DataString* ds = data_string_new("", ZSTR_LEN(data_str), 20);
        memcpy(ds->string, ZSTR_VAL(data_str), ZSTR_LEN(data_str));

        data_storage_append(d->storage, ds);

        data_string_free(ds);
    }
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_lessram_dynamic_append_data_info, 0, 0, 1)
    ZEND_ARG_TYPE_INFO(0, data, IS_STRING, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void DynamicArray::appendData(string data) */
PHP_METHOD(DynamicArray, appendData)
{
    php_lessram_dynamic_t* d = php_lessram_dynamic_fetch(getThis());
    zend_string* data = NULL;

    if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "S", &data) != SUCCESS) {
        return;
    }

    convert_to_string(data);

    DataString* ds = data_string_new("", ZSTR_LEN(data), 20);
    memcpy(ds->string, ZSTR_VAL(data), ZSTR_LEN(data));

    data_storage_append(d->storage, ds);

    data_string_free(ds);
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_lessram_dynamic_prepend_info, 0, 0, 1)
    ZEND_ARG_INFO(0, data)
ZEND_END_ARG_INFO()

/* {{{ proto void DynamicArray::prepend(mixed data) */
PHP_METHOD(DynamicArray, prepend)
{
    php_lessram_dynamic_t* d = php_lessram_dynamic_fetch(getThis());
    zval* data = NULL;
    zend_string* data_str = NULL;

    if(
        zend_parse_parameters_ex(
            ZEND_PARSE_PARAMS_QUIET, 
            ZEND_NUM_ARGS(), 
            "z", 
            &data
        ) 
        != 
        SUCCESS
    )
    {
        return;
    }

    if(Z_TYPE_P(data) == IS_ARRAY || Z_TYPE_P(data) == IS_OBJECT)
    {
        smart_str result = {0};
        php_json_encode(&result, data, 0);
        data_str = smart_str_extract(&result);

        DataString* ds = data_string_new("", ZSTR_LEN(data_str), 20);
        memcpy(ds->string, ZSTR_VAL(data_str), ZSTR_LEN(data_str));
        data_string_prepend_string(ds, "\0:", 2, 0);

        data_storage_prepend(d->storage, ds);

        smart_str_free(&result);
        zend_string_efree(data_str);
        data_string_free(ds);
    }
    else // TODO: throw exception if adding resources.
    {
        convert_to_string(data);
        data_str = Z_STR_P(data);

        DataString* ds = data_string_new("", ZSTR_LEN(data_str), 20);
        memcpy(ds->string, ZSTR_VAL(data_str), ZSTR_LEN(data_str));

        data_storage_prepend(d->storage, ds);

        data_string_free(ds);
    }
} /* }}} */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_lessram_dynamic_next_info, 0, 0, IS_STRING|IS_ARRAY, 0)
ZEND_END_ARG_INFO()

/* {{{ proto mixed DynamicArray::next(void) */
PHP_METHOD(DynamicArray, next)
{
    php_lessram_dynamic_t* d = php_lessram_dynamic_fetch(getThis());

    if (
        zend_parse_parameters_none() != SUCCESS
    ) {
        return;
    }

    DataString string = data_storage_get_next(d->storage);
    DataString* string_replace = NULL;

    if(string.string[0] == '\0' && string.string[1] == ':')
    {
        if(!d->storage->index && !d->storage->list)
        {
            string_replace = data_string_new(
                string.string+2, 
                string.len-2,
                0
            );

            data_string_replace(&SEPR_CHAR, &SEP_CHAR, string_replace);

            php_json_decode(
                return_value, 
                string_replace->string, 
                string_replace->len, 
                1, 
                512
            );
        }
        else
        {
            php_json_decode(
                return_value, 
                string.string+2, 
                string.len-2, 
                1, 
                512
            );
        }
    }
    else
    {
        if(!d->storage->index && !d->storage->list)
        {
            string_replace = data_string_new(
                string.string+2, 
                string.len-2,
                0
            );

            data_string_replace(&SEPR_CHAR, &SEP_CHAR, string_replace);

            RETVAL_STRINGL(string_replace->string, string_replace->len);
        }
        else
        {
            RETVAL_STRINGL(string.string, string.len);
        }
    }

    if(string_replace)
        data_string_free(string_replace);
} /* }}} */

/* {{{ */
const zend_function_entry php_lessram_dynamic_methods[] = {
    PHP_ME(DynamicArray, __construct,   php_lessram_dynamic_construct_info,     ZEND_ACC_CTOR|ZEND_ACC_PUBLIC)
    PHP_ME(DynamicArray, __destruct,    php_lessram_dynamic_destruct_info,      ZEND_ACC_DTOR|ZEND_ACC_PUBLIC)
    PHP_ME(DynamicArray, append,        php_lessram_dynamic_append_info,        ZEND_ACC_PUBLIC)
    PHP_ME(DynamicArray, appendData,    php_lessram_dynamic_append_data_info,   ZEND_ACC_PUBLIC)
    PHP_ME(DynamicArray, prepend,       php_lessram_dynamic_prepend_info,       ZEND_ACC_PUBLIC)
    PHP_ME(DynamicArray, next,          php_lessram_dynamic_next_info,          ZEND_ACC_PUBLIC)
    PHP_FE_END
}; /* }}} */

/* {{{ */
PHP_MINIT_FUNCTION(LessRam_DynamicArray)
{
    zend_class_entry ce;

    INIT_NS_CLASS_ENTRY(
        ce,
        "LessRam",
        "DynamicArray",
        php_lessram_dynamic_methods
    );

    lessramDynamic_ce = zend_register_internal_class(&ce);
    lessramDynamic_ce->create_object = php_lessram_dynamic_create;

    memcpy(
        &php_lessram_dynamic_handlers,
        zend_get_std_object_handlers(),
        sizeof(zend_object_handlers)
    );

    php_lessram_dynamic_handlers.offset = XtOffsetOf(
        php_lessram_dynamic_t,
        std
    );

    zend_declare_class_constant_long(
        lessramDynamic_ce, "LOWEST", 6, LOWEST
    );
    zend_declare_class_constant_long(
        lessramDynamic_ce, "MODERATE", 8, MODERATE
    );
    zend_declare_class_constant_long(
        lessramDynamic_ce, "HIGHEST", 7, HIGHEST
    );

    return SUCCESS;
} /* }}} */

#endif