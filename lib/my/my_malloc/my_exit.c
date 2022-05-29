/*
** EPITECH PROJECT, 2021
** B-CPE-201-LIL-2-1-corewar-noa.trachez
** File description:
** my_exit.c
*/

#include <stdlib.h>
#include "linked_list.h"
#include <stdio.h>

s_list_t **malloc_stack(void);
void flush_garbage(void);
int is_free_stdin(void);

void my_exit(int code)
{
    flush_garbage();
    if (is_free_stdin())
        fclose(stdin);
    exit(code);
}

void flush_garbage(void)
{
    s_list_t *next;
    s_list_t *head = *(malloc_stack());

    while (head) {
        free(head->data);
        next = head->next;
        free(head);
        head = next;
    }
    *(malloc_stack()) = NULL;
}

void my_super_free(void *data)
{
    s_list_t *prev = NULL;
    s_list_t *head = *(malloc_stack());

    free(data);
    while (head) {
        if (head->data == data) {
            prev ?
            (prev->next = head->next) :
            (*(malloc_stack()) = head->next);
            free(head);
            return;
        }
        prev = head;
        head = head->next;
    }
}
