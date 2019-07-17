/**
 * @author Jefferson González
 * @license MIT
 * @link https://github.com/jgmdev/lessram Source code.
*/

#ifndef HAVE_PHP_LESSRAM_DYNAMIC
#define HAVE_PHP_LESSRAM_DYNAMIC

#include <src/datastore.h>

#include "php.h"
#include "php/Zend/zend_interfaces.h"
#include "php/Zend/zend_exceptions.h"
#include "php/ext/json/php_json.h"
#include "php/ext/standard/php_var.h"
#include "php/ext/spl/spl_iterators.h"
#include "php/ext/spl/spl_exceptions.h"
#include "php/Zend/zend_smart_str.h"

#include <classes/storage.h>

static DataString SEP_CHAR = {"~", 1, 0};
static DataString SEPR_CHAR = {"\\~", 2, 0};

static enum savings {
    LOWEST = 0,
    MODERATE = 1,
    HIGHEST = 2
};

zend_object_handlers php_lessram_storage_handlers;

zend_class_entry* lessramStorage_ce;

zend_object* php_lessram_storage_create(zend_class_entry* ce) {
    php_lessram_storage_t* s = (php_lessram_storage_t*)
        ecalloc(
            1,
            sizeof(php_lessram_storage_t) + zend_object_properties_size(ce)
        )
    ;

    zend_object_std_init(&s->std, ce);

    object_properties_init(&s->std, ce);

    s->std.handlers = &php_lessram_storage_handlers;

    return &s->std;
}

static void php_lessram_storage_append(DataStorage* storage, zval* data)
{
    zend_string* data_str = NULL;

    if(Z_TYPE_P(data) == IS_ARRAY || Z_TYPE_P(data) == IS_OBJECT)
    {
        smart_str result = {0};
        php_json_encode(&result, data, 0);
        data_str = smart_str_extract(&result);

        DataString* ds = data_string_new(
            ZSTR_VAL(data_str), ZSTR_LEN(data_str), 20
        );
        data_string_prepend_string(ds, "\0:", 2, 0);

        data_storage_append(storage, ds);

        smart_str_free(&result);
        zend_string_efree(data_str);
        data_string_free(ds);
    }
    else // TODO: throw exception if adding resources.
    {
        convert_to_string(data);
        data_str = Z_STR_P(data);

        DataString* ds = data_string_new(
            ZSTR_VAL(data_str), ZSTR_LEN(data_str), 20
        );

        data_storage_append(storage, ds);

        data_string_free(ds);
    }
}

static void php_lessram_storage_edit(
    DataStorage* storage, size_t position, zval* data
)
{
    zend_string* data_str = NULL;

    if(Z_TYPE_P(data) == IS_ARRAY || Z_TYPE_P(data) == IS_OBJECT)
    {
        smart_str result = {0};
        php_json_encode(&result, data, 0);
        data_str = smart_str_extract(&result);

        DataString* ds = data_string_new(
            ZSTR_VAL(data_str), ZSTR_LEN(data_str), 20
        );
        data_string_prepend_string(ds, "\0:", 2, 0);

        data_storage_edit(storage, position, ds);

        smart_str_free(&result);
        zend_string_efree(data_str);
        data_string_free(ds);
    }
    else // TODO: throw exception if adding resources.
    {
        convert_to_string(data);
        data_str = Z_STR_P(data);

        DataString* ds = data_string_new(
            ZSTR_VAL(data_str), ZSTR_LEN(data_str), 20
        );

        data_storage_edit(storage, position, ds);

        data_string_free(ds);
    }
}

static void php_lessram_data_string_to_zval(
    const DataStorage* storage, const DataString string, zval* value 
)
{
    DataString* string_replace = NULL;

    if(string.len > 2 && string.string[0] == '\0' && string.string[1] == ':')
    {
        string_replace = data_string_new(
            string.string+2,
            string.len-2,
            0
        );

        if(!storage->index && !storage->list)
        {
            data_string_replace(&SEPR_CHAR, &SEP_CHAR, string_replace);
        }
        
        php_json_decode(
            value,
            string_replace->string,
            string_replace->len,
            1,
            PHP_JSON_PARSER_DEFAULT_DEPTH
        );
    }
    else
    {
        if(!storage->index && !storage->list)
        {
            string_replace = data_string_new(
                string.string,
                string.len,
                0
            );

            data_string_replace(&SEPR_CHAR, &SEP_CHAR, string_replace);

            ZVAL_STRINGL(value, string_replace->string, string_replace->len);
        }
        else
        {
            ZVAL_STRINGL(value, string.string, string.len);
        }
    }

    if(string_replace)
        data_string_free(string_replace);
}

ZEND_BEGIN_ARG_INFO_EX(php_lessram_storage_construct_info, 0, 0, 0)
    ZEND_ARG_TYPE_INFO(0, savings, IS_LONG, 0)
