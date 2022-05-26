/*
** EPITECH PROJECT, 2022
** DELETE_BACKSLASH_C
** File description:
** delete_backslash.c
*/

#include "minishell.h"

static void delete_backslash(char **str, int i)
{
    int index;
    char *tmp = NULL;

    for (int j = 0; str[i][j]; j++) {
        if (str[i][j] == '\\' && str[i][j + 1] != '\\') {
            index = j;
            tmp = replace(str[i], index, 1, "");
            free(str[i]);
            str[i] = tmp;
        }
    }
}

void find_all_back_slash(char **str)
{
    for (int i = 0; str[i]; i++)
        if (contain(str[i], '\\'))
            delete_backslash(str, i);
}
