/*
** EPITECH PROJECT, 2021
** B-PSU-210-LIL-2-1-42sh-quentin.desmettre
** File description:
** echo.c
*/

#include "minishell.h"

char *get_oldwd(void)
{
    env_t *vars = global_env(NULL);
    char const *old = get_field(vars->vars, "owd\t");

    if (!old[0])
        old = get_field(vars->env, "OLDPWD=");
    return strdup(old);
}

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
