/*
** EPITECH PROJECT, 2022
** EDIT_INPUT_C
** File description:
** edit_input.c
*/

#include "minishell.h"

char *add_separator(char *separator, char *input)
{
    int separator_type = -1;
    int before = -1;

    for (int i = 0; input[i]; i++) {
        separator_type = contain_separator(input[i], separator);
        if (separator_type >= 0 &&
        before != separator_type && i > 0 && input[i - 1] != ' ')
            input = replace(input, i, 0, " ");
        if (separator_type == -1 && before != -1 && input[i] != ' ')
            input = replace(input, i, 0, " ");
        before = separator_type;
    }
    return (input);
}
