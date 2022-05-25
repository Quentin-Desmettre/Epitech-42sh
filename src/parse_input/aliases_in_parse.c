/*
** EPITECH PROJECT, 2022
** ALIASES_IN_PARSE_C
** File description:
** aliases_in_parse.c
*/

#include "minishell.h"

void find_field_aliases(char **str, list_t *info)
{
    list_t *save = info;
    replace_t *to_cmp = NULL;

    if (!*str || !save)
        return;
    do {
        to_cmp = save->data;
        if (my_strcmp(*str, to_cmp->name) == 0) {
            *str = to_cmp->value;
            break;
        }
        save = save->next;
    } while (save != info);
}

void replace_aliases_in_word_parse(char **word_parse, list_t *vars)
{
    if (!vars)
        return;
    find_field_aliases(&word_parse[0], vars);
    for (int i = 1; word_parse[i]; i++) {
        if (contain_any_of(word_parse[i], ";&|")) {
            find_field_aliases(&word_parse[i + 1], vars);
        }
    }
}
