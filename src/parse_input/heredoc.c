/*
** EPITECH PROJECT, 2021
** B-PSU-210-LIL-2-1-minishell2-quentin.desmettre
** File description:
** heredoc.c
*/

#include "minishell.h"

int get_heredoc(char const *stop)
{
    int fds[2];
    char *buf;
    env_t *vars = global_env(NULL);

    set_reset_buffer(0);
    if (pipe(fds) < 0)
        return dprint(2, "pipe: %s.\n", strerror(errno)) ? -1 : -1;
    while (1) {
        write(1, "\033[s", 3);
        if (!(buf = get_command(NULL, vars->env, "? ")) || !strcmp(stop, buf)) {
            free(buf);
            break;
        }
        if (!buf[0] && is_reset_buf())
            return close_pipe(fds), set_reset_buffer(0), -1;
        dprint(fds[1], "%s\n", buf);
        free(buf);
    }
    close(fds[1]);
    return fds[0];
}
