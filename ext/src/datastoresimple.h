#ifndef DATA_STORE_SIMPLE_H
#define DATA_STORE_SIMPLE_H

#include <stddef.h>
#include <stdbool.h>

#include "datastore.h"

typedef struct _DataStoreSimpleElement {
    char type;
    void* data;
} DataStoreSimpleElement;

typedef struct _SimpleStore {
    DataStoreSimpleElement* elements;
    size_t count;
    size_t current;
    size_t buffer;
    size_t buffer_left;
} DataStorageSimple;

typedef enum _DataStoreElementSize {
    SIMPLE_BOOL_SIZE=sizeof(bool),
    SIMPLE_DOUBLE_SIZE=sizeof(double),
    SIMPLE_INT_SIZE=sizeof(size_t),    
    SIMPLE_POINTER_SIZE=sizeof(void*)
} DataStoreElementSize;

typedef union _DataStoreSimpleUnion {
    char bytes[sizeof(void*)];
    double double_float;
    bool boolean;
    void* pointer;
    size_t integer;
} DataStoreSimpleUnion;


DataStorageSimple* data_storage_simple_new(size_t buffer);

void data_storage_simple_free(DataStorageSimple* storage);

static inline void data_storage_simple_free_element(DataStoreSimpleElement element);

bool data_storage_simple_expand(DataStorageSimple* storage, size_t buffer);

bool data_storage_simple_shift(DataStorageSimple* storage);

bool data_storage_simple_unshift(DataStorageSimple* storage, size_t from, size_t to);


bool data_storage_simple_append_bool(DataStorageSimple* storage, bool value);

bool data_storage_simple_append_double(DataStorageSimple* storage, double value);

bool data_storage_simple_append_int(DataStorageSimple* storage, size_t value);

bool data_storage_simple_append_pointer(DataStorageSimple* storage, void* value);

bool data_storage_simple_append_bytes(DataStorageSimple* storage, char* value, size_t len);

bool data_storage_simple_append_json(DataStorageSimple* storage, char* value, size_t len);


bool data_storage_simple_prepend_bool(DataStorageSimple* storage, bool value);

bool data_storage_simple_prepend_double(DataStorageSimple* storage, double value);

bool data_storage_simple_prepend_int(DataStorageSimple* storage, size_t value);

bool data_storage_simple_prepend_pointer(DataStorageSimple* storage, void* value);

bool data_storage_simple_prepend_bytes(DataStorageSimple* storage, char* value, size_t len);

bool data_storage_simple_prepend_json(DataStorageSimple* storage, char* value, size_t len);


bool data_storage_simple_edit_bool(DataStorageSimple* storage, size_t position, bool value);

bool data_storage_simple_edit_double(DataStorageSimple* storage, size_t position, double value);

bool data_storage_simple_edit_int(DataStorageSimple* storage, size_t position, size_t value);

bool data_storage_simple_edit_pointer(DataStorageSimple* storage, size_t position, void* value);

bool data_storage_simple_edit_bytes(DataStorageSimple* storage, size_t position, char* value, size_t len);

bool data_storage_simple_edit_json(DataStorageSimple* storage, size_t position, char* value, size_t len);


DataStoreSimpleElement* data_storage_simple_get(DataStorageSimple* storage, size_t position);

DataStoreSimpleElement* data_storage_simple_get_next(DataStorageSimple* storage);

void data_storage_simple_rewind(DataStorageSimple* storage);

bool data_storage_simple_forward(DataStorageSimple* storage);


bool data_storage_simple_remove(DataStorageSimple* storage, size_t position);

void data_storage_simple_clear(DataStorageSimple* storage);

#endif