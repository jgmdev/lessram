/**
 * @author Jefferson González
 * @license MIT
 * @link https://github.com/jgmdev/lessram Source code.
*/

#ifndef HAVE_PHP_LESSRAM_LIST
#define HAVE_PHP_LESSRAM_LIST

#include <src/datastoresimple.h>

#include "php.h"
#include "php/Zend/zend_interfaces.h"
#include "php/Zend/zend_exceptions.h"
#include "php/ext/json/php_json.h"
#include "php/ext/standard/php_var.h"
#include "php/ext/spl/spl_iterators.h"
#include "php/ext/spl/spl_exceptions.h"
#include "php/Zend/zend_smart_str.h"

#include <classes/list.h>

zend_object_handlers php_lessram_list_handlers;

zend_class_entry* lessramList_ce;

zend_object* php_lessram_list_create(zend_class_entry* ce) {
    php_lessram_list_t* s = (php_lessram_list_t*)
        ecalloc(
            1,
            sizeof(php_lessram_list_t) + zend_object_properties_size(ce)
        )
    ;

    zend_object_std_init(&s->std, ce);

    object_properties_init(&s->std, ce);

    s->std.handlers = &php_lessram_list_handlers;

    return &s->std;
}

static zend_always_inline void php_lessram_list_append(
    DataStorageSimple* storage, 
    zval* data
)
{
    if(Z_TYPE_P(data) == IS_ARRAY || Z_TYPE_P(data) == IS_OBJECT)
    {
        zend_string* data_str = NULL;
        smart_str result = {0};
        php_json_encode(&result, data, 0);
        data_str = smart_str_extract(&result);
        
        data_storage_simple_append_json(
            storage, 
            ZSTR_VAL(data_str),
            ZSTR_LEN(data_str)
        );

        smart_str_free(&result);
        zend_string_efree(data_str);

        return;
    }

    switch(Z_TYPE_P(data))
    {
        case IS_STRING:
        {
            data_storage_simple_append_bytes(
                storage, 
                Z_STRVAL_P(data), 
                Z_STRLEN_P(data)
            );

            return;
        }
        case IS_FALSE:
        {
            data_storage_simple_append_bool(
                storage,
                false
            );

            return;
        }
        case IS_TRUE:
        {
            data_storage_simple_append_bool(
                storage,
                true
            );

            return;
        }
        case IS_LONG:
        {
            data_storage_simple_append_int(
                storage,
                Z_LVAL_P(data)
            );

            return;
        }
        case IS_DOUBLE:
        {
            data_storage_simple_append_double(
                storage,
                Z_DVAL_P(data)
            );

            return;
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

static zend_always_inline void php_lessram_list_prepend(
    DataStorageSimple* storage, 
    zval* data
)
{
    if(Z_TYPE_P(data) == IS_ARRAY || Z_TYPE_P(data) == IS_OBJECT)
    {
        zend_string* data_str = NULL;
        smart_str result = {0};
        php_json_encode(&result, data, 0);
        data_str = smart_str_extract(&result);
        
        data_storage_simple_prepend_json(
            storage, 
            ZSTR_VAL(data_str),
            ZSTR_LEN(data_str)
        );

        smart_str_free(&result);
        zend_string_efree(data_str);

        return;
    }

    switch(Z_TYPE_P(data))
    {
        case IS_STRING:
        {
            data_storage_simple_prepend_bytes(
                storage, 
                Z_STRVAL_P(data), 
                Z_STRLEN_P(data)
            );

            return;
        }
        case IS_FALSE:
        {
            data_storage_simple_prepend_bool(
                storage,
                false
            );

            return;
        }
        case IS_TRUE:
        {
            data_storage_simple_prepend_bool(
                storage,
                true
            );

            return;
        }
        case IS_LONG:
        {
            data_storage_simple_prepend_int(
                storage,
                Z_LVAL_P(data)
            );

            return;
        }
        case IS_DOUBLE:
        {
            data_storage_simple_prepend_double(
                storage,
                Z_DVAL_P(data)
            );

            return;
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

static zend_always_inline void php_lessram_list_edit(
    DataStorageSimple* storage, 
    zval* data, 
    size_t position
)
{
    if(Z_TYPE_P(data) == IS_ARRAY || Z_TYPE_P(data) == IS_OBJECT)
    {
        zend_string* data_str = NULL;
        smart_str result = {0};
        php_json_encode(&result, data, 0);
        data_str = smart_str_extract(&result);
        
        data_storage_simple_edit_json(
            storage, 
            position,
            ZSTR_VAL(data_str),
            ZSTR_LEN(data_str)
        );

        smart_str_free(&result);
        zend_string_efree(data_str);

        return;
    }

    switch(Z_TYPE_P(data))
    {
        case IS_STRING:
        {
            data_storage_simple_edit_bytes(
                storage, 
                position,
                Z_STRVAL_P(data), 
                Z_STRLEN_P(data)
            );

            return;
        }
        case IS_FALSE:
        {
            data_storage_simple_edit_bool(
                storage,
                position,
                false
            );

            return;
        }
        case IS_TRUE:
        {
            data_storage_simple_edit_bool(
                storage,
                position,
                true
            );

            return;
        }
        case IS_LONG:
        {
            data_storage_simple_edit_int(
                storage,
                position,
                Z_LVAL_P(data)
            );

            return;
        }
        case IS_DOUBLE:
        {
            data_storage_simple_edit_double(
                storage,
                position,
                Z_DVAL_P(data)
            );

            return;
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

static zend_always_inline void php_lessram_data_value_to_zval(
    const DataStorageSimple* storage, 
    const DataStoreSimpleElement* data, 
    zval* value
)
{
    if(!data)
    {
        ZVAL_NULL(value);
        return;
    }

    switch(data->type)
    {
        case DT_POINTER:
        {
            ZVAL_ZVAL(value, data->data, 0, 0);
            break;
        }
        case DT_INTEGER:
        {
            DataStoreSimpleUnion converter = {.pointer=data->data};
            ZVAL_LONG(value, converter.integer);
            break;
        }
        case DT_DOUBLE:
        {
            DataStoreSimpleUnion converter = {.pointer=data->data};
            ZVAL_DOUBLE(value, converter.double_float);
            break;
        }
        case DT_STRING:
        {
            DataString* data_string = data->data;

            ZVAL_STRINGL(
                value, 
                data_string->string,
                data_string->len
            );

            break;
        }
        case DT_BOOLEAN:
        {
            DataStoreSimpleUnion converter = {.pointer = data->data};
            ZVAL_BOOL(value, converter.boolean);
            break;
        }
        case DT_JSON:
        {
            DataString* data_string = data->data;

            php_json_decode(
                value,
                data_string->string,
                data_string->len,
                1,
                PHP_JSON_PARSER_DEFAULT_DEPTH
            );

            break;
        }
    }
}

ZEND_BEGIN_ARG_INFO_EX(php_lessram_list_construct_info, 0, 0, 0)
    ZEND_ARG_TYPE_INFO(0, stack, IS_LONG, 0)
ZEND_END_ARG_INFO()

/* {{{ proto List List::__construct(int stack) */
PHP_METHOD(List, __construct)
{
    php_lessram_list_t* s = php_lessram_list_fetch(getThis());

    zend_long stack = 30;

    ZEND_PARSE_PARAMETERS_START(0, 1)
        Z_PARAM_OPTIONAL
		Z_PARAM_LONG(stack)
	ZEND_PARSE_PARAMETERS_END();

    s->storage = data_storage_simple_new(stack);
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_lessram_list_destruct_info, 0, 0, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void List::__destruct() */
PHP_METHOD(List, __destruct)
{
    php_lessram_list_t* s = php_lessram_list_fetch(getThis());

    data_storage_simple_free(s->storage);
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_lessram_list_append_info, 0, 0, 1)
    ZEND_ARG_INFO(0, data)
ZEND_END_ARG_INFO()

/* {{{ proto void List::append(mixed data) */
PHP_METHOD(List, append)
{
    php_lessram_list_t* s = php_lessram_list_fetch(getThis());
    zval* data = NULL;

    ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(data)
	ZEND_PARSE_PARAMETERS_END();

    php_lessram_list_append(s->storage, data);
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_lessram_list_prepend_info, 0, 0, 1)
    ZEND_ARG_INFO(0, data)
ZEND_END_ARG_INFO()

/* {{{ proto void List::prepend(mixed data) */
PHP_METHOD(List, prepend)
{
    php_lessram_list_t* s = php_lessram_list_fetch(getThis());
    zval* data = NULL;

    ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(data)
	ZEND_PARSE_PARAMETERS_END();

    php_lessram_list_prepend(s->storage, data);
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_lessram_list_edit_info, 0, 0, 2)
    ZEND_ARG_TYPE_INFO(0, position, IS_LONG, 0)
    ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

/* {{{ proto void List::edit(int position, mixed value) */
PHP_METHOD(List, edit)
{
    php_lessram_list_t* s = php_lessram_list_fetch(getThis());

    zend_long position = 0;
    zval* value = NULL;

    ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_LONG(position)
        Z_PARAM_ZVAL(value)
	ZEND_PARSE_PARAMETERS_END();

    if(
        s->storage->count == 0 
        || 
        position > s->storage->count 
        || 
        (int) position < 0
    )
    {
        zend_throw_exception(
            spl_ce_OutOfRangeException,
            "",
            0
        );

        return;
    }

    php_lessram_list_edit(s->storage, value, position);
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_lessram_list_remove_info, 0, 0, 1)
    ZEND_ARG_TYPE_INFO(0, position, IS_LONG, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void List::remove(int position) */
PHP_METHOD(List, remove)
{
    php_lessram_list_t* s = php_lessram_list_fetch(getThis());

    zend_long position = 0;

    ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(position)
	ZEND_PARSE_PARAMETERS_END();

    if(
        s->storage->count == 0 
        || 
        position > s->storage->count
        ||
        (int) position < 0
    )
    {
        zend_throw_exception(
            spl_ce_OutOfRangeException,
            "",
            0
        );

        return;
    }

    data_storage_simple_remove(s->storage, position);
} /* }}} */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_lessram_list_get_next_info, 0, 0, IS_STRING|IS_ARRAY, 0)
ZEND_END_ARG_INFO()

/* {{{ proto mixed List::getNext(void) */
PHP_METHOD(List, getNext)
{
    php_lessram_list_t* s = php_lessram_list_fetch(getThis());

    php_lessram_data_value_to_zval(
        s->storage,
        data_storage_simple_get_next(s->storage),
        return_value
    );
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_lessram_list_clear_info, 0, 0, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void List::clear(void) */
PHP_METHOD(List, clear)
{
    php_lessram_list_t* s = php_lessram_list_fetch(getThis());

    ZEND_PARSE_PARAMETERS_NONE();

    data_storage_simple_clear(s->storage);
} /* }}} */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_lessram_list_count_info, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

/* {{{ proto int List::count(void) */
PHP_METHOD(List, count)
{
    php_lessram_list_t* s = php_lessram_list_fetch(getThis());

    RETURN_LONG(s->storage->count)
} /* }}} */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_lessram_list_offset_exists_info, 0, 1, _IS_BOOL, 0)
    ZEND_ARG_INFO(0, offset)
ZEND_END_ARG_INFO()

/* {{{ proto bool List::offsetExists(mixed offset) */
PHP_METHOD(List, offsetExists)
{
    php_lessram_list_t* s = php_lessram_list_fetch(getThis());

    zval* offset = NULL;

    ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(offset)
	ZEND_PARSE_PARAMETERS_END();

    if(Z_TYPE_P(offset) == IS_LONG)
    {
        zend_long offset_val = Z_LVAL_P(offset);
        zend_long size = s->storage->count;

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

ZEND_BEGIN_ARG_INFO_EX(php_lessram_list_offset_get_info, 0, 0, 1)
    ZEND_ARG_INFO(0, offset)
ZEND_END_ARG_INFO()

/* {{{ proto mixed List::offsetGet(mixed offset) */
PHP_METHOD(List, offsetGet)
{
    php_lessram_list_t* s = php_lessram_list_fetch(getThis());

    zval* offset = NULL;

    ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(offset)
	ZEND_PARSE_PARAMETERS_END();

    if(Z_TYPE_P(offset) == IS_LONG)
    {
        zend_long offset_val = Z_LVAL_P(offset);

        if(s->storage->count == 0 || offset_val > s->storage->count)
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
            (s->storage->elements+offset_val),
            return_value
        );

        return;
    }

    RETURN_NULL()
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_lessram_list_offset_set_info, 0, 0, 2)
    ZEND_ARG_INFO(0, offset)
    ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

/* {{{ proto void List::offsetSet(mixed offset, mixed value) */
PHP_METHOD(List, offsetSet)
{
    php_lessram_list_t* s = php_lessram_list_fetch(getThis());

    zval* offset = NULL;
    zval* value = NULL;

    ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(offset)
        Z_PARAM_ZVAL(value)
	ZEND_PARSE_PARAMETERS_END();

    if(Z_TYPE_P(offset) == IS_NULL)
    {
        php_lessram_list_append(s->storage, value);
    }
    else if(Z_TYPE_P(offset) == IS_LONG)
    {
        zend_long offset_val = Z_LVAL_P(offset);

        if(
            s->storage->count == 0 
            || 
            offset_val > s->storage->count
            ||
            (int) offset_val < 0
        )
        {
            zend_throw_exception(
                spl_ce_OutOfRangeException,
                "",
                0
            );

            return;
        }

        php_lessram_list_edit(s->storage, value, offset_val);
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

ZEND_BEGIN_ARG_INFO_EX(php_lessram_list_offset_unset_info, 0, 0, 1)
    ZEND_ARG_INFO(0, offset)
ZEND_END_ARG_INFO()

/* {{{ proto void List::offsetUnset(mixed offset) */
PHP_METHOD(List, offsetUnset)
{
    php_lessram_list_t* s = php_lessram_list_fetch(getThis());

    zval* offset = NULL;

    ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(offset)
	ZEND_PARSE_PARAMETERS_END();

    if(Z_TYPE_P(offset) == IS_LONG)
    {
        zend_long offset_val = Z_LVAL_P(offset);

        if(!data_storage_simple_remove(s->storage, offset_val))
        {
            zend_throw_exception(
                spl_ce_OutOfRangeException,
                "",
                0
            );
        }
    }
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_lessram_list_current_info, 0, 0, 0)
ZEND_END_ARG_INFO()

/* {{{ proto mixed List::current(void) */
PHP_METHOD(List, current)
{
    php_lessram_list_t* s = php_lessram_list_fetch(getThis());

    php_lessram_data_value_to_zval(
        s->storage,
        (s->storage->elements+s->storage->current),
        return_value
    );
} /* }}} */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_lessram_list_key_info, 0, 0, IS_LONG|IS_STRING, 0)
ZEND_END_ARG_INFO()

/* {{{ proto ing|string List::key(void) */
PHP_METHOD(List, key)
{
    php_lessram_list_t* s = php_lessram_list_fetch(getThis());

    RETURN_LONG(s->storage->current);
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_lessram_list_next_info, 0, 0, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void List::next(void) */
PHP_METHOD(List, next)
{
    php_lessram_list_t* s = php_lessram_list_fetch(getThis());

    s->storage->current++;
}
/* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_lessram_list_rewind_info, 0, 0, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void List::rewind(void) */
PHP_METHOD(List, rewind)
{
    php_lessram_list_t* s = php_lessram_list_fetch(getThis());

    s->storage->current = 0;
} /* }}} */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_lessram_list_valid_info, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

/* {{{ proto bool List::valid(void) */
PHP_METHOD(List, valid)
{
    php_lessram_list_t* s = php_lessram_list_fetch(getThis());

    RETURN_BOOL(s->storage->count > s->storage->current);
} /* }}} */

/* {{{ */
const zend_function_entry php_lessram_list_methods[] = {
    PHP_ME(
        List, __construct,
        php_lessram_list_construct_info,
        ZEND_ACC_CTOR|ZEND_ACC_PUBLIC
    )
    PHP_ME(
        List, __destruct,
        php_lessram_list_destruct_info,
        ZEND_ACC_DTOR|ZEND_ACC_PUBLIC
    )
    /* Core Methods */
    PHP_ME(
        List, append,
        php_lessram_list_append_info,
        ZEND_ACC_PUBLIC
    )
    PHP_ME(
        List, prepend,
        php_lessram_list_prepend_info,
        ZEND_ACC_PUBLIC
    )
    PHP_ME(
        List, edit,
        php_lessram_list_edit_info,
        ZEND_ACC_PUBLIC
    )
    PHP_ME(
        List, remove,
        php_lessram_list_remove_info,
        ZEND_ACC_PUBLIC
    )
    PHP_ME(
        List, getNext,
        php_lessram_list_get_next_info,
        ZEND_ACC_PUBLIC
    )
    PHP_ME(
        List, clear,
        php_lessram_list_clear_info,
        ZEND_ACC_PUBLIC
    )
    /* Countable Methods */
    PHP_ME(
        List, count,
        php_lessram_list_count_info,
        ZEND_ACC_PUBLIC
    )
    /* ArrayAccess Methods */
    PHP_ME(
        List, offsetExists,
        php_lessram_list_offset_exists_info,
        ZEND_ACC_PUBLIC
    )
    PHP_ME(
        List, offsetGet,
        php_lessram_list_offset_get_info,
        ZEND_ACC_PUBLIC
    )
    PHP_ME(
        List, offsetSet,
        php_lessram_list_offset_set_info,
        ZEND_ACC_PUBLIC
    )
    PHP_ME(
        List, offsetUnset,
        php_lessram_list_offset_unset_info,
        ZEND_ACC_PUBLIC
    )
    /* Iterator Methods */
    PHP_ME(
        List, current,
        php_lessram_list_current_info,
        ZEND_ACC_PUBLIC
    )
    PHP_ME(
        List, key,
        php_lessram_list_key_info,
        ZEND_ACC_PUBLIC
    )
    PHP_ME(
        List, next,
        php_lessram_list_next_info,
        ZEND_ACC_PUBLIC
    )
    PHP_ME(
        List, rewind,
        php_lessram_list_rewind_info,
        ZEND_ACC_PUBLIC
    )
    PHP_ME(
        List, valid,
        php_lessram_list_valid_info,
        ZEND_ACC_PUBLIC
    )
    PHP_FE_END
}; /* }}} */

/* {{{ */
PHP_MINIT_FUNCTION(LessRam_List)
{
    zend_class_entry ce;

    INIT_NS_CLASS_ENTRY(
        ce,
        "LessRam",
        "ListStore",
        php_lessram_list_methods
    );

    lessramList_ce = zend_register_internal_class(&ce);
    lessramList_ce->create_object = php_lessram_list_create;

    memcpy(
        &php_lessram_list_handlers,
        zend_get_std_object_handlers(),
        sizeof(zend_object_handlers)
    );

    php_lessram_list_handlers.offset = XtOffsetOf(
        php_lessram_list_t,
        std
    );

    zend_class_implements(lessramList_ce, 3,
        spl_ce_ArrayAccess,
        spl_ce_Countable,
        spl_ce_Iterator
        //spl_ce_Serializable
    );

    return SUCCESS;
} /* }}} */

#endif