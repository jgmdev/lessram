#include <string.h>

#include "datastoresimple.h"

DataStorageSimple* data_storage_simple_new(size_t buffer)
{
    DataStorageSimple* storage = dsmalloc(sizeof(DataStorageSimple));

    if(!storage)
        return NULL;

    storage->count = 0;
    storage->current = 0;
    storage->buffer = 0;
    storage->buffer_left = 0;

    data_storage_simple_expand(storage, (int)buffer <= 0 ? 1 : buffer);

    return storage;
}

void data_storage_simple_free(DataStorageSimple* storage)
{
    if(storage)
    {
        for(size_t i=0; i<storage->count; i++)
        {
            char type = storage->elements[i].type;

            if(type == DT_STRING || type == DT_JSON)
            {
                data_string_free(storage->elements[i].data);
            }
            else if(type == DT_POINTER)
            {
                dsfree(storage->elements[i].data);
            }
        }

        dsfree(storage->elements);
        dsfree(storage);
    }
}

static inline void data_storage_simple_free_element(
    DataStoreSimpleElement element
)
{
    char type = element.type;

    if(type == DT_STRING || type == DT_JSON)
    {
        data_string_free(element.data);
    }
    else if(type == DT_POINTER)
    {
        dsfree(element.data);
    }
}

bool data_storage_simple_expand(DataStorageSimple* storage, size_t buffer)
{
    if(storage->buffer_left <= 0)
    {
        if(storage->buffer > 0)
        {
            storage->elements = dsrealloc(
                storage->elements,
                sizeof(DataStoreSimpleElement) 
                    * (storage->count + storage->buffer)
            );

            storage->buffer_left = storage->buffer;
        }
        else
        {
            storage->elements = dsmalloc(
                sizeof(DataStoreSimpleElement) * buffer
            );

            storage->buffer = buffer;
            storage->buffer_left = 1;
        }
    }

    if(!storage->elements)
        return false;

    return true;
}

bool data_storage_simple_shift(DataStorageSimple* storage)
{
    if(!data_storage_simple_expand(storage, storage->buffer))
    {
        return false;
    }

    memmove(
        &(storage->elements[1]),
        &(storage->elements[0]),
        sizeof(DataStoreSimpleElement) * storage->count
    );

    return true;
}

bool data_storage_simple_unshift(
    DataStorageSimple* storage, size_t from, size_t to
)
{
    if(from >= storage->count || to > from)
    {
        return false;
    }

    memmove(
        &(storage->elements[to]),
        &(storage->elements[from]),
        sizeof(DataStoreSimpleElement) * storage->count
    );

    storage->count--;
    storage->buffer_left++;

    return true;
}


bool data_storage_simple_append_bool(DataStorageSimple* storage, bool value)
{
    if(!data_storage_simple_expand(storage, storage->buffer))
        return false;

    DataStoreSimpleUnion converter = {.boolean = value};

    storage->elements[storage->count].type = DT_BOOLEAN;
    storage->elements[storage->count].data = converter.pointer;

    storage->count++;
    storage->buffer_left--;

    return true;
}

bool data_storage_simple_append_double(DataStorageSimple* storage, double value)
{
    if(!data_storage_simple_expand(storage, storage->buffer))
        return false;

    DataStoreSimpleUnion converter = {.double_float = value};

    storage->elements[storage->count].type = DT_DOUBLE;
    storage->elements[storage->count].data = converter.pointer;

    storage->count++;
    storage->buffer_left--;

    return true;
}

bool data_storage_simple_append_int(DataStorageSimple* storage, size_t value)
{
    if(!data_storage_simple_expand(storage, storage->buffer))
        return false;

    DataStoreSimpleUnion converter = {.integer = value};

    storage->elements[storage->count].type = DT_INTEGER;
    storage->elements[storage->count].data = converter.pointer;

    storage->count++;
    storage->buffer_left--;

    return true;
}

bool data_storage_simple_append_pointer(DataStorageSimple* storage, void* value)
{
    if(!data_storage_simple_expand(storage, storage->buffer))
        return false;

    storage->elements[storage->count].type = DT_POINTER;
    storage->elements[storage->count].data = value;

    storage->count++;
    storage->buffer_left--;

    return true;
}

bool data_storage_simple_append_bytes(
    DataStorageSimple* storage, char* value, size_t len
)
{
    if(!data_storage_simple_expand(storage, storage->buffer))
        return false;

    DataString* string = data_string_new(value, len, 1);
    string->string[len] = '\0';

    storage->elements[storage->count].type = DT_STRING;
    storage->elements[storage->count].data = string;

    storage->count++;
    storage->buffer_left--;

    return true;
}

bool data_storage_simple_append_json(
    DataStorageSimple* storage, char* value, size_t len
)
{
    if(!data_storage_simple_expand(storage, storage->buffer))
        return false;

    DataString* string = data_string_new(value, len, 1);
    string->string[len] = '\0';

    storage->elements[storage->count].type = DT_JSON;
    storage->elements[storage->count].data = string;

    storage->count++;
    storage->buffer_left--;

    return true;
}


bool data_storage_simple_prepend_bool(DataStorageSimple* storage, bool value)
{
    if(!data_storage_simple_shift(storage))
        return false;

    DataStoreSimpleUnion converter = {.boolean = value};

    storage->elements[0].type = DT_BOOLEAN;
    storage->elements[0].data = converter.pointer;

    storage->count++;
    storage->buffer_left--;

    return true;
}

