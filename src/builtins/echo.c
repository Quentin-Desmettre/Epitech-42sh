/*
** EPITECH PROJECT, 2021
** B-PSU-210-LIL-2-1-42sh-quentin.desmettre
** File description:
** echo.c
*/

#include "minishell.h"

void echo_builtin(char **args, char ***env, int o_fd, int is_pipe)
{
    (void)is_pipe;
    (void)env;

    for (int i = 1; args[i]; i++) {
        write(o_fd, args[i], strlen(args[i]));
        if (args[i + 1])
            write(o_fd, " ", 1);
    }
    write(o_fd, "\n", 1);
}
