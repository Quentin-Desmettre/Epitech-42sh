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

int verif_probe(char *str)
{
    int i = 0;

    while (str[i]) {
        if (str[i] == '*' || str[i] == '?')
            return 1;
        i++;
    }
    return 0;
}

int search_pattern(char *pattern, char *str)
{
    char **arr = my_str_to_word_array(pattern, "*");
    char *tmp = 0;
    int space = 0;
    int i = 0;
    if (arr[0] == 0 && pattern[0] == '*') {
        my_free("P", arr);
        return 1;
    }
    for (i = 0; arr[i]; space = tmp - str + strlen(arr[i]), i++) {
        tmp = strstr(str + space, arr[i]);
        if (tmp == NULL) {
            my_free("P", arr);
            return 0;
        }
    }
    space = ((pattern[strlen(pattern) - 1] != '*' && strcmp(str + strlen(str) -
    strlen(arr[i - 1]), arr[i - 1]) != 0) || (pattern[0] != '*' &&
    strstr(str, arr[0]) != str)) ? 0 : 1;
    my_free("P", arr);
    return space;
}

void one_unalias(list_t **c, char *av)
{
    list_t *tmp = *c;
    replace_t *cmd = 0;
    int is_pattern = 0;

    if (*c == 0)
        return;
    for (int i = 0; (i == 0 || (tmp != *c)) && *c != 0; i++) {
        cmd = tmp->data;
        is_pattern = search_pattern(av, cmd->name);
        printf("%s %d\n", cmd->name, is_pattern);
        if (is_pattern) {
            remove_node(c, i, rm_alias);
            i = -1;
            tmp = *c;
        }
        if (*c != 0)
            tmp = tmp->next;
    }
}

void unalias(char **args, env_t *e,  __attribute__((unused))int o_fd,
int is_pipe)
{
    int size = my_str_array_len(args);

    if (size == 1) {
        my_putstr_error("unalias: Too few arguments.");
        set_exit_status(1);
        return;
    }
    set_exit_status(0);
    if (e->aliases->commands)
        check_glob_unalias(&args, e->aliases->commands);
    for (int i = 1; i < size && is_pipe == 0; i++)
        one_unalias(&e->aliases->commands, args[i]);
}
