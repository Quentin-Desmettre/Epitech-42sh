/*
** EPITECH PROJECT, 2021
** my_free.c
** File description:
** my_free
*/

#include <stdlib.h>
#include <stdarg.h>
#include "libmy.h"

void try_free(void *to_free)
{
    if (to_free)
        free(to_free);
}

void free_table(void **table)
{
    int i = 0;

    for (; table[i]; i++)
        try_free(table[i]);
    try_free(table[i]);
    if (table)
        free(table);
}

void my_free(char *to_free, ...)
{
    va_list list_arg;

    va_start(list_arg, to_free);
    for (int i = 0; to_free[i]; i++) {
        if (to_free[i] == 'p')
            try_free(va_arg(list_arg, void *));
        if (to_free[i] == 'P')
            free_table(va_arg(list_arg, void **));
    }
    va_end(list_arg);
}
