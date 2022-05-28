/*
** EPITECH PROJECT, 2021
** B-PSU-210-LIL-2-1-minishell2-quentin.desmettre
** File description:
** unsetvar.c
*/

#include "minishell.h"

int unvar_args_valid(char **args)
{
    if (!args[1]) {
        dprint(2, "unset: Too few arguments.\n");
        return 0;
    }
    return 1;
}

static void rotate_var(char **vars, int start)
{
    for (int i = start; vars[i]; i++) {
        vars[i] = vars[i + 1];
        if (!vars[i])
            break;
    }
}

char **resolve_unset_globbings(char **args, char **vars)
{
    int len = my_str_array_len(vars);
    char **splitted_vars = malloc(sizeof(char *) * (len + 1));
    char **tmp;
    char **solved;

    splitted_vars[len] = NULL;
    for (int i = 0; vars[i]; i++) {
        tmp = my_str_to_word_array(vars[i], "\t");
        splitted_vars[i] = tmp[0];
        for (int j = 1; tmp[j]; j++)
            free(tmp[j]);
        free(tmp);
    }
    solved = resolve_globbings(args, splitted_vars);
    free_str_array(splitted_vars, 0);
    return solved;
}

void unsetvar_pipe(char **args, char ***e, int o_fd, int is_pipe)
{
    int index = o_fd;
    char **vars = *e;

    if (!unvar_args_valid(args))
        return;
    set_last_exit(0);
    if (is_pipe)
        return;
    args = resolve_unset_globbings(args, vars);
    for (int i = 0; args[i]; i++) {
        index = var_index_of_key(vars, args[i]);
        if (index < 0)
            continue;
        free(vars[index]);
        rotate_var(vars, index);
    }
    free_str_array(args, 0);
}
