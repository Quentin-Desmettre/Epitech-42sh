/*
** EPITECH PROJECT, 2022
** B-PSU-210-LIL-2-1-42sh-quentin.desmettre
** File description:
** history
*/

#include "minishell.h"

void clear_history(hist_t **history)
{
    hist_t *tmp = *history;
    int fd = open(glob_history(NULL), O_RDWR | O_CREAT | O_TRUNC, 0644);

    if (fd != -1)
        close(fd);
    while (tmp) {
        *history = tmp->next;
        free(tmp->command);
        free(tmp);
        tmp = *history;
    }
}

void print_history(hist_t *history, int o_fd)
{
    hist_t *tmp = history;

    for (int i = 1; tmp; tmp = tmp->next, i++) {
        dprint(o_fd, "%6i  %s", i, tmp->command);
        if (my_strlen(tmp->command) > 0 &&
        tmp->command[my_strlen(tmp->command) - 1] != '\n')
            dprint(o_fd, "\n");
    }
}

void history_builtin(char **args, hist_t **history, int o_fd, int is_pipe)
{
    set_last_exit(0);
    if ((args[1] && args[2]) || (args[1] && my_strcmp(args[1], "-c") != 0)) {
        dprint(2, "Usage: history [-c].\n");
        set_last_exit(1);
        return;
    }
    if (args[1] && my_strcmp(args[1], "-c") == 0 && !is_pipe) {
        clear_history(history);
        return;
    }
    if (args[1] == NULL)
        print_history(*history, o_fd);
}
