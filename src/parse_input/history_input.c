/*
** EPITECH PROJECT, 2022
** B-PSU-210-LIL-2-1-42sh-quentin.desmettre
** File description:
** history_input
*/

#include "minishell.h"

int is_history_error(char *str)
{
    int error = 0;
    int nb = my_getnbr(str, &error);

    if (error || nb == 0)
        return 1;
    return 0;
}

char *get_hist_cmd(hist_t *history, int nb)
{
    hist_t *tmp = history;

    if (nb < 0) {
        while (tmp->next)
            tmp = tmp->next;
        nb = -nb;
        for (int i = 0; i < nb - 1 && tmp->prev; i++)
            tmp = tmp->prev;
        return tmp->command;
    }
    for (int i = 0; i < nb - 1 && tmp->next; i++)
        tmp = tmp->next;
    return tmp->command;
}

char *replace_history_cmd(char *str, hist_t *history, int start, int len)
{
    int error = 0;
    int nb = 0;
    char *new;
    char *cmd;

    if (len == 0 || is_history_error(str + start + 1))
        return str;
    nb = my_getnbr(str + start + 1, &error);
    cmd = get_hist_cmd(history, nb);
    if (cmd[my_strlen(cmd) - 1] == '\n')
        cmd[my_strlen(cmd) - 1] = 0;
    new = replace(str, start, len + 1, cmd);
    free(str);
    return new;
}

char *replace_history(char *str, env_t *vars)
{
    int start = -1;
    int len = 0;

    for (int i = 0; str[i]; i++) {
        if (str[i] == '!') {
            start = i;
            break;
        }
    }
    if (start == -1)
        return str;
    for (int i = start + 1; str[i]; i++, len++) {
        if (str[i] == ' ' || str[i] == '\t')
            break;
    }
    return replace_history_cmd(str, vars->history, start, len);
}
