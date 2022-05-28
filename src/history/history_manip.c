/*
** EPITECH PROJECT, 2022
** B-PSU-210-LIL-2-1-42sh-quentin.desmettre
** File description:
** history_manip
*/

#include "minishell.h"

char *glob_history(char *new)
{
    static char *path = NULL;

    if (new != NULL)
        path = new;
    return path;
}

void set_history_path(char **env)
{
    char const *home = get_field(env, "HOME=");
    char *path = malloc(sizeof(char) * (my_strlen(home) + 15));

    path = str_concat(2, home, "/.42sh_history");
    glob_history(path);
}

int history_cmp(char *str1, char *str2)
{
    int len1 = my_strlen(str1);
    int len2 = my_strlen(str2);

    if (len1 > 0 && str1[len1 - 1] == '\n')
        len1--;
    if (len2 > 0 && str2[len2 - 1] == '\n')
        len2--;
    if (len1 != len2)
        return 1;
    if (!strncmp(str1, str2, len1))
        return 0;
    return 1;
}

int append_history_node(hist_t **head, hist_t *new)
{
    hist_t *tmp = *head;

    if (!(*head)) {
        *head = new;
        new->next = NULL;
        new->prev = NULL;
        return 1;
    }
    while (tmp->next)
        tmp = tmp->next;
    if (!history_cmp(tmp->command, new->command))
        return 0;
    tmp->next = new;
    new->prev = tmp;
    new->next = NULL;
    return 1;
}

void reset_history(hist_t **history)
{
    hist_t *tmp = *history;

    while (tmp) {
        tmp->select = 0;
        tmp = tmp->next;
    }
}
