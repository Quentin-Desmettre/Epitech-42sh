/*
** EPITECH PROJECT, 2021
** B-PSU-210-LIL-2-1-minishell2-quentin.desmettre
** File description:
** heredoc.c
*/

#include "minishell.h"

static int read_stdin_lines(int fds[2], char const *stop)
{
    char *buf = NULL;

    while ((buf = get_next_line(buf))) {
        if (!my_strcmp(buf, stop))
            break;
        dprint(fds[1], "%s\n", buf);
    }
    close(fds[1]);
    return fds[0];
}

static int get_edited_lines(int fds[2], char const *stop, env_t *vars)
{
    char *buf;

    while (1) {
        write(1, "\033[s", 3);
        buf = get_command(NULL, "? ", &vars->history);
        if (!buf || (!strcmp(stop, buf) && buf[0])) {
            free(buf);
            break;
        }
        if (!buf[0] && is_reset_buf()) {
            close_pipe(fds);
            set_reset_buffer(0);
            return -1;
        }
        dprint(fds[1], "%s\n", buf);
        free(buf);
    }
    close(fds[1]);
    return fds[0];
}

int get_heredoc(char const *stop)
{
    int fds[2];

    set_reset_buffer(0);
    if (pipe(fds) < 0)
        return dprint(2, "pipe: %s.\n", strerror(errno)) ? -1 : -1;
    if (!isatty(0))
        return read_stdin_lines(fds, stop);
    return get_edited_lines(fds, stop, global_env(NULL));
}
