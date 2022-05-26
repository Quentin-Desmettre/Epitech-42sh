/*
** EPITECH PROJECT, 2021
** lib
** File description:
** my_malloc.c
*/

#include <stdlib.h>
#include <unistd.h>
#include "linked_list.h"

void my_exit(int code);

s_list_t **malloc_stack(void)
{
    static s_list_t *n = 0;

    return &n;
}

void push_to_stack(s_list_t **stack, void *data)
{
    s_list_t *node = malloc(sizeof(s_list_t));

    if (!node) {
        write(2, "ERROR: malloc() failed.\n", 24);
        my_exit(84);
    }
    node->data = data;
    node->next = *stack;
    *stack = node;
}

void *my_malloc(size_t size)
{
    void *r = malloc(size);

    if (!r) {
        write(2, "ERROR: malloc() failed.\n", 24);
        my_exit(84);
    }
    push_to_stack(malloc_stack(), r);
    return r;
}

void *my_calloc(size_t nb, size_t size)
{
    void *r = calloc(nb, size);

    if (!r) {
        write(2, "ERROR: calloc() failed.\n", 24);
        my_exit(84);
    }
    push_to_stack(malloc_stack(), r);
    return r;
}

void *my_realloc(void *base, size_t new_size)
{
    void *r = realloc(base, new_size);

    if (!r) {
        write(2, "ERROR: realloc() failed.\n", 25);
        my_exit(84);
    }
    for (s_list_t *s = *malloc_stack(); s; s = s->next)
        if (s->data == base) {
            s->data = r;
            return r;
        }
    push_to_stack(malloc_stack(), r);
    return r;
}