ZEND_END_ARG_INFO()

/* {{{ proto Storage Storage::__construct(savings=Storage::LOWEST) */
PHP_METHOD(Storage, __construct)
{
    php_lessram_storage_t* s = php_lessram_storage_fetch(getThis());

    s->this = getThis();

    zend_long savings = 0;

    if(ZEND_NUM_ARGS() > 0)
    {
        if (
            zend_parse_parameters(
                ZEND_NUM_ARGS(),
                "L",
                &savings
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
            s->storage = data_storage_new();
            break;
        case MODERATE:
            s->storage = data_storage_new_with_index();
            break;
        default:
            s->storage = data_storage_new_with_index_and_list();
    }

} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_lessram_storage_destruct_info, 0, 0, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void Storage::__destruct() */
PHP_METHOD(Storage, __destruct)
{
    php_lessram_storage_t* s = php_lessram_storage_fetch(getThis());

    data_storage_free(s->storage);
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_lessram_storage_append_info, 0, 0, 1)
    ZEND_ARG_INFO(0, data)
ZEND_END_ARG_INFO()

/* {{{ proto void Storage::append(mixed data) */
PHP_METHOD(Storage, append)
{
    php_lessram_storage_t* s = php_lessram_storage_fetch(getThis());
    zval* data = NULL;

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

    php_lessram_storage_append(s->storage, data);
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_lessram_storage_append_data_info, 0, 0, 1)
    ZEND_ARG_TYPE_INFO(0, data, IS_STRING, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void Storage::appendData(string data) */
PHP_METHOD(Storage, appendData)
{
    php_lessram_storage_t* s = php_lessram_storage_fetch(getThis());
    zend_string* data = NULL;

    if (
        zend_parse_parameters_throw(
            ZEND_NUM_ARGS(),
            "S",
            &data
        )
        != SUCCESS
    ) {
        return;
    }

    convert_to_string(data);

    DataString* ds = data_string_new(ZSTR_VAL(data), ZSTR_LEN(data), 20);

    data_storage_append(s->storage, ds);

    data_string_free(ds);
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_lessram_storage_prepend_info, 0, 0, 1)
    ZEND_ARG_INFO(0, data)
ZEND_END_ARG_INFO()

/* {{{ proto void Storage::prepend(mixed data) */
PHP_METHOD(Storage, prepend)
{
    php_lessram_storage_t* s = php_lessram_storage_fetch(getThis());
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

        DataString* ds = data_string_new(
            ZSTR_VAL(data_str), ZSTR_LEN(data_str), 20
        );
        data_string_prepend_string(ds, "\0:", 2, 0);

        data_storage_prepend(s->storage, ds);

        smart_str_free(&result);
        zend_string_efree(data_str);
        data_string_free(ds);
    }
    else // TODO: throw exception if adding resources.
    {
        convert_to_string(data);
        data_str = Z_STR_P(data);

        DataString* ds = data_string_new(
            ZSTR_VAL(data_str), ZSTR_LEN(data_str), 20
        );

        data_storage_prepend(s->storage, ds);

        data_string_free(ds);
    }
} /* }}} */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_lessram_storage_get_next_info, 0, 0, IS_STRING|IS_ARRAY, 0)
ZEND_END_ARG_INFO()

/* {{{ proto mixed Storage::next(void) */
PHP_METHOD(Storage, getNext)
{
    php_lessram_storage_t* s = php_lessram_storage_fetch(getThis());

    if (
        zend_parse_parameters_none() != SUCCESS
    ) {
        zend_throw_exception(
            spl_ce_InvalidArgumentException,
            "No parameters required.",
            0
        );

        return;
    }

    php_lessram_data_string_to_zval(
        s->storage, 
        data_storage_get_next(s->storage),
        return_value
    );
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_lessram_storage_clear_info, 0, 0, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void Storage::clear(void) */
PHP_METHOD(Storage, clear)
{
    php_lessram_storage_t* s = php_lessram_storage_fetch(getThis());

    if (
        zend_parse_parameters_none() != SUCCESS
    ) {
        zend_throw_exception(
            spl_ce_InvalidArgumentException,
            "No parameters required.",
            0
        );

        return;
    }

    data_storage_clear(s->storage);
} /* }}} */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_lessram_storage_count_info, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

/* {{{ proto int Storage::count(void) */
PHP_METHOD(Storage, count)
{
    php_lessram_storage_t* s = php_lessram_storage_fetch(getThis());

    if (
        zend_parse_parameters_none() != SUCCESS
    ) {
        zend_throw_exception(
            spl_ce_InvalidArgumentException,
            "No parameters required.",
            0
        );

        return;
    }

    RETURN_LONG(data_storage_size(s->storage))
} /* }}} */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_lessram_storage_offset_exists_info, 0, 0, _IS_BOOL, 0)
    ZEND_ARG_INFO(0, offset)
ZEND_END_ARG_INFO()

/* {{{ proto bool Storage::offsetExists(mixed offset) */
PHP_METHOD(Storage, offsetExists)
{
    php_lessram_storage_t* s = php_lessram_storage_fetch(getThis());

    zval* offset = NULL;

    if(
        zend_parse_parameters_ex(
            ZEND_PARSE_PARAMS_QUIET,
            ZEND_NUM_ARGS(),
            "z",
            &offset
        )
        !=
        SUCCESS
    )
    {
        RETURN_BOOL(0)
    }

    if(Z_TYPE_P(offset) == IS_LONG)
    {
        zend_long offset_val = Z_LVAL_P(offset);
        zend_long size = data_storage_size(s->storage);

        if(size != 0)
        {
            RETURN_BOOL(
                offset_val >= 0
                &&
                offset_val < size
            )
        }
    }

    RETURN_BOOL(0)
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_lessram_storage_offset_get_info, 0, 0, 1)
    ZEND_ARG_INFO(0, offset)
ZEND_END_ARG_INFO()

/* {{{ proto mixed Storage::offsetGet(mixed offset) */
PHP_METHOD(Storage, offsetGet)
{
    php_lessram_storage_t* s = php_lessram_storage_fetch(getThis());

    zval* offset = NULL;

    if(
        zend_parse_parameters_ex(
            ZEND_PARSE_PARAMS_QUIET,
            ZEND_NUM_ARGS(),
            "z",
            &offset
        )
        !=
        SUCCESS
    )
    {
        RETURN_BOOL(0)
    }

    if(Z_TYPE_P(offset) == IS_LONG)
    {
        zend_long offset_val = Z_LVAL_P(offset);

        if(s->storage->list_size != 0 && offset_val < s->storage->list_size)
        {
            php_lessram_data_string_to_zval(
                s->storage, 
                data_storage_get(s->storage, offset_val),
                return_value
            );

            return;
        }
    }

    RETURN_NULL()
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_lessram_storage_offset_set_info, 0, 0, 2)
    ZEND_ARG_INFO(0, offset)
    ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

/* {{{ proto void Storage::offsetSet(mixed offset, mixed value) */
PHP_METHOD(Storage, offsetSet)
{
    php_lessram_storage_t* s = php_lessram_storage_fetch(getThis());

    zval* offset = NULL;
    zval* value = NULL;

    if(
        zend_parse_parameters_throw(
            ZEND_NUM_ARGS(),
            "zz",
            &offset,
            &value
        )
        !=
        SUCCESS
    )
    {
        return;
    }

    if(Z_TYPE_P(offset) == IS_NULL)
    {
        php_lessram_storage_append(s->storage, value);
    }
    else if(Z_TYPE_P(offset) == IS_LONG)
    {
        zend_long offset_val = Z_LVAL_P(offset);

        php_lessram_storage_edit(s->storage, offset_val, value);
    }
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_lessram_storage_offset_unset_info, 0, 0, 1)
    ZEND_ARG_INFO(0, offset)
ZEND_END_ARG_INFO()

/* {{{ proto void Storage::offsetUnset(mixed offset) */
PHP_METHOD(Storage, offsetUnset)
{
    php_lessram_storage_t* s = php_lessram_storage_fetch(getThis());

    zval* offset = NULL;

    if(
        zend_parse_parameters_throw(
            ZEND_NUM_ARGS(),
            "z",
            &offset
        )
        !=
        SUCCESS
    )
    {
        return;
    }

    if(Z_TYPE_P(offset) == IS_LONG)
    {
        zend_long offset_val = Z_LVAL_P(offset);

        data_storage_remove(s->storage, offset_val);
    }
} /* }}} */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_lessram_storage_key_info, 0, 0, IS_LONG|IS_STRING, 0)
ZEND_END_ARG_INFO()

