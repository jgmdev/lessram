/**
 * @author Jefferson González
 * @license MIT
 * @link https://github.com/jgmdev/lessram Source code.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "datastore.h"

#ifdef USE_C_MALLOC
    #define dsmalloc(bytes) malloc(bytes)
    #define dsrealloc(pointer, bytes) realloc(pointer, bytes)
    #define dsfree(pointer) free(pointer)
#else
    #include "php.h"
    #define dsmalloc(bytes) emalloc(bytes)
    #define dsrealloc(pointer, bytes) erealloc(pointer, bytes)
    #define dsfree(pointer) efree(pointer)
#endif

static const int DATASTORE_BUFFER = 1024 * 2; // 2 Kilobytes of buffer

static DataString SEPARATOR = {"~", 1, 0};
static DataString SEPARATOR_REPLACE = {"\\~", 2, 0};

// DataIndex Methods --------------------------------------------------

DataIndex* data_index_new(size_t buffer)
{
    DataIndex* index = dsmalloc(sizeof(DataIndex));

    if(!index)
        return NULL;

    index->count = 0;
    index->buffer = buffer;
    index->buffer_left = buffer;
    index->buffer_next = 0;

    if(buffer > 0)
    {
        index->start = dsmalloc(sizeof(size_t) * buffer);
        index->end = dsmalloc(sizeof(size_t) * buffer);
    }
    else
    {
        index->start = dsmalloc(sizeof(size_t));
        index->end = dsmalloc(sizeof(size_t));
        index->buffer_left = 1;
    }

    return index;
}

DataIndex* data_index_new_with_end_only(size_t buffer)
{
    DataIndex* index = dsmalloc(sizeof(DataIndex));

    if(!index)
        return NULL;

    index->start = NULL;
    index->count = 0;
    index->buffer = buffer;
    index->buffer_left = buffer;
    index->buffer_next = 0;

    if(buffer > 0)
    {
        index->end = dsmalloc(sizeof(size_t) * buffer);
    }
    else
    {
        index->end = dsmalloc(sizeof(size_t));
        index->buffer_left = 1;
    }

    return index;
}

bool data_index_append(DataIndex* index, size_t start, size_t end)
{
    if(index->buffer_left <= 0)
    {
        if(index->buffer > 0)
        {
            if(index->start)
            {
                index->start = dsrealloc(
                    index->start,
                    sizeof(size_t) * (index->count + index->buffer)
                );
            }

            index->end = dsrealloc(
                index->end,
                sizeof(size_t) * (index->count + index->buffer)
            );

            index->buffer_left = index->buffer;
        }
        else
        {
            if(index->start)
            {
                index->start = dsrealloc(
                    index->start,
                    sizeof(size_t) * (index->count + 1)
                );
            }

            index->end = dsrealloc(
                index->end,
                sizeof(size_t) * (index->count + 1)
            );

            index->buffer_left = 1;
        }
    }

    if(!index->end)
        return false;

    if(index->start)
        index->start[index->buffer_next] = start;

    index->end[index->buffer_next] = end;

    index->count++;
    index->buffer_next++;
    index->buffer_left--;

    return true;
}

bool data_index_prepend(DataIndex* index, size_t start, size_t end)
{
    if(index->buffer_left <= 0)
    {
        if(index->buffer > 0)
        {
            if(index->start)
            {
                index->start = dsrealloc(
                    index->start,
                    sizeof(size_t) * (index->count + index->buffer)
                );
            }

            index->end = dsrealloc(
                index->end,
                sizeof(size_t) * (index->count + index->buffer)
            );

            index->buffer_left = index->buffer;
        }
        else
        {
            if(index->start)
            {
                index->start = dsrealloc(
                    index->start,
                    sizeof(size_t) * (index->count + 1)
                );
            }

            index->end = dsrealloc(
                index->end,
                sizeof(size_t) * (index->count + 1)
            );

            index->buffer_left = 1;
        }
    }

    if(!index->end)
        return false;

    if(index->start)
    {
        memmove(
            &(index->start[1]),
            &(index->start[0]),
            sizeof(size_t) * index->count
        );

        index->start[0] = start;
    }

    memmove(
        &(index->end[1]),
        &(index->end[0]),
        sizeof(size_t) * index->count
    );

    index->end[0] = end;

    index->count++;
    index->buffer_next++;
    index->buffer_left--;

    return true;
}

bool data_index_edit(
    DataIndex* index, size_t position, size_t start, size_t end
)
{
    if(position > index->count)
        return false;

    if(index->start)
    {
        index->start[position] = start;
    }

    index->end[position] = end;

    return true;
}

bool data_index_delete(
    DataIndex* index, size_t position
)
{
    if(position >= index->count)
        return false;

    if(position != index->count-1)
    {
        if(index->start)
        {
            memmove(
                &(index->start[position]),
                &(index->start[position+1]),
                sizeof(size_t) * (index->count - position + 1)
            );
        }

        memmove(
            &(index->end[position]),
            &(index->end[position+1]),
            sizeof(size_t) * (index->count - position + 1)
        );
    }

    index->count--;
    index->buffer_left++;
    index->buffer_next--;

    return true;
}

bool data_index_get(
    const DataIndex* index, size_t position, size_t* start, size_t* end
)
{
    if(position > index->count)
        return false;

    if(index->start)
    {
        *start = index->start[position];
    }

    *end = index->end[position];

    return true;
}

void data_index_clear(DataIndex* index)
{
    if(!index)
        return;

    dsfree(index->start);
    dsfree(index->end);

    index->count = 0;
    index->buffer_next = 0;

    if(index->buffer > 0)
    {
        if(index->start)
        {
            index->start = dsmalloc(sizeof(size_t) * index->buffer);
        }

        index->end = dsmalloc(sizeof(size_t) * index->buffer);
        index->buffer_left = index->buffer;
    }
    else
    {
        if(index->start)
        {
            index->start = dsmalloc(sizeof(size_t));
        }

        index->end = dsmalloc(sizeof(size_t));
        index->buffer_left = 1;
    }
}

void data_index_free(DataIndex* index)
{
    if(index)
    {
        if(index->start)
        {
            dsfree(index->start);
        }

        dsfree(index->end);
        dsfree(index);
    }
}

// DataList Methods ----------------------------------------------------

DataList* data_list_new(size_t buffer)
{
    DataList* list = dsmalloc(sizeof(DataList));

    if(!list)
        return NULL;

    list->count = 0;
    list->buffer = buffer;
    list->buffer_left = buffer;
    list->buffer_next = 0;

    if(buffer > 0)
    {
        list->items = dsmalloc(sizeof(void*) * buffer);
    }
    else
    {
        list->items = dsmalloc(sizeof(void*));
        list->buffer_left = 1;
    }

    return list;
}

bool data_list_append(DataList* list, const void* data, size_t bytes)
{
    if(list->buffer_left <= 0)
    {
        if(list->buffer > 0)
        {
            list->items = dsrealloc(
                list->items,
                sizeof(void*) * (list->count + list->buffer)
            );

            list->buffer_left = list->buffer;
        }
        else
        {
            list->items = dsrealloc(
                list->items,
                sizeof(void*) * (list->count + 1)
            );

            list->buffer_left = 1;
        }
    }

    if(!list->items)
        return false;

    list->items[list->buffer_next] = dsmalloc(bytes);
    memcpy(list->items[list->buffer_next], data, bytes);

    list->count++;
    list->buffer_next++;
    list->buffer_left--;

    return true;
}

bool data_list_prepend(DataList* list, const void* data, size_t bytes)
{
    if(list->buffer_left <= 0)
    {
        if(list->buffer > 0)
        {
            list->items = dsrealloc(
                list->items,
                sizeof(void*) * (list->count + list->buffer)
            );

            list->buffer_left = list->buffer;
        }
        else
        {
            list->items = dsrealloc(
                list->items,
                sizeof(void*) * (list->count + 1)
            );

            list->buffer_left = 1;
        }
    }

    if(!list->items)
        return false;

    memmove(
        &(list->items[1]),
        &(list->items[0]),
        sizeof(void*) * list->count
    );

    list->items[0] = dsmalloc(bytes);
    memcpy(list->items[0], data, bytes);

    list->count++;
    list->buffer_next++;
    list->buffer_left--;

    return true;
}

bool data_list_edit(
    DataList* list, size_t position, const void* data, size_t bytes
)
{
    if(position > list->count)
        return false;

    dsfree(list->items[position]);

    list->items[position] = dsmalloc(bytes);
    memcpy(list->items[position], data, bytes);

    return true;
}

bool data_list_delete(DataList* list, size_t position)
{
    if(position >= list->count)
        return false;

    if(position != list->count-1)
    {
        memmove(
            &(list->items[position]),
            &(list->items[position+1]),
            sizeof(void*) * (list->count - position + 1)
        );
    }

    list->count--;
    list->buffer_left++;
    list->buffer_next--;

    return true;
}

bool data_list_get(
    const DataList* list, size_t position, void** data
)
{
    if(position > list->count)
        return false;

    *data = list->items[position];

    return true;
}

void data_list_clear(DataList* list)
{
    if(!list)
        return;

    for(size_t i=0; i<list->count; i++)
    {
        dsfree(list->items[i]);
    }

    dsfree(list->items);

    list->count = 0;
    list->buffer_next = 0;

    if(list->buffer > 0)
    {
        list->items = dsmalloc(sizeof(void*) * list->buffer);
        list->buffer_left = list->buffer;
    }
    else
    {
        list->items = dsmalloc(sizeof(void*));
        list->buffer_left = 1;
    }
}

void data_list_free(DataList* list)
{
    if(list)
    {
        for(size_t i=0; i<list->count; i++)
        {
            dsfree(list->items[i]);
        }

        dsfree(list->items);
        dsfree(list);
    }
}

// DataString Methods --------------------------------------------------

DataString* data_string_new(
    const char* string, size_t len, size_t buffer
)
{
    DataString* data = dsmalloc(sizeof(DataString));

    data->string = dsmalloc(len + buffer);

    if(len > 0)
    {
        memcpy(data->string, string, len);
    }

    data->len = len;
    data->buffer = buffer;

    return data;
}

void data_string_free(DataString* data)
{
    do
    {
    if(data)
    {
        dsfree(data->string);
        dsfree(data);
    }
    }while(0);
}

void data_string_clear(DataString* data, size_t buffer)
{
    if(!data)
        return;

    data->string = dsrealloc(data->string, buffer);

    data->len = 0;
    data->buffer = buffer;
}

size_t data_string_count(
    const DataString* haystack,
    const DataString* needle,
    size_t** positions
)
{
    size_t count = 0,
        hi, ni,
        h_len = haystack->len,
        n_len = needle->len
    ;

    if(positions != NULL)
    {
        *positions = dsmalloc(sizeof(size_t));
    }

    for(hi=0; hi<h_len; hi++)
    {
        bool found = false;

        for(ni=0; ni<n_len; ni++)
        {
            if((hi+ni) < h_len)
            {
                if(haystack->string[hi+ni] != needle->string[ni])
                {
                    break;
                }
            }

            if(ni+1 == n_len)
            {
                found = true;
            }
        }

        if(found)
        {
            count++;

            if(positions != NULL)
            {
                *positions = dsrealloc(*positions, sizeof(size_t) * count);
                (*positions)[count-1] = hi + 1;
            }
        }
    }

    if(positions != NULL)
    {
        *positions = dsrealloc(*positions, sizeof(size_t) * (count+1));
        (*positions)[count] = 0;
    }

    return count;
}

void data_string_replace(
    const DataString* find,
    const DataString* replacement,
    DataString* target
)
{
    size_t f_len = find->len,
        r_len = replacement->len,
        t_len = target->len
    ;

    size_t* positions = NULL;
    size_t found_count = data_string_count(target, find, &positions);

    if(found_count > 0)
    {
        size_t new_len = t_len - (f_len*found_count) + (r_len*found_count);

        if(r_len > f_len)
        {
            if(target->len + target->buffer < new_len)
            {
                // Allocate the required space
                target->string = dsrealloc(
                    target->string,
                    new_len
                );

                target->buffer = 0;
            }
            else
            {
                target->buffer -= new_len - target->len;
            }

            target->len = new_len;

            size_t i = 0;
            size_t shift = 0;
            do
            {
                size_t insert = (positions[i] - 1) + shift;
                size_t move_len = t_len - insert - f_len + shift;

                memmove(
                    &(target->string[insert+r_len]),
                    &(target->string[insert+f_len]),
                    move_len
                );

                shift += r_len - f_len;

                memcpy(
                    &(target->string[insert]),
                    replacement->string,
                    r_len
                );

                i++;
            }
            while (positions[i] != 0);
        }
        else
        {
            target->buffer += t_len - new_len;

            target->len = new_len;

            size_t i = 0;
            size_t unshift = 0;

            do
            {
                size_t insert = (positions[i] - 1) - unshift;
                size_t move_len = t_len - insert - f_len;

                memmove(
                    &(target->string[insert+r_len]),
                    &(target->string[insert+f_len]),
                    move_len
                );

                memcpy(
                    &(target->string[insert]),
                    replacement->string,
                    r_len
                );

                i++;

                unshift = i * (f_len - r_len);
            }
            while (positions[i] != 0);
        }
    }

    dsfree(positions);
}

bool data_string_replace_position(
    DataString* target,
    const DataString* replacement,
    size_t start,
    size_t end
)
{
    size_t r_len = replacement->len,
        t_len = target->len,
        p_len = end - start + 1;
    ;

    if(start > t_len || end > t_len)
        return false;

    if(p_len == r_len)
    {
        memcpy(
            &(target->string[start]),
            replacement->string,
            replacement->len
        );
    }
    else if(p_len > r_len)
    {
        memmove(
            &(target->string[start+r_len]),
            &(target->string[end+1]),
            t_len - end - 1
        );

        memcpy(
            &(target->string[start]),
            replacement->string,
            replacement->len
        );

        target->buffer += p_len - r_len;
        target->len -= p_len - r_len;
    }
    else
    {
        if(target->len + (r_len - p_len) > target->buffer)
        {
            target->string = dsrealloc(
                target->string,
                target->len + (r_len - p_len)
            );

            target->buffer = 0;
        }

        memmove(
            &(target->string[start+r_len]),
            &(target->string[end+1]),
            t_len - end - 1
        );

        memcpy(
            &(target->string[start]),
            replacement->string,
            replacement->len
        );

        target->len += r_len - p_len;

        if(target->buffer > 0)
        {
            target->buffer -= r_len - p_len;
        }
    }

    return true;
}

void data_string_append(DataString* string, const char value, size_t buffer)
{
    bool reallocated = false;

    if(string->buffer < 1)
    {
        string->string = dsrealloc(
            string->string, string->len + 1 + buffer
        );

        string->buffer = buffer;

        reallocated = true;
    }

    string->string[string->len] = value;
    string->len++;

    if(!reallocated)
    {
        string->buffer--;
    }
}

void data_string_append_string(
    DataString* string, const char* value, size_t len, size_t buffer
)
{
    bool reallocated = false;

    if(len > string->buffer)
    {
        string->string = dsrealloc(
            string->string, string->len + len + buffer
        );

        string->buffer = buffer;

        reallocated = true;
    }

    memcpy(&(string->string[string->len]), value, len);

    if(!reallocated)
    {
        string->buffer -= len;
    }

    string->len += len;
}

void data_string_prepend(
    DataString* string, const char value, size_t buffer
)
{
    bool reallocated = false;

    if(string->buffer < 1)
    {
        string->string = dsrealloc(
            string->string, string->len + 1 + buffer
        );

        string->buffer = buffer;

        reallocated = true;
    }

    memmove(
        &(string->string[1]),
        &(string->string[0]),
        string->len
    );

    string->string[0] = value;
    string->len++;

    if(!reallocated)
    {
        string->buffer--;
    }
}

void data_string_prepend_string(
    DataString* string, const char* value, size_t len, size_t buffer
)
{
    bool reallocated = false;

    if(len > string->buffer)
    {
        string->string = dsrealloc(
            string->string, string->len + len + buffer
        );

        string->buffer = buffer;

        reallocated = true;
    }

    memmove(
        &(string->string[len]),
        &(string->string[0]),
        string->len
    );

    memcpy(string->string, value, len);

    if(!reallocated)
    {
        string->buffer -= len;
    }

    string->len += len;
}

void data_string_print(DataString* data)
{
    if(!data)
        return;

    for(size_t i=0; i<data->len; i++)
    {
        printf("%c", data->string[i]);
    }
}

void data_string_println(DataString* data)
{
    if(!data)
        return;

    for(size_t i=0; i<data->len; i++)
    {
        printf("%c", data->string[i]);
    }

    printf("\n");
}

// DataStorage Methods -------------------------------------------------
DataStorage* data_storage_new()
{
    DataStorage* storage = (DataStorage*) dsmalloc(sizeof(DataStorage));

    if(!storage)
        return NULL;

    storage->elements = data_string_new("", 0, DATASTORE_BUFFER);
    storage->list_size = 0;
    storage->current_item = 0;
    storage->current_position = 0;
    storage->index = NULL;
    storage->list = NULL;

    return storage;
}

DataStorage* data_storage_new_with_index()
{
    DataStorage* storage = data_storage_new();

    if(!storage)
        return NULL;

    if(DATASTORE_BUFFER > 32)
    {
        storage->index = data_index_new(DATASTORE_BUFFER / 32);
    }
    else
    {
        storage->index = data_index_new(0);
    }

    return storage;
}

DataStorage* data_storage_new_with_index_and_list()
{
    DataStorage* storage = (DataStorage*) dsmalloc(sizeof(DataStorage));

    if(!storage)
        return NULL;

    storage->elements = NULL;
    storage->list_size = 0;
    storage->current_item = 0;
    storage->current_position = 0;

    if(DATASTORE_BUFFER > 32)
    {
        storage->index = data_index_new_with_end_only(DATASTORE_BUFFER / 32);
        storage->list = data_list_new(DATASTORE_BUFFER / 32);
    }
    else
    {
        storage->index = data_index_new_with_end_only(0);
        storage->list = data_list_new(0);
    }

    return storage;
}

void data_storage_free(DataStorage* storage)
{
    data_string_free(storage->elements);
    data_index_free(storage->index);
    data_list_free(storage->list);
    dsfree(storage);
}

void data_storage_append(DataStorage* storage, DataString* value)
{
    if(storage->index && storage->list)
    {
        data_index_append(storage->index, 0, value->len-1);
        data_list_append(storage->list, value->string, value->len);

        storage->list_size++;

        return;
    }

    data_string_replace(&SEPARATOR, &SEPARATOR_REPLACE, value);

    bool reallocated = false;

    // The value->len+1 is for extra ~ added at start
    if(value->len+1 > storage->elements->buffer)
    {
        storage->elements->string = dsrealloc(
            storage->elements->string,
            storage->elements->len + value->len+1 + DATASTORE_BUFFER
        );

        storage->elements->buffer = DATASTORE_BUFFER;

        reallocated = true;
    }

    storage->elements->string[storage->elements->len] = '~';

    memcpy(
        &(storage->elements->string[storage->elements->len+1]),
        value->string,
        value->len
    );

    if(storage->index)
    {
        data_index_append(
            storage->index,
            storage->elements->len+1,
            storage->elements->len + value->len
        );
    }

    storage->elements->len += value->len+1;

    if(!reallocated)
    {
        storage->elements->buffer -= value->len+1;
    }

    storage->list_size++;
}

void data_storage_prepend(DataStorage* storage, DataString* value)
{
    if(storage->index && storage->list)
    {
        data_index_prepend(storage->index, 0, value->len-1);
        data_list_prepend(storage->list, value->string, value->len);

        storage->list_size++;

        return;
    }

    data_string_replace(&SEPARATOR, &SEPARATOR_REPLACE, value);

    if(value->len+1 > storage->elements->buffer)
    {
        storage->elements->string = dsrealloc(
            storage->elements->string,
            storage->elements->len + value->len+1 + DATASTORE_BUFFER
        );

        storage->elements->buffer = DATASTORE_BUFFER;
    }

    memmove(
        storage->elements->string+(value->len+1),
        storage->elements->string,
        storage->elements->len
    );

    storage->elements->string[0] = '~';

    memcpy(
        storage->elements->string+1,
        value->string,
        value->len
    );

    if(storage->index)
    {
        data_index_prepend(
            storage->index,
            1,
            value->len
        );

        if(storage->index->count > 1)
        {
            size_t shift = value->len+1;

            for(size_t i=1; i<storage->index->count; i++)
            {
                storage->index->start[i] += shift;
                storage->index->end[i] += shift;
            }
        }
    }

    storage->elements->len += value->len+1;

    if(value->len+1 < storage->elements->buffer)
    {
        storage->elements->buffer -= value->len+1;
    }
    else
    {
        storage->elements->buffer = 0;
    }

    storage->list_size++;
}

bool data_storage_edit(
    DataStorage* storage, size_t position, DataString* value
)
{
    if(position >= storage->list_size || storage->list_size == 0)
        return false;

    if(storage->index && storage->list)
    {
        return
            data_index_edit(storage->index, position, 0, value->len-1)
            &&
            data_list_edit(storage->list, position, value->string, value->len)
        ;
    }

    data_string_replace(&SEPARATOR, &SEPARATOR_REPLACE, value);

    if(storage->index)
    {
        size_t start, end, previous_len;

        data_index_get(storage->index, position, &start, &end);

        data_string_replace_position(storage->elements, value, start, end);

        previous_len = end - start + 1;

        if(previous_len > value->len)
        {
            size_t unshift = previous_len - value->len;

            if(storage->index->count > 1)
            {
                for(size_t i=position+1; i<storage->index->count; i++)
                {
                    storage->index->start[i] -= unshift;
                    storage->index->end[i] -= unshift;
                }
            }
        }
        else if(previous_len < value->len)
        {
            size_t shift = value->len - previous_len;

            if(storage->index->count > 1)
            {
                for(size_t i=position+1; i<storage->index->count; i++)
                {
                    storage->index->start[i] += shift;
                    storage->index->end[i] += shift;
                }
            }
        }

        return true;
    }

    //size_t str_pos = 0;
    //DataString current_value = data_storage_get(position, str_pos);

    return true;
}

DataString data_storage_get(DataStorage* storage, size_t position)
{
    DataString data = {"", 0, 0};

    if(position >= storage->list_size)
    {
        return data;
    }

    if(storage->index && storage->list)
    {
        size_t end;

        data_index_get(
            storage->index,
            position,
            (void*) NULL, &end
        );

        data.string = storage->list->items[position];

        data.len = (storage->list->items[position] + end)
            - storage->list->items[position]
            + 1
        ;

        return data;
    }
    else if(storage->index)
    {
        size_t start, end;

        data_index_get(
            storage->index,
            position,
            &start, &end
        );

        data.string = storage->elements->string+start;
        data.len = end - start + 1;

        return data;
    }

    return data;
}

bool data_storage_remove(
    DataStorage* storage, size_t position
)
{
    if(position >= storage->list_size || storage->list_size == 0)
        return false;

    if(storage->index && storage->list)
    {
        data_index_delete(storage->index, position);
        data_list_delete(storage->list, position);

        storage->list_size--;

        return true;
    }

    if(storage->index)
    {
        size_t start, end, previous_len;

        data_index_get(storage->index, position, &start, &end);

        DataString empty = {"", 0, 0};

        data_string_replace_position(storage->elements, &empty, start-1, end);

        data_index_delete(storage->index, position);

        storage->list_size--;

        return true;
    }

    //size_t str_pos = 0;
    //DataString current_value = data_storage_get(position, str_pos);

    return true;
}

size_t data_storage_size(const DataStorage* storage)
{
    return storage->list_size;
}

void data_storage_clear(DataStorage* storage)
{
    data_string_clear(storage->elements, DATASTORE_BUFFER);
    data_index_clear(storage->index);
    data_list_clear(storage->list);

    storage->list_size = 0;
    storage->current_item = 0;
    storage->current_position = 0;
}

DataString data_storage_get_next(DataStorage* storage)
{
    DataString data = {"", 0, 0};
    if(storage->current_item >= storage->list_size)
    {
        storage->current_item = 0;
        storage->current_position = 0;
    }

    if(storage->index && storage->list)
    {
        size_t end;

        data_index_get(
            storage->index,
            storage->current_item,
            (void*) NULL, &end
        );

        data.string = (char*) storage->list->items[storage->current_item];

        data.len = (storage->list->items[storage->current_item] + end)
            - storage->list->items[storage->current_item]
            + 1
        ;

        storage->current_item++;

        return data;
    }
    else if(storage->index)
    {
        size_t start, end;

        data_index_get(
            storage->index,
            storage->current_item,
            &start, &end
        );

        data.string = storage->elements->string+start;
        data.len = end - start + 1;

        storage->current_item++;

        return data;
    }

    size_t elements_len = storage->elements->len,
        i = storage->current_position,
        len = 0
    ;

    i++; //Skip starting ~

    // Empty value
    if(i >= elements_len || storage->elements->string[i] == '~')
    {
        return data;
    }

    do
    {
        if(
            storage->elements->string[i] == '\\'
            &&
            storage->elements->string[i+1] == '~'
        )
        {
            len++;
            i++;
        }

        len++;

        i++;
    }
    while(
        i < elements_len
        &&
        storage->elements->string[i] != '~'
    );

    size_t start_pos = storage->current_position+1;

    storage->current_item++;
    storage->current_position = i;

    data.string = &(storage->elements->string[start_pos]);
    data.len = len;

    return data;
}

DataString* data_storage_get_next_copy(DataStorage* storage)
{
    if(storage->current_item >= storage->list_size)
    {
        storage->current_item = 0;
        storage->current_position = 0;
    }

    DataString* value = data_string_new("", 0, 200);

    if(storage->index && storage->list)
    {
        size_t end, len;

        data_index_get(
            storage->index,
            storage->current_item,
            (void*) NULL, &end
        );

        len = (storage->list->items[storage->current_item] + end)
            - storage->list->items[storage->current_item]
            + 1
        ;

        data_string_append_string(
            value,
            storage->list->items[storage->current_item],
            len,
            0
        );

        storage->current_item++;

        return value;
    }
    else if(storage->index)
    {
        size_t start, end;

        data_index_get(
            storage->index,
            storage->current_item,
            &start, &end
        );

        data_string_append_string(
            value,
            storage->elements->string+start,
            end - start + 1,
            0
        );

        storage->current_item++;

        return value;
    }

    size_t elements_len = storage->elements->len;
    size_t i=storage->current_position, len = 0;

    i++; //Skip starting ~

    // Empty value
    if(i >= elements_len || storage->elements->string[i] == '~')
    {
        return value;
    }

    do
    {
        if(
            storage->elements->string[i] == '\\'
            &&
            storage->elements->string[i+1] == '~'
        )
        {
            len++;
            i++;
        }

        len++;

        i++;
    }
    while(
        i < elements_len
        &&
        storage->elements->string[i] != '~'
    );

    size_t start_pos = storage->current_position+1;

    storage->current_item++;
    storage->current_position = i;

    data_string_append_string(
        value, &(storage->elements->string[start_pos]), len, 0
    );

    return value;
}

void data_storage_rewind(DataStorage* storage)
{
    storage->current_item = 0;
    storage->current_position = 0;
}