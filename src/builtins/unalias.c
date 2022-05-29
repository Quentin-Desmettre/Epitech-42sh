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

int is_pattern(char const *pattern, char const *check)
{
    return !fnmatch(pattern, check, FN_FLAGS);
}

void one_unalias(list_t **c, char *av)
{
    list_t *tmp = *c;
    replace_t *cmd = 0;
    int i = 0;
    int is_head = 0;

    if (*c == 0)
        return;
    do {
        cmd = tmp->data;
        if (is_pattern(av, cmd->name)) {
            remove_node(c, i, rm_alias);
            tmp = *c;
            i = 0;
            is_head = 1;
            continue;
        }
        is_head = 0;
        i++;
        tmp = tmp->next;
    } while (*c && (tmp != *c || is_head));
}

void unalias(char **args, env_t *e, __attribute__((unused))int o_fd,
int is_pipe)
{
    int size = my_str_array_len(args);

    if (size == 1) {
        my_putstr_error("unalias: Too few arguments.");
        set_last_exit(1);
        return;
    }
    set_last_exit(0);
    if (is_pipe)
        return;
    for (int i = 1; i < size; i++)
        one_unalias(&e->aliases->commands, args[i]);
}
