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

static enum {
    LOWEST = 0,
    MODERATE = 1,
    HIGHEST = 2
} StorageSavings;

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

static zend_always_inline void php_lessram_storage_add_edit(
    DataStorage* storage, zval* data, bool prepend, bool edit, size_t position
)
{
    DataValue dv;

    if(Z_TYPE_P(data) == IS_ARRAY || Z_TYPE_P(data) == IS_OBJECT)
    {
        zend_string* data_str = NULL;
        smart_str result = {0};
        php_json_encode(&result, data, 0);
        data_str = smart_str_extract(&result);

        data_value_set_json_alloc(
            dv, ZSTR_VAL(data_str), ZSTR_LEN(data_str), 10
        );

        if(edit)
        {
            data_storage_edit(storage, position, dv);
        }
        else
        {
            if(prepend)
                data_storage_prepend(storage, dv);
            else
                data_storage_append(storage, dv);
        }

        smart_str_free(&result);
        zend_string_efree(data_str);

        return;
    }
    else
    {
        switch(Z_TYPE_P(data))
        {
            case IS_STRING:
            {
                data_value_set_string_alloc(
                    dv, 
                    Z_STRVAL_P(data), 
                    Z_STRLEN_P(data),
                    10
                );

                if(edit)
                {
                    data_storage_edit(storage, position, dv);
                }
                else
                {
                    if(prepend)
                    {
                        data_storage_prepend(storage, dv);
                    }
                    else
                    {
                        data_storage_append(storage, dv);
                    }
                }

                return;
            }
            case _IS_BOOL:
            {
                data_value_set_boolean(
                    dv, 
                    (bool) Z_LVAL_P(data)
                );
                break;
            }
            case IS_FALSE:
            {
                data_value_set_boolean(
                    dv, 
                    false
                );
                break;
            }
            case IS_TRUE:
            {
                data_value_set_boolean(
                    dv, 
                    true
                );
                break;
            }
            case IS_LONG:
            {
                data_value_set_integer(
                    dv, 
                    Z_LVAL_P(data)
                );
                break;
            }
            case IS_DOUBLE:
            {
                data_value_set_double(
                    dv, 
                    Z_DVAL_P(data)
                );
                break;
            }
            default:
            {
                // TODO: store as pointer?
                zend_throw_exception(
                    zend_ce_error_exception,
                    "Unsupported data type",
                    0
                );
                return;
            }
        }
    }

    if(edit)
    {
        data_storage_edit(storage, position, dv);
    }
    else
    {
        if(prepend)
            data_storage_prepend(storage, dv);
        else
            data_storage_append(storage, dv);
    }
}

static zend_always_inline void php_lessram_data_value_to_zval(
    const DataStorage* storage, const DataValue data, zval* value
)
{
    switch(data.type)
    {
        case DT_POINTER:
        {
            ZVAL_ZVAL(value, data.value.pointer.value, 0, 0);
            break;
        }
        case DT_INTEGER:
        {
            ZVAL_LONG(value, data.value.integer.value);
            break;
        }
        case DT_DOUBLE:
        {
            ZVAL_DOUBLE(value, data.value.double_float.value);
            break;
        }
        case DT_STRING:
        {
            if(!storage->list)
            {
                data_string_replace(
                    &SEP_CHAR, &SEPR_CHAR, data.value.string_alloc
                );
            }

            ZVAL_STRINGL(
                value, 
                data.value.string_alloc->string,
                data.value.string_alloc->len
            );

            data_value_free(data);

            break;
        }
        case DT_BOOLEAN:
        {
            ZVAL_BOOL(value, data.value.boolean.value);
            break;
        }
        case DT_JSON:
        {
            if(!storage->list)
            {
                data_string_replace(
                    &SEPR_CHAR, &SEP_CHAR, data.value.string_alloc
                );

                data_string_append(data.value.string_alloc, '\0', 0);
            }
            
            php_json_decode(
                value,
                data.value.string_alloc->string,
                data.value.string_alloc->len-1,
                1,
                PHP_JSON_PARSER_DEFAULT_DEPTH
            );

            data_value_free(data);

            break;
        }
    }
}

ZEND_BEGIN_ARG_INFO_EX(php_lessram_storage_construct_info, 0, 0, 0)
    ZEND_ARG_TYPE_INFO(0, savings, IS_LONG, 0)
ZEND_END_ARG_INFO()

