/**
 * @author Jefferson González
 * @license MIT
 * @link https://github.com/jgmdev/lessram Source code.
*/

#ifndef DATA_STORE_NATIVE_H
#define DATA_STORE_NATIVE_H

#include <stddef.h>
#include <stdbool.h>

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
    DataStorage* storage, DataString* value
);

void data_storage_prepend(
    DataStorage* storage, DataString* value
);

bool data_storage_edit(
    DataStorage* storage, size_t position, DataString* value
);

DataString data_storage_get(DataStorage* storage, size_t position);

DataString data_storage_get_current(DataStorage* storage);

bool data_storage_remove(
    DataStorage* storage, size_t position
);

size_t data_storage_size(const DataStorage* storage);

void data_storage_clear(DataStorage* storage);

DataString data_storage_get_next(DataStorage* storage);

DataString* data_storage_get_next_copy(DataStorage* storage);

bool data_storage_forward(DataStorage* storage, size_t to);

bool data_storage_rewind(DataStorage* storage, size_t to);

#endif