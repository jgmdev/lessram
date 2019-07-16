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

const size_t append = 165;
const size_t prepend = 0;

int main(int argc, char** argv)
{
    printf("DataString tests:\n");
    printf("============================\n");
    printf("\nCreate string 'John John':\n");
    DataString* message = data_string_new("John John", 9, 10);
    printf("Prepend 'Hola':\n");
    data_string_prepend_string(message, "Hola ", 5, 0);
    printf("Append ' today':\n");
    data_string_append_string(message, " today", 6, 0);
    printf("Append '!':\n");
    data_string_append(message, '!', 0);
    printf("Append '\\n':\n");
    data_string_append(message, '\n', 0);
    printf("Prepend '>':\n");
    data_string_prepend(message, '>', 0);
    printf("Result:\n");
    data_string_print(message);
    printf("----------------------------\n");
    
    printf("\nReplace 'Hola' with 'Hello':\n");
    DataString salute_es = {"Hola", 4, 0};
    DataString salute_en = {"Hello", 5, 0};
    data_string_replace(&salute_es, &salute_en, message);
    printf("Result:\n");
    data_string_print(message);
    printf("----------------------------\n");

    printf("\nReplace 'John' with 'Jeff':\n");
    DataString name_one = {"John", 4, 0};
    DataString name_two = {"Jeff", 4, 0};
    data_string_replace(&name_one, &name_two, message);
    printf("Result:\n");
    data_string_print(message);
    printf("----------------------------\n");

    printf("\nReplace position 7-10 with 'Jim':\n");
    DataString name_three = {"Jim", 3, 0};
    data_string_replace_position(message, &name_three, 7, 10);
    printf("Result:\n");
    data_string_print(message);
    printf("----------------------------\n");

    data_string_free(message);

    printf("\nDataStorage tests:\n");
    printf("============================\n");
    DataStorage* storage = data_storage_new_with_index_and_list();

    struct timeval time_append;
    gettimeofday(&time_append, NULL);

    DataString* value = data_string_new("", 0, 25);
    char buffer[25];
    for(size_t i=0; i<append; i++)
    {
        sprintf(buffer, "%s - %zu\n", "~hello ~ world~", i);
        int len = strlen(buffer);
        memcpy(value->string, buffer, len);
        value->len = len;

        data_storage_append(storage, value);
    }

    struct timeval time_append_finish;
    gettimeofday(&time_append_finish, NULL);

    double microsec_append_start = (time_append.tv_sec * 1000 * 1000) + time_append.tv_usec;
    double microsec_append_end = (time_append_finish.tv_sec * 1000 * 1000) + time_append_finish.tv_usec;

    printf("\nAfter append data looks like:\n");
    for(size_t i=0; i<append; i++)
    {
        DataString string = data_storage_get_next(storage);
        data_string_print(&string);
    }
    printf("\n----------------------------\n");

    struct timeval time_prepend;
    gettimeofday(&time_prepend, NULL);

    for(size_t i=0; i<prepend; i++)
    {
        sprintf(buffer, "%s - %zu\n", "~hello ~ world~", i);
        int len = strlen(buffer);
        memcpy(value->string, buffer, len);
        value->len = len;

        data_storage_prepend(storage, value);
    }

    struct timeval time_prepend_finish;
    gettimeofday(&time_prepend_finish, NULL);

    double microsec_prepend_start = (time_prepend.tv_sec * 1000 * 1000) + time_prepend.tv_usec;
    double microsec_prepend_end = (time_prepend_finish.tv_sec * 1000 * 1000) + time_prepend_finish.tv_usec;

    if(prepend)
    {
        printf("\nAfter prepend data looks like:\n");
        for(size_t i=0; i<prepend; i++)
        {
            DataString string = data_storage_get_next(storage);
            data_string_print(&string);
        }
        printf("\n----------------------------\n");
    }

    data_string_free(value);

    struct timeval time_loop;
    gettimeofday(&time_loop, NULL);

    printf("\nFinal result:\n");
    for(size_t i=0; i<(append+prepend); i++)
    {
        data_storage_get_next(storage);
    }
    
    struct timeval time_loop_finish;
    gettimeofday(&time_loop_finish, NULL);

    double microsec_start_loop = (time_loop.tv_sec * 1000 * 1000) + time_loop.tv_usec;
    double microsec_end_loop = (time_loop_finish.tv_sec * 1000 * 1000) + time_loop_finish.tv_usec;

    for(size_t i=0; i<(append+prepend); i++)
    {
        DataString string = data_storage_get_next(storage);
        data_string_print(&string);
    }

    printf("\n----------------------------\n");

    printf("Append time: %f\n", (microsec_append_end -  microsec_append_start) / 1000 / 1000);
    printf("Prepend time: %f\n", (microsec_prepend_end -  microsec_prepend_start) / 1000 / 1000);
    printf("Loop time %f\n", (microsec_end_loop -  microsec_start_loop) / 1000 / 1000);

    data_storage_free(storage);

    return 0;
}