/* {{{ proto Storage Storage::__construct(savings=Storage::LOWEST) */
PHP_METHOD(Storage, __construct)
{
    php_lessram_storage_t* s = php_lessram_storage_fetch(getThis());

    s->this = getThis();

    zend_long savings = LOWEST;

    ZEND_PARSE_PARAMETERS_START(0, 1)
        Z_PARAM_OPTIONAL
		Z_PARAM_LONG(savings)
	ZEND_PARSE_PARAMETERS_END();

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

    s->current = 0;

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

    ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(data)
	ZEND_PARSE_PARAMETERS_END();

    php_lessram_storage_add_edit(s->storage, data, false, false, 0);
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_lessram_storage_append_data_info, 0, 0, 1)
    ZEND_ARG_TYPE_INFO(0, data, IS_STRING, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void Storage::appendData(string data) */
PHP_METHOD(Storage, appendData)
{
    php_lessram_storage_t* s = php_lessram_storage_fetch(getThis());
    zend_string* data = NULL;

    ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(data)
	ZEND_PARSE_PARAMETERS_END();

    data_value_define_string(ds, ZSTR_VAL(data), ZSTR_LEN(data), 20);

    data_storage_append(s->storage, ds);
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_lessram_storage_prepend_info, 0, 0, 1)
    ZEND_ARG_INFO(0, data)
ZEND_END_ARG_INFO()

/* {{{ proto void Storage::prepend(mixed data) */
PHP_METHOD(Storage, prepend)
{
    php_lessram_storage_t* s = php_lessram_storage_fetch(getThis());
    zval* data = NULL;

    ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(data)
	ZEND_PARSE_PARAMETERS_END();

    php_lessram_storage_add_edit(s->storage, data, true, false, 0);
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_lessram_storage_prepend_data_info, 0, 0, 1)
    ZEND_ARG_TYPE_INFO(0, data, IS_STRING, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void Storage::prependData(string data) */
PHP_METHOD(Storage, prependData)
{
    php_lessram_storage_t* s = php_lessram_storage_fetch(getThis());
    zend_string* data = NULL;

    ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(data)
	ZEND_PARSE_PARAMETERS_END();

    data_value_define_string(ds, ZSTR_VAL(data), ZSTR_LEN(data), 20);

    data_storage_prepend(s->storage, ds);
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_lessram_storage_edit_info, 0, 0, 2)
    ZEND_ARG_TYPE_INFO(0, position, IS_LONG, 0)
    ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

/* {{{ proto void Storage::edit(int position, mixed value) */
PHP_METHOD(Storage, edit)
{
    php_lessram_storage_t* s = php_lessram_storage_fetch(getThis());

    zend_long position = 0;
    zval* value = NULL;

    ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_LONG(position)
        Z_PARAM_ZVAL(value)
	ZEND_PARSE_PARAMETERS_END();

    if(s->storage->list_size == 0 || position > s->storage->list_size)
    {
        zend_throw_exception(
            spl_ce_OutOfRangeException,
            "",
            0
        );

        return;
    }

    php_lessram_storage_add_edit(s->storage, value, false, true, position);
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_lessram_storage_remove_info, 0, 0, 1)
    ZEND_ARG_TYPE_INFO(0, position, IS_LONG, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void Storage::remove(int position) */
PHP_METHOD(Storage, remove)
{
    php_lessram_storage_t* s = php_lessram_storage_fetch(getThis());

    zend_long position = 0;

    ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(position)
	ZEND_PARSE_PARAMETERS_END();

    if(s->storage->list_size == 0 || position > s->storage->list_size)
    {
        zend_throw_exception(
            spl_ce_OutOfRangeException,
            "",
            0
        );

        return;
    }

    data_storage_remove(s->storage, position);
} /* }}} */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_lessram_storage_get_next_info, 0, 0, IS_STRING|IS_ARRAY, 0)
ZEND_END_ARG_INFO()

/* {{{ proto mixed Storage::getNext(void) */
PHP_METHOD(Storage, getNext)
{
    php_lessram_storage_t* s = php_lessram_storage_fetch(getThis());

    ZEND_PARSE_PARAMETERS_NONE();

    php_lessram_data_value_to_zval(
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

    ZEND_PARSE_PARAMETERS_NONE();

    data_storage_clear(s->storage);
} /* }}} */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_lessram_storage_count_info, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

/* {{{ proto int Storage::count(void) */
PHP_METHOD(Storage, count)
{
    php_lessram_storage_t* s = php_lessram_storage_fetch(getThis());

    ZEND_PARSE_PARAMETERS_NONE();

    RETURN_LONG(data_storage_size(s->storage))
} /* }}} */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_lessram_storage_offset_exists_info, 0, 1, _IS_BOOL, 0)
    ZEND_ARG_INFO(0, offset)
ZEND_END_ARG_INFO()

/* {{{ proto bool Storage::offsetExists(mixed offset) */
PHP_METHOD(Storage, offsetExists)
{
    php_lessram_storage_t* s = php_lessram_storage_fetch(getThis());

    zval* offset = NULL;

    ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(offset)
	ZEND_PARSE_PARAMETERS_END();

    if(Z_TYPE_P(offset) == IS_LONG)
    {
        zend_long offset_val = Z_LVAL_P(offset);
        zend_long size = s->storage->list_size;

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

    ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(offset)
	ZEND_PARSE_PARAMETERS_END();

    if(Z_TYPE_P(offset) == IS_LONG)
    {
        zend_long offset_val = Z_LVAL_P(offset);

        if(s->storage->list_size == 0 || offset_val > s->storage->list_size)
        {
            zend_throw_exception(
                spl_ce_OutOfRangeException,
                "",
                0
            );

            return;
        }

        php_lessram_data_value_to_zval(
            s->storage,
            data_storage_get(s->storage, offset_val),
            return_value
        );
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

    ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(offset)
        Z_PARAM_ZVAL(value)
	ZEND_PARSE_PARAMETERS_END();

    if(Z_TYPE_P(offset) == IS_NULL)
    {
        php_lessram_storage_add_edit(
            s->storage, value, false, false, 0
        );
    }
    else if(Z_TYPE_P(offset) == IS_LONG)
    {
        zend_long offset_val = Z_LVAL_P(offset);

        if(s->storage->list_size == 0 || offset_val > s->storage->list_size)
        {
            zend_throw_exception(
                spl_ce_OutOfRangeException,
                "",
                0
            );

            return;
        }

        php_lessram_storage_add_edit(
            s->storage, value, false, true, offset_val
        );
    }
    else if(Z_TYPE_P(offset) == IS_STRING)
    {
        zend_throw_exception(
            zend_ce_exception,
            "Not implemented yet",
            0
        );
    }
    else
    {
        zend_throw_exception(
            spl_ce_OutOfRangeException,
            "Only integer indexes supported",
            0
        );
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

    ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(offset)
	ZEND_PARSE_PARAMETERS_END();

    if(Z_TYPE_P(offset) == IS_LONG)
    {
        zend_long offset_val = Z_LVAL_P(offset);

        if(!data_storage_remove(s->storage, offset_val))
        {
            zend_throw_exception(
                spl_ce_OutOfRangeException,
                "",
                0
            );
        }
    }
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_lessram_storage_current_info, 0, 0, 0)
ZEND_END_ARG_INFO()

/* {{{ proto mixed Storage::current(void) */
PHP_METHOD(Storage, current)
{
    php_lessram_storage_t* s = php_lessram_storage_fetch(getThis());

    php_lessram_data_value_to_zval(
        s->storage,
        data_storage_get_current(s->storage),
        return_value
    );
} /* }}} */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_lessram_storage_key_info, 0, 0, IS_LONG|IS_STRING, 0)
ZEND_END_ARG_INFO()

/* {{{ proto ing|string Storage::key(void) */
PHP_METHOD(Storage, key)
{
    php_lessram_storage_t* s = php_lessram_storage_fetch(getThis());

    RETURN_LONG(s->storage->current_item);
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_lessram_storage_next_info, 0, 0, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void Storage::next(void) */
PHP_METHOD(Storage, next)
{
    php_lessram_storage_t* s = php_lessram_storage_fetch(getThis());

    s->current++;
    data_storage_forward(s->storage, s->current);
}
/* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_lessram_storage_rewind_info, 0, 0, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void Storage::rewind(void) */
PHP_METHOD(Storage, rewind)
{
    php_lessram_storage_t* s = php_lessram_storage_fetch(getThis());

    s->current = 0;
    data_storage_rewind(s->storage, 0);
} /* }}} */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_lessram_storage_valid_info, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

/* {{{ proto bool Storage::valid(void) */
PHP_METHOD(Storage, valid)
{
    php_lessram_storage_t* s = php_lessram_storage_fetch(getThis());

    RETURN_BOOL(s->storage->list_size > s->current);
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
        Storage, prependData,
        php_lessram_storage_prepend_data_info,
        ZEND_ACC_PUBLIC
    )
    PHP_ME(
        Storage, edit,
        php_lessram_storage_edit_info,
        ZEND_ACC_PUBLIC
    )
    PHP_ME(
        Storage, remove,
        php_lessram_storage_remove_info,
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
    PHP_ME(
        Storage, current,
        php_lessram_storage_current_info,
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