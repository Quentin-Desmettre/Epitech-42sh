/*
** EPITECH PROJECT, 2021
** B-PSU-101-LIL-1-1-minishell1-quentin.desmettre
** File description:
** main.c
*/

#include "minishell.h"

void print_alias(list_t **c, int fd)
{
    list_t *tmp = *c;
    replace_t *cmd = 0;

    if (*c == 0)
        return;
    do {
        cmd = tmp->data;
        dprintf(fd, "%s\t%s\n", cmd->name, cmd->value);
        tmp = tmp->next;
    } while (tmp != *c);
}

void print_one_alias(list_t **c, int fd, char *av)
{
    list_t *tmp = *c;
    replace_t *cmd = 0;

    if (*c == 0)
        return;
    for (int i = 0; i == 0 || tmp != *c; i++) {
        cmd = tmp->data;
        if (search_pattern(av, cmd->name))
            dprintf(fd, "%s\t%s\n", cmd->name, cmd->value);
        tmp = tmp->next;
    }
}

void set_value(char **args, replace_t *new)
{
    int size = my_str_array_len(args);
    char *all = malloc(1);

    all[0] = '\0';
    for (int i = 2; i < size; i++) {
        all = realloc(all, strlen(args[i]) + strlen(all) + 2);
        if (i != 2)
            strcat(all, " ");
        strcat(all, args[i]);
    }
    new->name = strdup(args[1]);
    new->value = all;
}

void add_alias(char **args, list_t **c, int is_pipe)
{
    list_t *tmp = *c;
    replace_t *cmd = 0;
    replace_t *new = malloc(sizeof(replace_t));

    if (is_pipe)
        return;
    set_value(args, new);
    if (*c == 0 || strcmp(((replace_t *)tmp->data)->name, args[1]) > 0) {
        append_node(c, new);
        *c = (*c)->prev;
        return;
    }
    for (int i = 0; *c != 0 && (i == 0 || tmp != *c); tmp = tmp->next, i++) {
        cmd = tmp->data;
        if (strcmp(cmd->name, args[1]) == 0) {
            free(cmd->value);
            cmd->value = strdup(new->value);
            return rm_alias(new);
        }
        if (strcmp(cmd->name, args[1]) < 0)
            return append_node(&tmp, new);
    }
}

void alias(char **args, env_t *e, int o_fd, int is_pipe)
{
    int size = my_str_array_len(args);

    set_exit_status(0);
    if (size == 1)
        print_alias(&e->aliases->commands, o_fd);
    if (size == 2)
        print_one_alias(&e->aliases->commands, o_fd, args[1]);
    if (size >= 3 && is_pipe == 0)
        add_alias(args, &e->aliases->commands, is_pipe);
}
