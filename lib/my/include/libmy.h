/*
** EPITECH PROJECT, 2021
** my
** File description:
** libmy.h
*/

#ifndef LIBMY_H
    #define LIBMY_H
    #include <stdlib.h>
    #define init_with my_memset
    #include "iomanip.h"
    #include "linked_list.h"
    #include "my_printf.h"
    #include "others.h"
    #include "strmanip.h"

void flush_garbage(void);
void my_super_free(void *data);
void my_exit(int code);
void *my_malloc(size_t size);
void *my_calloc(size_t nb, size_t size);
void *my_realloc(void *base, size_t new_size);

    #define strdup my_strdup
    #define exit my_exit
    #define malloc my_malloc
    #define free my_super_free
    #define calloc my_calloc
    #define realloc my_realloc

#endif
