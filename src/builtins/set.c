/*
** EPITECH PROJECT, 2021
** B-PSU-210-LIL-2-1-minishell2-quentin.desmettre
** File description:
** setvar.c
*/

#include "minishell.h"

int var_is_key(char const *test, char const *field)
{
    int i;

    for (i = 0; field[i] != '\t' && test[i] != '\t' && field[i] && test[i]; i++)
        if (test[i] != field[i])
            return 0;
    if (test[i] == 0 && field[i] == '\t')
        return 1;
    return 0;
}

int var_index_of_key(char **env, char const *key)
{
    if (env == NULL)
        return -1;
    for (int i = 0; env[i]; i++)
        if (var_is_key(key, env[i]))
            return i;
    return -1;
}

void var_setter(char ***var, int index, char **args)
{
    if (index < 0)
        create_var(var, args[0], args[1]);
    else
        set_var(var, index, args[0], args[1]);
}

int set(char **args, char ***var, __attribute__((unused))int o_fd, int is_pipe)
{
    if ((args[0][0] < 'A' || (args[0][0] > 'Z' && args[0][0] < 'a') ||
    (args[0][0] > 'z')) && args[0][0] != '_') {
        dprint(2, "set: Variable name must begin with a letter.\n");
        return 1;
    } else if (!str_is_alphanum(args[0] + 1)) {
        dprint(2, "set: "
        "Variable name must contain alphanumeric characters.\n");
        return 1;
    }
    if (!is_pipe)
        var_setter(var, var_index_of_key(*var, args[0]), args);
    return 0;
}

void setvar_pipe(char **args, char ***var, int o_fd, int is_pipe)
{
    char **arr = 0;
    char **array;
    int tmp = 0;

    arr = set_arr_var(args);
    if (!var_args_valid(args)) {
        args[1] ? set_last_exit(1) : var_pipe(var, o_fd);
        return;
    }
    for (int i = 0; arr[i]; i++) {
        array = loop_call_set(i, arr);
        tmp = set(array, var, o_fd, is_pipe);
        set_last_exit(tmp);
        my_free("P", array);
        if (tmp == 1)
            return;
    }
}
