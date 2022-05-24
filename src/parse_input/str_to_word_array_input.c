/*
** EPITECH PROJECT, 2022
** STR_TO_WORD_ARRAY_INPUT_C
** File description:
** str_to_word_array_input.c
*/

#include "minishell.h"

static char *my_strset(int len_malloc)
{
    char *str = malloc(sizeof(char) * len_malloc);

    if (str == 0) {
        write(2, "Invalid malloc()\n", 17);
        exit(84);
    }
    for (int i = 0; i < len_malloc; str[i] = 0, i++);
    return str;
}

static char *my_strndup(char const *src, int nb)
{
    int i = 0;
    char *dest = my_strset(nb + 1);

    while (i < nb) {
        dest[i] = src[i];
        i++;
    }
    return dest;
}

static int is_in_str(char to_find, char const *str)
{
    for (int i = 0; str[i]; i++) {
        if (to_find == str[i])
            return 1;
    }
    return 0;
}

static char **my_dbstrset(int len_malloc)
{
    char **str = malloc(sizeof(char *) * len_malloc);
    if (str == 0) {
        write(2, "Invalid malloc()\n", 17);
        exit(84);
    }
    for (int i = 0; i < len_malloc; str[i] = 0, i++);
    return str;
}

char **str_to_word_array(char const *str, char *delimiters)
{
    int count = 0;
    int start = -1;
    int len = my_strlen(str);
    char **tmp = my_dbstrset(len + 1);

    for (int i = 0; i <= len; i++) {
        if (!is_in_str(str[i], delimiters) && str[i] != 0 && start == -1)
            start = i;
        if ((is_in_str(str[i], delimiters) || str[i] == 0)
        && start >= 0 && (i == 0 || str[i - 1] != '\\')) {
            tmp[count] = my_strndup(str + start, i - start);
            start = -1;
            count++;
        }
    }
    return (tmp);
}
