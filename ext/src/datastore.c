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
    #define dsmalloc(a) malloc(a)
    #define dsrealloc(p, s) realloc(p, s)
    #define dsfree(p) free(p)
#else
    #include "php.h"
    #define dsmalloc(a) emalloc(a)
    #define dsrealloc(p, s) erealloc(p, s)
    #define dsfree(p) efree(p)
#endif

static const int DATASTORE_BUFFER = 1024 * 5; // 5 Kilobytes of buffer

static DataString SEPARATOR = {"~", 1, 0};
static DataString SEPARATOR_REPLACE = {"\\~", 2, 0};

// DataIndex Methods --------------------------------------------------

DataIndex* data_index_new(size_t buffer)
{
    DataIndex* index = malloc(sizeof(DataIndex));

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

bool data_index_add(DataIndex* index, size_t start, size_t end)
{
    if(index->buffer_left <= 0)
    {
        if(index->buffer > 0)
        {
            index->start = dsrealloc(
                index->start, 
                sizeof(size_t) * (index->count + index->buffer)
            );

            index->end = dsrealloc(
                index->start, 
                sizeof(size_t) * (index->count + index->buffer)
            );

            index->buffer_left = index->buffer;
        }
        else
        {
            index->start = dsrealloc(
                index->start, 
                sizeof(size_t) * (index->count + 1)
            );

            index->end = dsrealloc(
                index->start, 
                sizeof(size_t) * (index->count + 1)
            );

            index->buffer_left = 1;
        }
    }

    if(!index->start)
        return false;

    index->start[index->buffer_next] = start;
    index->end[index->buffer_next] = end;

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

    index->start[position] = start;
    index->end[position] = start;
    
    return true;
}

bool data_index_delete(
    DataIndex* index, size_t position
)
{
    if(position > index->count)
        return false;

    if(position != index->count-1)
    {
        memmove(
            &(index->start[position]), 
            &(index->start[position+1]),
            sizeof(size_t) * (index->count - position + 1)
        );

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

    *start = index->start[position];
    *end = index->end[position];

    return true;
}

void data_index_clear(DataIndex* index)
{
    dsfree(index->start);
    dsfree(index->end);

    index->count = 0;
    index->buffer_next = 0;

    if(index->buffer > 0)
    {
        index->start = dsmalloc(sizeof(size_t) * index->buffer);
        index->end = dsmalloc(sizeof(size_t) * index->buffer);
        index->buffer_left = index->buffer;
    }
    else
    {
        index->start = dsmalloc(sizeof(size_t));
        index->end = dsmalloc(sizeof(size_t));
        index->buffer_left = 1;
    }
}

void data_index_free(DataIndex* index)
{
    if(index)
    {
        dsfree(index->start);
        dsfree(index->end);
        dsfree(index);
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
    if(data)
    {
        dsfree(data->string);
        dsfree(data);
    }
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
    for(int i=0; i<data->len; i++)
    {
        printf("%c", data->string[i]);
    }
}

// DataStorage Methods -------------------------------------------------
DataStorage* data_storage_new()
{
    DataStorage* storage = (DataStorage*) dsmalloc(sizeof(DataStorage));

    storage->elements = data_string_new("", 0, DATASTORE_BUFFER);
    storage->list_size = 0;
    storage->current_item = 0;
    storage->current_position = 0;

    return storage;
}

void data_storage_free(DataStorage* storage)
{
    data_string_free(storage->elements);
    dsfree(storage);
}

void data_storage_append(DataStorage* storage, DataString* value)
{
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

    storage->elements->len += value->len+1;

    if(!reallocated)
    {
        storage->elements->buffer -= value->len+1;
    }

    storage->list_size++;
}

void data_storage_prepend(DataStorage* storage, DataString* value)
{
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
    if(position >= storage->list_size)
        return false;

    data_string_replace(&SEPARATOR, &SEPARATOR_REPLACE, value);

    size_t str_pos = 0;
    //DataString current_value = data_storage_get(position, str_pos);

    return true;
}

size_t data_storage_len(const DataStorage* storage)
{
    return storage->list_size;
}

void data_storage_clear(DataStorage* storage)
{
    data_string_free(storage->elements);

    storage->list_size = 0;
    storage->current_item = 0;
    storage->current_position = 0;
}

DataString data_storage_next(DataStorage* storage)
{
    DataString data = {"", 0, 0};
    if(storage->current_item >= storage->list_size)
    {
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

DataString* data_storage_next_copy(DataStorage* storage)
{
    if(storage->current_item >= storage->list_size)
    {
        return NULL;
    }

    DataString* value = data_string_new("", 0, 200);
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