/*
** EPITECH PROJECT, 2021
** B-CPE-200-LIL-2-1-lemin-quentin.desmettre
** File description:
** append_node.c
*/

#include "libmy.h"

int list_size(list_t *list)
{
    int size = 0;
    list_t *begin = list;

    if (!begin)
        return 0;
    do {
        size++;
        list = list->next;
    } while (begin != list);
    return size;
}

void append_node(list_t **begin, void *data)
{
    list_t *new = malloc(sizeof(list_t));

    new->data = data;
    if (!(*begin)) {
        *begin = new;
        new->next = new;
        new->prev = new;
        return;
    }
    new->next = (*begin);
    new->prev = (*begin)->prev;
    (*begin)->prev->next = new;
    (*begin)->prev = new;
}

void remove_node(list_t **begin, unsigned offset, void (*freer)(void *))
{
    list_t *s = *begin;
    list_t *next;
    list_t *prev;

    if (!s)
        return;
    for (unsigned i = 0; i < offset; i++)
        s = s->next;
    if (freer)
        freer(s->data);
    if (s == s->next)
        *begin = NULL;
    else {
        prev = s->prev;
        next = s->next;
        prev->next = next;
        next->prev = prev;
        *begin = next;
    }
    free(s);
}

void destroy_list(list_t **list, void (*freer)(void *))
{
    while (*list)
        remove_node(list, 0, freer);
}

list_t *copy_list(list_t *used)
{
    list_t *begin = used;
    list_t *list = NULL;

    if (!used)
        return NULL;
    do {
        append_node(&list, used->data);
        used = used->next;
    } while (used != begin);
    return list;
}
