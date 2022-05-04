/*
** EPITECH PROJECT, 2021
** B-PSU-210-LIL-2-1-minishell2-quentin.desmettre
** File description:
** setenv.c
*/

#include "minishell.h"

static void set_env(char ***env, int index, char const *key, char const *val)
{
    char *buf = "";

    append(&buf, key, 0);
    append(&buf, "=", 1);
    if (val)
        append(&buf, val, 1);
    free((*env)[index]);
    (*env)[index] = buf;
}

static void create_env(char ***env, char const *key, char const *val)
{
    char *buf = "";

    append(&buf, key, 0);
    append(&buf, "=", 1);
    if (val)
        append(&buf, val, 1);
    append_str_array(env, buf);
}

static int are_args_valid(char **args)
{
    int is_error;

    if (!args[1])
        return 0;
    if (args[2] && args[3])
        return (dprint(2, "setenv: Too many arguments.\n") ? 0 : 0);
    my_getnbr(args[1], &is_error);
    return 1;
}

static void env_setter(char ***env, int index, char **args)
{
    if (index < 0)
        create_env(env, args[1], args[2]);
    else
        set_env(env, index, args[1], args[2]);
}

void setenv_pipe(char **args, char ***env, int o_fd, int is_pipe)
{
    if (!are_args_valid(args)) {
        args[1] ? set_last_exit(1) : env_pipe(args, env, o_fd);
        return;
    }
    if ((args[1][0] < 'A' || (args[1][0] > 'Z' && args[1][0] < 'a') ||
    (args[1][0] > 'z')) && args[1][0] != '_') {
        dprint(2, "setenv: Variable name must begin with a letter.\n");
        set_last_exit(1);
    } else if (!str_is_alphanum(args[1])) {
        dprint(2, "setenv: "
        "Variable name must contain alphanumeric characters.\n");
        set_last_exit(1);
    } else {
        set_last_exit(0);
        return is_pipe ? 0 :
        env_setter(env, index_of_key(*env, args[1]), args);
    }
}
