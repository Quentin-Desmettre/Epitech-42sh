/*
** EPITECH PROJECT, 2021
** MY_MEMSET
** File description:
** Copies the char c to the first n characters of the string
*/

#include "libmy.h"

void my_memset(void *what, int c, int n)
{
    char *cp = what;

    for (int i = 0; i < n; i++)
        cp[i] = c;
}

int mem_is_eq(void *a, void *b, size_t size)
{
    char *aa = a;
    char *bb = b;

    for (size_t i = 0; i < size; i++)
        if (aa[i] != bb[i])
            return 0;
    return 1;
}

void my_memcpy(void *dest, void *source, size_t size)
{
    char *a = dest;
    char *b = source;

    for (size_t i = 0; i < size; i++)
        a[i] = b[i];
}

void free_str_array(char **array, int include_null)
{
    int i = 0;

    if (!array)
        return;
    for (; array[i]; i++)
        free(array[i]);
    if (include_null)
        free(array[i]);
    free(array);
}
