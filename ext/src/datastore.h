/**
 * @author Jefferson González
 * @license MIT
 * @link https://github.com/jgmdev/lessram Source code.
*/

#ifndef DATA_STORE_NATIVE_H
#define DATA_STORE_NATIVE_H

#include <stddef.h>
#include <stdbool.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef USE_RP_MALLOC
    #include "rpmalloc/rpmalloc.h"
    #define dsmalloc(bytes) rpmalloc(bytes)
    #define dsrealloc(pointer, bytes) rprealloc(pointer, bytes)
    #define dsfree(pointer) rpfree(pointer)
#elif defined(USE_C_MALLOC)
    #include <stdlib.h>
    #define dsmalloc(bytes) malloc(bytes)
    #define dsrealloc(pointer, bytes) realloc(pointer, bytes)
    #define dsfree(pointer) free(pointer)
#else
    #include "php.h"
    #define dsmalloc(bytes) emalloc(bytes)
    #define dsrealloc(pointer, bytes) erealloc(pointer, bytes)
    #define dsfree(pointer) efree(pointer)
#endif

#define DS_STR(s) s, sizeof(s)-1

typedef struct _DataIndex {
    size_t* start;
    size_t* end;
    size_t count;
    size_t buffer;
    size_t buffer_left;
    size_t buffer_next;
} DataIndex;

typedef struct _DataList {
    void** items;
    size_t count;
    size_t buffer;
    size_t buffer_left;
    size_t buffer_next;
} DataList;

typedef struct _DataString {
    char* string;
    size_t len;
    size_t buffer;
} DataString;

typedef struct _DataStorage {
    DataString* elements;

    DataIndex* index;
    DataList* list;

    size_t list_size;
    size_t current_item;
    size_t current_position;
} DataStorage;

typedef enum _DataType {
    DT_POINTER  = 'P',
    DT_INTEGER  = 'I',
    DT_DOUBLE   = 'D',
    DT_STRING   = 'S',
    DT_BOOLEAN  = 'B',
    DT_JSON     = 'J'
} DataType;

typedef union _DataInteger {
    char bytes[sizeof(size_t)];
    size_t value;
} DataInteger;

typedef union _DataDouble {
    char bytes[sizeof(double)];
    double value;
} DataDouble;

typedef union _DataPointer {
    char bytes[sizeof(void*)];
    void* value;
} DataPointer;

typedef union _DataBool {
    char bytes[sizeof(bool)];
    bool value;
} DataBool;

typedef union _DataValues {
    DataInteger integer;
    DataDouble double_float;
    DataPointer pointer;
    DataBool boolean;
    DataString string;
    DataString* string_alloc;
} DataValues;

typedef struct _DataValue {
    DataValues value;
    DataType type;
    bool alloc_str;
} DataValue;

#define data_value_free(name) \
    if(name.alloc_str){ \
        data_string_free(name.value.string_alloc); \
    }
    
#define data_value_define_string(name, val, size, buf) \
    DataValue name; \
    name.type = DT_STRING; \
    name.alloc_str = true; \
    name.value.string_alloc = data_string_new(val, size, buf+1); \
    name.value.string_alloc->string[size] = '\0'

#define data_value_define_json(name, val, size, buf) \
    DataValue name; \
    name.type = DT_JSON; \
    name.alloc_str = true; \
    name.value.string_alloc = data_string_new(val, size, buf)

#define data_value_define_integer(name, val) \
    DataValue name; \
    name.type = DT_INTEGER; \
    name.alloc_str = false; \
    name.value.integer.value = val

#define data_value_define_double(name, val) \
    DataValue name; \
    name.type = DT_DOUBLE; \
    name.alloc_str = false; \
    name.value.double_float.value = val

#define data_value_define_pointer(name, val) \
    DataValue name; \
    name.type = DT_POINTER; \
    name.alloc_str = false; \
    name.value.pointer.value = val

#define data_value_define_boolean(name, val) \
    DataValue name; \
    name.type = DT_BOOLEAN; \
    name.alloc_str = false; \
    name.value.boolean.value = val

#define data_value_set_string(name, val, size, buf) \
    name.type = DT_STRING; \
    name.alloc_str = false; \
    name.value.string.string = val; \
    name.value.string.len = size; \
    name.value.string.buffer = buf

#define data_value_set_string_alloc(name, val, size, buf) \
    name.type = DT_STRING; \
    name.alloc_str = true; \
    name.value.string_alloc = data_string_new(val, size, buf+1); \
    name.value.string_alloc->string[size] = '\0'

#define data_value_set_json(name, val, size, buf) \
    name.type = DT_JSON; \
    name.alloc_str = false; \
    name.value.string.string = val; \
    name.value.string.len = size; \
    name.value.string.buffer = buf

#define data_value_set_json_alloc(name, val, size, buf) \
    name.type = DT_JSON; \
    name.alloc_str = true; \
    name.value.string_alloc = data_string_new(val, size, buf+1); \
    name.value.string_alloc->string[size] = '\0'

#define data_value_set_integer(name, val) \
    name.type = DT_INTEGER; \
    name.alloc_str = false; \
    name.value.integer.value = val