bool data_storage_simple_prepend_double(DataStorageSimple* storage, double value)
{
    if(!data_storage_simple_shift(storage))
        return false;

    DataStoreSimpleUnion converter = {.double_float = value};

    storage->elements[0].type = DT_DOUBLE;
    storage->elements[0].data = converter.pointer;

    storage->count++;
    storage->buffer_left--;

    return true;
}

bool data_storage_simple_prepend_int(DataStorageSimple* storage, size_t value)
{
    if(!data_storage_simple_shift(storage))
        return false;

    DataStoreSimpleUnion converter = {.integer = value};

    storage->elements[0].type = DT_INTEGER;
    storage->elements[0].data = converter.pointer;

    storage->count++;
    storage->buffer_left--;

    return true;
}

bool data_storage_simple_prepend_pointer(
    DataStorageSimple* storage, void* value
)
{
    if(!data_storage_simple_shift(storage))
        return false;

    storage->elements[0].type = DT_INTEGER;
    storage->elements[0].data = value;

    storage->count++;
    storage->buffer_left--;

    return true;
}

bool data_storage_simple_prepend_bytes(
    DataStorageSimple* storage, char* value, size_t len
)
{
    if(!data_storage_simple_shift(storage))
        return false;

    DataString* string = data_string_new(value, len, 1);
    string->string[len] = '\0';

    storage->elements[0].type = DT_STRING;
    storage->elements[0].data = string;

    storage->count++;
    storage->buffer_left--;

    return true;
}

bool data_storage_simple_prepend_json(
    DataStorageSimple* storage, char* value, size_t len
)
{
    if(!data_storage_simple_shift(storage))
        return false;

    DataString* string = data_string_new(value, len, 1);
    string->string[len] = '\0';

    storage->elements[0].type = DT_JSON;
    storage->elements[0].data = string;

    storage->count++;
    storage->buffer_left--;

    return true;
}


bool data_storage_simple_edit_bool(
    DataStorageSimple* storage, size_t position, bool value
)
{
    if(position > storage->count)
        return false;

    data_storage_simple_free_element(storage->elements[position]);

    DataStoreSimpleUnion converter = {.boolean = value};

    storage->elements[position].type = DT_BOOLEAN;
    storage->elements[position].data = converter.pointer;

    return true;
}

bool data_storage_simple_edit_double(
    DataStorageSimple* storage, size_t position, double value
)
{
    if(position > storage->count)
        return false;

    data_storage_simple_free_element(storage->elements[position]);

    DataStoreSimpleUnion converter = {.double_float = value};

    storage->elements[position].type = DT_DOUBLE;
    storage->elements[position].data = converter.pointer;
}

bool data_storage_simple_edit_int(
    DataStorageSimple* storage, size_t position, size_t value
)
{
    if(position > storage->count)
        return false;

    data_storage_simple_free_element(storage->elements[position]);

    DataStoreSimpleUnion converter = {.integer = value};

    storage->elements[position].type = DT_INTEGER;
    storage->elements[position].data = converter.pointer;
}

bool data_storage_simple_edit_pointer(
    DataStorageSimple* storage, size_t position, void* value
)
{
    if(position > storage->count)
        return false;

    data_storage_simple_free_element(storage->elements[position]);

    storage->elements[position].type = DT_POINTER;
    storage->elements[position].data = value;
}

bool data_storage_simple_edit_bytes(
    DataStorageSimple* storage, size_t position, char* value, size_t len
)
{
    if(position > storage->count)
        return false;

    data_storage_simple_free_element(storage->elements[position]);

    DataString* string = data_string_new(value, len, 1);
    string->string[len] = '\0';

    storage->elements[position].type = DT_STRING;
    storage->elements[position].data = string;
}

bool data_storage_simple_edit_json(
    DataStorageSimple* storage, size_t position, char* value, size_t len
)
{
    if(position > storage->count)
        return false;

    data_storage_simple_free_element(storage->elements[position]);

    DataString* string = data_string_new(value, len, 1);
    string->string[len] = '\0';

    storage->elements[position].type = DT_JSON;
    storage->elements[position].data = string;
}


DataStoreSimpleElement* data_storage_simple_get(
    DataStorageSimple* storage, size_t position
)
{
    if(storage->count > 0 && position < storage->count)
    {
        return (storage->elements+position);
    }

    return NULL;
}

DataStoreSimpleElement* data_storage_simple_get_next(DataStorageSimple* storage)
{
    if(storage->count > 0 && storage->current < storage->count)
    {
        storage->current++;

        return (storage->elements+storage->current-1);
    }

    data_storage_simple_rewind(storage);

    return NULL;
}

void data_storage_simple_rewind(DataStorageSimple* storage)
{
    storage->current = 0;
}

bool data_storage_simple_forward(DataStorageSimple* storage)
{
    if(storage->count > 0 && storage->current+1 < storage->count)
    {
        storage->current++;

        return true;
    }

    return false;
}


bool data_storage_simple_remove(DataStorageSimple* storage, size_t position)
{
    return data_storage_simple_unshift(storage, position+1, position);
}

void data_storage_simple_clear(DataStorageSimple* storage)
{
    if(!storage)
        return;

    for(size_t i=0; i<storage->count; i++)
    {
        char type = storage->elements[i].type;

        if(type == DT_STRING || type == DT_JSON)
        {
            data_string_free(storage->elements[i].data);
        }
        else if(type == DT_POINTER)
        {
            dsfree(storage->elements[i].data);
        }

        storage->buffer_left++;
    }

    storage->count = 0;

    if(storage->buffer_left > storage->buffer)
    {
        storage->elements = dsrealloc(
            storage->elements,
            sizeof(DataStoreSimpleElement) * storage->buffer
        );

        storage->buffer_left = storage->buffer;
    }
}