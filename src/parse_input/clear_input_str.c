/*
** EPITECH PROJECT, 2022
** CLEAR_INPUT_STR_C
** File description:
** clear_input_str.c
*/

#include "minishell.h"

int contain_separator(char str, const char *specifier)
{
    int index = -1;

    for (int i = 0; specifier[i] != '\0'; i++)
        if (str == specifier[i]) {
            index = i;
            break;
        }
    return (index);
}

static void count_separator_number(size_t *nbr_separator, char *string,
char *separator)
{
    for (int i = 0; string[i]; i++) {
        if (contain_separator(string[i], separator) >= 0)
            (*nbr_separator)++;
        if ((string[i] == ' ') && ((i > 0 && string[i - 1] != '\\')))
            (*nbr_separator)++;
    }
}

static void keep_space_in_str(char **string, int *i, char *separator)
{
    int first_index = contain_separator((*string)[*i], separator);
    int second_index = 0;

    if ((*string)[*i + 1])
        second_index = contain_separator((*string)[*i + 1], separator);
    if (first_index == second_index && first_index != -1 && (*string)[*i + 1]) {
        (*string)[*i] = '\\';
        (*string)[*i + 1] = ' ';
        while (first_index == second_index && (*string)[*i]) {
            (*i)++;
            second_index = contain_separator((*string)[*i], separator);
        }
    }
}

static void return_clean_string(char **tmp, char *string, int i, int *index)
{
    char *separator = "\t\r";

    if (string[i] == ' ') {
        if (i > 0 && contain_separator(string[i - 1], separator) < 0) {
            (*tmp)[*index] = string[i];
            (*index)++;
        }
    } else if (string[i] != ' ') {
        (*tmp)[*index] = string[i];
        (*index)++;
    }
}

char *clear_str(char *string)
{
    char *tmp;
    int index = 0;
    char *separator = "\t\r";
    size_t size = strlen(string);
    size_t nbr_separator = 0;

    for (int i = 0; string[i]; i++)
        string[i] = (string[i] == '\t') ? ' ' : string[i];
    for (int i = 0; string[i]; i++)
        keep_space_in_str(&string, &i, separator);
    count_separator_number(&nbr_separator, string, separator);
    tmp = malloc(sizeof(char) * (size + 1));
    my_memset(tmp, 0, (int)size);
    for (int i = 0; string[i]; i++)
        if (contain_separator(string[i], separator) < 0)
            return_clean_string(&tmp, string, i, &index);
    tmp[index] = '\0';
    free(string);
    return tmp;
}