#define data_value_set_double(name, val) \
    name.type = DT_DOUBLE; \
    name.alloc_str = false; \
    name.value.double_float.value = val

#define data_value_set_pointer(name, val) \
    name.type = DT_POINTER; \
    name.alloc_str = false; \
    name.value.pointer.value = val

#define data_value_set_boolean(name, val) \
    name.type = DT_BOOLEAN; \
    name.alloc_str = false; \
    name.value.boolean.value = val

#define data_value_set_integer_from_bytes(name, byte) \
    name.type = DT_INTEGER; \
    name.alloc_str = false; \
    memcpy(name.value.integer.bytes, byte, sizeof(DataInteger))

#define data_value_set_double_from_bytes(name, byte) \
    name.type = DT_DOUBLE; \
    name.alloc_str = false; \
    memcpy(name.value.double_float.bytes, byte, sizeof(DataDouble))

#define data_value_set_pointer_from_bytes(name, byte) \
    name.type = DT_POINTER; \
    name.alloc_str = false; \
    memcpy(name.value.pointer.bytes, byte, sizeof(DataPointer))

#define data_value_set_boolean_from_bytes(name, byte) \
    name.type = DT_BOOLEAN; \
    name.alloc_str = false; \
    name.value.boolean.bytes[0] = byte

// Testing to see if inlining is faster
#define data_value_from_bytes(value, bytes, len) \
    switch(bytes[0]) \
    { \
        case DT_POINTER: \
        { \
            data_value_set_pointer_from_bytes(value, bytes+1); \
            break; \
        } \
        case DT_INTEGER: \
        { \
            data_value_set_integer_from_bytes(value, bytes+1); \
            break; \
        } \
        case DT_DOUBLE: \
        { \
            data_value_set_double_from_bytes(value, bytes+1); \
            break; \
        } \
        case DT_STRING: \
        { \
            data_value_set_string(value, bytes+1, len, 0); \
            break; \
        } \
        case DT_BOOLEAN: \
        { \
            data_value_set_boolean_from_bytes(value, bytes[1]); \
            break; \
        } \
        case DT_JSON: \
        { \
            data_value_set_json(value, bytes+1, len, 0); \
            break; \
        } \
    }

DataIndex* data_index_new(size_t buffer);

DataIndex* data_index_new_with_end_only(size_t buffer);

bool data_index_append(DataIndex* index, size_t start, size_t end);

bool data_index_prepend(DataIndex* index, size_t start, size_t end);

bool data_index_edit(
    DataIndex* index, size_t position, size_t start, size_t end
);

bool data_index_delete(DataIndex* index, size_t position);

bool data_index_get(
    const DataIndex* index, size_t position, size_t* start, size_t* end
);

void data_index_clear(DataIndex* index);

void data_index_free(DataIndex* index);



DataList* data_list_new(size_t buffer);

bool data_list_append(DataList* list, const void* data, size_t bytes);

bool data_list_prepend(DataList* list, const void* data, size_t bytes);

bool data_list_edit(
    DataList* list, size_t position, const void* data, size_t bytes
);

bool data_list_delete(DataList* list, size_t position);

bool data_list_get(
    const DataList* list, size_t position, void** data
);

void data_list_clear(DataList* list);

void data_list_free(DataList* list);



DataString* data_string_new(
    const char* string, size_t len, size_t buffer
);

void data_string_free(DataString* data);

void data_string_clear(DataString* data, size_t buffer);

size_t data_string_count(
    const DataString* haystack,
    const DataString* needle,
    size_t** positions
);

void data_string_replace(
    const DataString* find,
    const DataString* replacement,
    DataString* target
);

bool data_string_replace_position(
    DataString* target,
    const DataString* replacement,
    size_t start,
    size_t end
);

void data_string_append(DataString* string, const char value, size_t buffer);

void data_string_append_string(
    DataString* string, const char* value, size_t len, size_t buffer
);

void data_string_prepend(
    DataString* string, const char value, size_t buffer
);

void data_string_prepend_string(
    DataString* string, const char* value, size_t len, size_t buffer
);

void data_string_print(DataString* data);

void data_string_println(DataString* data);



DataStorage* data_storage_new();

DataStorage* data_storage_new_with_index();

DataStorage* data_storage_new_with_index_and_list();

void data_storage_free(DataStorage* storage);

void data_storage_append(
    DataStorage* storage, DataValue value
);

void data_storage_prepend(
    DataStorage* storage, DataValue value
);

bool data_storage_edit(
    DataStorage* storage, size_t position, DataValue value
);

DataValue data_storage_get(DataStorage* storage, size_t position);

DataString data_storage_get_raw(DataStorage* storage, size_t position);

DataValue data_storage_get_current(DataStorage* storage);

DataString data_storage_get_current_raw(DataStorage* storage);

bool data_storage_remove(
    DataStorage* storage, size_t position
);

size_t data_storage_size(const DataStorage* storage);

void data_storage_clear(DataStorage* storage);

DataValue data_storage_get_next(DataStorage* storage);

bool data_storage_forward(DataStorage* storage, size_t to);

bool data_storage_rewind(DataStorage* storage, size_t to);

#endif