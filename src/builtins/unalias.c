/*
** EPITECH PROJECT, 2021
** B-PSU-101-LIL-1-1-minishell1-quentin.desmettre
** File description:
** main.c
*/

#include "minishell.h"
#include <glob.h>

void rm_alias(void *alias)
{
    replace_t *tmp = alias;

    free(tmp->name);
    free(tmp->value);
    free(tmp);
}

void one_unalias(list_t **c, int fd, char *av)
{
    list_t *tmp = *c;
    replace_t *cmd = 0;

    if (*c == 0)
        return;
    for (int i = 0; i == 0 || (tmp != *c); i++) {
        cmd = tmp->data;
        if (strcmp(cmd->name, av) == 0) {
            remove_node(c, i, rm_alias);
            return;
        }
        tmp = tmp->next;
    }
}

void unalias(char **args, env_t *e, int o_fd, int is_pipe)
{
    int size = my_str_array_len(args);

    if (size == 1) {
        my_putstr_error("unalias: Too few arguments.");
        set_exit_status(1);
        return;
    }
    set_exit_status(0);
    for (int i = 1; i < size && is_pipe == 0; i++)
        one_unalias(&e->aliases->commands, o_fd, args[i]);
}
