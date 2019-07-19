/**
 * @author Jefferson González
 * @license MIT
 * @link https://github.com/jgmdev/lessram Source code.
 * 
 * Serves to test the small DataStorage library.
*/

#include <stdio.h>
#include <string.h>

#include <unistd.h>
#include <sys/time.h>

#include "datastore.h"
#include "datastoresimple.h"
#include "rpmalloc/rpmalloc.h"

const size_t append = 1024*1024;
const size_t prepend = 0;

int main(int argc, char** argv)
{
    DataStorageSimple* simple = data_storage_simple_new(100);

    for(size_t i=0; i<1024; i++)
    {
        data_storage_simple_append_bytes(simple, DS_STR("hola mundo 1234"));
    }

    for(size_t i=0; i<1024; i++)
    {
        data_storage_simple_get_next(simple);
    }

    data_storage_simple_free(simple);

    return 0;

    rpmalloc_initialize();
    DataStorage* storage = data_storage_new();

    for(size_t i=0; i<append; i++)
    {
        char value[25];
        sprintf(value, "[\"hello ~ %zu\"]", i);

        data_value_define_json(val, value, strlen(value), 10);
        data_storage_append(storage, val);
    }

    for(size_t i=0; i<storage->list_size; i++)
    {
        DataValue value = data_storage_get_next(storage);

        data_value_free(value);
    }

    data_storage_free(storage);

    rpmalloc_finalize();

    return 0;

    for(size_t i=0; i<storage->list_size; i++)
    {
        char value[25];
        sprintf(value, "[\"hola ~ %zu\"]", i);

        data_value_define_json(val, value, strlen(value), 0);
        data_storage_edit(storage, i, val);
    }

    data_value_define_string(newer, "hola", 4, 0);
    data_value_define_string(another, "jaja", 4, 0);
    data_storage_remove(storage, 2);
    data_storage_remove(storage, 3);    

    data_storage_rewind(storage, 0);

    data_value_free(newer);
    data_value_free(another);

    for(size_t i=0; i<10; i++)
    {
        DataValue value = data_storage_get_next(storage);

        switch(value.type)
        {
            case DT_STRING:
            case DT_JSON:
                data_string_println(value.value.string_alloc);
                break;
            default:
                break;
        }

        data_value_free(value);
    }

    data_storage_free(storage);

    return 0;
}