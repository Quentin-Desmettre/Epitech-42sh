/*
** EPITECH PROJECT, 2021
** B-PSU-210-LIL-2-1-minishell2-quentin.desmettre
** File description:
** heredoc.c
*/

#include "minishell.h"

static void sigint_heredoc(int id)
{
    if (id != SIGINT)
        return;
    close_stdin();
    set_reset_buffer(1);
}

static void reset(char **line)
{
    re_alloc(line, NULL, 1);
    set_reset_buffer(0);
    open_stdin();
    write(1, "\n", 1);
}

static char *getline_heredoc(char const *pattern)
{
    char *line = my_strdup("");
    char tmp;
    int c;
    signal(SIGINT, sigint_heredoc);
    while (1) {
        c = read(get_stdin(), &tmp, 1);
        if (is_reset_buf()) {
            reset(&line);
            break;
        }
        if (c == 0 && !line[0]) {
            re_alloc(&line, my_strdup(pattern), 1);
            break;
        }
        if (tmp == '\n')
            break;
        c ? append_char(&line, tmp, 1) : 0;
    }
    signal(SIGINT, redirect_sigint);
    return line;
}

int get_heredoc(char const *stop)
{
    int fds[2];
    char *buf;
    if (pipe(fds) < 0)
        return dprint(2, "pipe: %s.\n", strerror(errno)) ? -1 : -1;
    while (1) {
        print("? ");
        buf = getline_heredoc(stop);
        if (!buf) {
            close_pipe(fds);
            return -1;
        }
        if (!my_strcmp(stop, buf)) {
            free(buf);
            break;
        }
        dprint(fds[1], "%s\n", buf);
        free(buf);
    }
    close(fds[1]);
    return fds[0];
}
