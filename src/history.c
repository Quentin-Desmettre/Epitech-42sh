/*
** EPITECH PROJECT, 2022
** B-PSU-210-LIL-2-1-42sh-quentin.desmettre
** File description:
** history of the 42
*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include "../include/minishell.h"

void free_history_list(histo_t *head, histo_t *temp)
{
    while (head->next != NULL)
    {
        temp = head->next;
        free(head->command);
        free(head);
        head = temp;
    }
}

void history_append(char *command, histo_t *head)
{
    int fd = open("history.txt", O_WRONLY | O_APPEND | O_CREAT, 0644);

    if (fd < 0)
        return;
    push_node(head, init_node(cp(command, 0)));
    write(fd, command, count_line(command));
    write(fd, "\n", 1);
    close(fd);
}

char *is_down(histo_t *head)
{
    while (head->next != NULL) {
        if (head->select == 1) {
            head->select = 0;
            head->next->select = 1;
            return head->next->command;
        }
        head = head->next;
    }
    return head->command;
}

char *is_up(histo_t *head)
{
    while (head->next != NULL) {
        if (head->next->select == 1) {
            head->next->select = 0;
            head->select = 1;
            return head->command;
        }
        head = head->next;
    }
    head->select = 1;
    return head->command;
}

histo_t *init_history()
{
    histo_t *head = init_node(cp("fd\n", 0));
    histo_t *temp;

    give("history.txt", head);
    if (head->next != NULL) {
        temp = head->next;
        free(head->command);
        free(head);
        head = temp;
    }
    return head;
}
