/**
 * @author Jefferson González
 * @license MIT
 * @link https://github.com/jgmdev/lessram Source code.
 * 
 * Serves to test the small DataStorage library but e* functions
 * need to be replaced with standard C ones.
*/

#include <stdio.h>
#include <string.h>

#include <unistd.h>
#include <sys/time.h>

#include "datastore.h"

int main(int argc, char** argv)
{
    DataString* texto = data_string_new("hola", 4, 10);
    data_string_prepend_string(texto, "jepa ", 5, 0);

    data_string_append_string(texto, " todo", 5, 0);
    data_string_append(texto, '+', 0);
    data_string_append(texto, '\n', 0);
    data_string_prepend(texto, '=', 0);
    data_string_print(texto);

    DataStorage* storage = data_storage_new();

    struct timeval time;
    gettimeofday(&time, NULL);

    DataString* value = data_string_new("", 0, 25);
    char buffer[25];
    for(size_t i=0; i<1048577; i++)
    {
        sprintf(buffer, "%s - %d\n", "hello world", (int) i);
        int len = strlen(buffer);
        memcpy(value->string, buffer, len);
        value->len = len;

        data_storage_append(storage, value);
    }

    for(size_t i=0; i<100; i++)
    {
        sprintf(buffer, "%s - %d\n", "hello world", (int) i);
        int len = strlen(buffer);
        memcpy(value->string, buffer, len);
        value->len = len;

        data_storage_prepend(storage, value);
    }

    data_string_free(value);

    struct timeval time_finish;
    gettimeofday(&time_finish, NULL);

    double microsec_start = (time.tv_sec * 1000 * 1000) + time.tv_usec;
    double microsec_end = (time_finish.tv_sec * 1000 * 1000) + time_finish.tv_usec;

    printf("Add time: %f\n", (microsec_end -  microsec_start) / 1000 / 1000);

    struct timeval time_loop;
    gettimeofday(&time_loop, NULL);

    for(size_t i=0; i<10; i++)
    {
        DataString string = data_storage_next(storage);
        data_string_print(&string);
    }

    struct timeval time_loop_finish;
    gettimeofday(&time_loop_finish, NULL);

    double microsec_start_loop = (time_loop.tv_sec * 1000 * 1000) + time_loop.tv_usec;
    double microsec_end_loop = (time_loop_finish.tv_sec * 1000 * 1000) + time_loop_finish.tv_usec;

    printf("Loop time %f\n", (microsec_end_loop -  microsec_start_loop) / 1000 / 1000);

    data_storage_free(storage);

    return 0;
}