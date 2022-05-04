/*
** EPITECH PROJECT, 2021
** B-PSU-210-LIL-2-1-minishell2-quentin.desmettre
** File description:
** env.c
*/

#include "minishell.h"

int exec_builtin_fd(char **args, char ***env, int fds[2], int is_pipe)
{
    void (*builtin[5])(char **, char ***, int, int) = {
        &cd_pipe, &setenv_pipe, &unsetenv_pipe, NULL, NULL
    };
    char *builtins[5] = {
        "cd", "setenv", "unsetenv", "env", "exit"
    };
    int index = index_str_in_array(builtins, args[0]);

    if (index < 3)
        builtin[index](args, env, fds[1], is_pipe);
    else if (index == 3)
        env_pipe(args, env, fds[1]);
    else
        exit_pipe(args, is_pipe);
    return 0;
}

int is_builtin(char const *word)
{
    char *builtins[5] = {
    "cd", "setenv", "unsetenv", "env", "exit"
    };

    for (int i = 0; i < 5; i++)
        if (my_strcmp(word, builtins[i]) == 0)
            return 1;
    return 0;
}

void env_pipe(char **args, char ***e, int o_fd)
{
    char **env = *e;

    if (args[1]) {
        dprint(2, "env: Too many arguments.\n");
        set_last_exit(1);
        return;
    }
    for (int i = 0; env[i]; i++)
        dprint(o_fd, "%s\n", env[i]);
    set_last_exit(0);
}