/* {{{ proto ing|string Storage::key(void) */
PHP_METHOD(Storage, key)
{
    php_lessram_storage_t* s = php_lessram_storage_fetch(getThis());

    if (
        zend_parse_parameters_none() != SUCCESS
    ) {
        zend_throw_exception(
            spl_ce_InvalidArgumentException,
            "No parameters required.",
            0
        );

        return;
    }

    RETURN_LONG(s->storage->current_item);
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_lessram_storage_next_info, 0, 0, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void Storage::next(void) */
PHP_METHOD(Storage, next){} 
/* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_lessram_storage_rewind_info, 0, 0, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void Storage::rewind(void) */
PHP_METHOD(Storage, rewind)
{
    php_lessram_storage_t* s = php_lessram_storage_fetch(getThis());

    if (
        zend_parse_parameters_none() != SUCCESS
    ) {
        zend_throw_exception(
            spl_ce_InvalidArgumentException,
            "No parameters required.",
            0
        );

        return;
    }

    data_storage_rewind(s->storage);
} /* }}} */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_lessram_storage_valid_info, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

/* {{{ proto bool Storage::valid(void) */
PHP_METHOD(Storage, valid)
{
    php_lessram_storage_t* s = php_lessram_storage_fetch(getThis());

    if (
        zend_parse_parameters_none() != SUCCESS
    ) {
        zend_throw_exception(
            spl_ce_InvalidArgumentException,
            "No parameters required.",
            0
        );

        return;
    }

    RETURN_BOOL(s->storage->list_size > s->storage->current_item);
} /* }}} */

/* {{{ */
const zend_function_entry php_lessram_storage_methods[] = {
    PHP_ME(
        Storage, __construct,   
        php_lessram_storage_construct_info,     
        ZEND_ACC_CTOR|ZEND_ACC_PUBLIC
    )
    PHP_ME(
        Storage, __destruct,    
        php_lessram_storage_destruct_info,      
        ZEND_ACC_DTOR|ZEND_ACC_PUBLIC
    )
    /* Core Methods */
    PHP_ME(
        Storage, append,        
        php_lessram_storage_append_info,        
        ZEND_ACC_PUBLIC
    )
    PHP_ME(
        Storage, appendData,    
        php_lessram_storage_append_data_info,   
        ZEND_ACC_PUBLIC
    )
    PHP_ME(
        Storage, prepend,       
        php_lessram_storage_prepend_info,       
        ZEND_ACC_PUBLIC
    )
    PHP_ME(
        Storage, getNext,       
        php_lessram_storage_get_next_info,      
        ZEND_ACC_PUBLIC
    )
    PHP_ME(
        Storage, clear,         
        php_lessram_storage_clear_info,         
        ZEND_ACC_PUBLIC
    )
    /* Countable Methods */
    PHP_ME(
        Storage, count,         
        php_lessram_storage_count_info,         
        ZEND_ACC_PUBLIC
    )
    /* ArrayAccess Methods */
    PHP_ME(
        Storage, offsetExists,         
        php_lessram_storage_offset_exists_info,         
        ZEND_ACC_PUBLIC
    )
    PHP_ME(
        Storage, offsetGet,         
        php_lessram_storage_offset_get_info,         
        ZEND_ACC_PUBLIC
    )
    PHP_ME(
        Storage, offsetSet,         
        php_lessram_storage_offset_set_info,         
        ZEND_ACC_PUBLIC
    )
    PHP_ME(
        Storage, offsetUnset,         
        php_lessram_storage_offset_unset_info,         
        ZEND_ACC_PUBLIC
    )
    /* Iterator Methods */
    PHP_MALIAS(
        Storage, current, getNext, 
        php_lessram_storage_get_next_info,      
        ZEND_ACC_PUBLIC
    )
    PHP_ME(
        Storage, key,           
        php_lessram_storage_key_info,           
        ZEND_ACC_PUBLIC
    )
    PHP_ME(
        Storage, next,          
        php_lessram_storage_next_info,          
        ZEND_ACC_PUBLIC
    )
    PHP_ME(
        Storage, rewind,        
        php_lessram_storage_rewind_info,        
        ZEND_ACC_PUBLIC
    )
    PHP_ME(
        Storage, valid,         
        php_lessram_storage_valid_info,         
        ZEND_ACC_PUBLIC
    )
    PHP_FE_END
}; /* }}} */

/* {{{ */
PHP_MINIT_FUNCTION(LessRam_Storage)
{
    zend_class_entry ce;

    INIT_NS_CLASS_ENTRY(
        ce,
        "LessRam",
        "Storage",
        php_lessram_storage_methods
    );

    lessramStorage_ce = zend_register_internal_class(&ce);
    lessramStorage_ce->create_object = php_lessram_storage_create;

    memcpy(
        &php_lessram_storage_handlers,
        zend_get_std_object_handlers(),
        sizeof(zend_object_handlers)
    );

    php_lessram_storage_handlers.offset = XtOffsetOf(
        php_lessram_storage_t,
        std
    );

    zend_class_implements(lessramStorage_ce, 3,
        spl_ce_ArrayAccess,
        spl_ce_Countable,
        spl_ce_Iterator
        //spl_ce_Serializable
    );

    zend_declare_class_constant_long(
        lessramStorage_ce, ZEND_STRL("LOWEST"), LOWEST
    );
    zend_declare_class_constant_long(
        lessramStorage_ce, ZEND_STRL("MODERATE"), MODERATE
    );
    zend_declare_class_constant_long(
        lessramStorage_ce, ZEND_STRL("HIGHEST"), HIGHEST
    );

    return SUCCESS;
} /* }}} */

#endif