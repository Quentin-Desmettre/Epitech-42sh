/*
** EPITECH PROJECT, 2022
** GLOBING_C
** File description:
** globing.c
*/

#include "minishell.h"

static int check_bracket(char const *str)
{
    int open_bracket = 0;
    int char_between = 0;
    int close_bracket = 0;

    for (int i = 0; str[i]; i++) {
        if (str[i] == '[')
            open_bracket = 1;
        if (str[i] == ']')
            close_bracket = 1;
        if (open_bracket && !close_bracket && str[i] != '[')
            char_between = 1;
        if (close_bracket && !open_bracket)
            return 0;
        if (open_bracket && close_bracket) {
            return (char_between ? 1 : 0);
        }
    }
    return 0;
}

static int check_stars_or_interrogative(char ***args, int *i)
{
    char **tmp;
    int len = 0;

    if (contain((*args)[*i], '*') || contain((*args)[*i], '?') ||
    check_bracket((*args)[*i])) {
        tmp = get_glob((*args)[*i]);
        if (!tmp || !tmp[0]) {
            dprint(2, "%s: No match.\n", (*args)[0]);
            return 1;
        } else {
            len = my_str_array_len(tmp);
            (*args) = place_arr_in_arr((*args), tmp, *i);
            (*i) += len - 1;
            tmp = NULL;
        }
    }
    return 0;
}

int not_global(char ***args)
{
    for (int i = 0; (*args)[i]; i++)
        if (check_stars_or_interrogative(args, &i))
            return 1;
    return 0;
}
