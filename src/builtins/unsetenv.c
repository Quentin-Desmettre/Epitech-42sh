/*
** EPITECH PROJECT, 2021
** B-PSU-210-LIL-2-1-minishell2-quentin.desmettre
** File description:
** unsetenv.c
*/

#include "minishell.h"

static int are_args_valid(char **args)
{
    if (!args[1]) {
        dprint(2, "unsetenv: Too few arguments.\n");
        return 0;
    }
    return 1;
}

static int is_key(char const *test, char const *field)
{
    int i;

    for (i = 0; field[i] != '=' && test[i] != '=' && field[i] && test[i]; i++)
        if (test[i] != field[i])
            return 0;
    if (test[i] == 0 && field[i] == '=')
        return 1;
    return 0;
}

int index_of_key(char **env, char const *key)
{
    for (int i = 0; env[i]; i++)
        if (is_key(key, env[i]))
            return i;
    return -1;
}

static void rotate_env(char **envs, int start)
{
    for (int i = start; envs[i]; i++) {
        envs[i] = envs[i + 1];
        if (!envs[i])
            break;
    }
}

void unsetenv_pipe(char **args, char ***e, int o_fd, int is_pipe)
{
    int index = o_fd;
    char **envs = *e;

    if (!are_args_valid(args))
        return;
    set_last_exit(0);
    if (is_pipe)
        return;
    args = resolve_unsetenv_globbings(args, *e);
    for (int i = 0; args[i]; i++) {
        index = index_of_key(envs, args[i]);
        if (index < 0)
            continue;
        free(envs[index]);
        rotate_env(envs, index);
    }
    free_str_array(args, 0);
}
