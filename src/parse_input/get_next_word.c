/*
** EPITECH PROJECT, 2021
** B-PSU-210-LIL-2-1-minishell2-quentin.desmettre
** File description:
** get_next_word.c
*/

#include "minishell.h"

char *get_next_word(char const *cmd, int *start, char **err_mess)
{
    char *word = my_strdup("");
    int i = *start + 1;

    if (!cmd[i] || contain("><|", cmd[i])) {
        free(word);
        return (*err_mess = "Missing name for redirect.") ? NULL : NULL;
    }
    while (cmd[i] == ' ' || cmd[i] == '\t')
        i++;
    if (!cmd[i] || contain("><|", cmd[i])) {
        free(word);
        return (*err_mess = "Missing name for redirect.") ? NULL : NULL;
    }
    while (cmd[i] && !contain("<>| \t", cmd[i])) {
        append_char(&word, cmd[i], 1);
        i++;
    }
    *start = i - 1;
    return word;
}

int can_get_next_word(char const *cmd, int start, char **err_mess)
{
    char *tmp = get_next_word(cmd, &start, err_mess);

    if (!tmp)
        return 0;
    free(tmp);
    return 1;
}
