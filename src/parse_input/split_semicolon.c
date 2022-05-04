/*
** EPITECH PROJECT, 2021
** B-PSU-210-LIL-2-1-minishell2-quentin.desmettre
** File description:
** split_semicolon.c
*/

#include "minishell.h"

list_t *split_semicolons(char const *input)
{
    char **words = my_str_to_word_array(input, ";");
    list_t *list = NULL;

    for (int i = 0; words[i]; i++)
        append_node(&list, words[i]);
    free(words);
    return list;
}
