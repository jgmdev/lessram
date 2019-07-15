/**
 * @author Jefferson González
 * @license MIT
 * @link https://github.com/jgmdev/lessram Source code.
*/

#ifndef DATA_STORE_NATIVE_H
#define DATA_STORE_NATIVE_H

#include <stddef.h>

#define DS_PARAM(s) s, sizeof(s)-1


typedef struct _DataString {
    char* string;
    size_t len;
    size_t buffer;
} DataString;

typedef struct _DataStorage {
    DataString* elements;
    size_t list_size;
    size_t current_item;
    size_t current_position;
} DataStorage;



DataString* data_string_new(
    const char* string, size_t len, size_t buffer
);

void data_string_free(DataString* data);

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



DataStorage* data_storage_new();

void data_storage_free(DataStorage* storage);

void data_storage_append(
    DataStorage* storage, DataString* value
);

void data_storage_prepend(
    DataStorage* storage, DataString* value
);

DataString data_storage_next(DataStorage* storage);

DataString* data_storage_next_copy(DataStorage* storage);

#endif