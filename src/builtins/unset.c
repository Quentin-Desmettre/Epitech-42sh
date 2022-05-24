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

void unsetvar_pipe(char **args, char ***e, int o_fd, int is_pipe)
{
    int index = o_fd;
    char **vars = *e;

    if (!unvar_args_valid(args))
        return;
    set_last_exit(0);
    if (is_pipe)
        return;
    for (int i = 1; args[i]; i++) {
        index = var_index_of_key(vars, args[i]);
        if (index < 0)
            continue;
        free(vars[index]);
        rotate_var(vars, index);
    }
}
