/*
** EPITECH PROJECT, 2022
** ALIASES_IN_PARSE_C
** File description:
** aliases_in_parse.c
*/

#include "minishell.h"

void check_multi_words(char ***words, char **str, int *i)
{
    char **splitted;
    char **new;

    if (contain_any_of(*str, " \t")) {
        splitted = my_str_to_word_array(*str, " \t");
        new = place_arr_in_arr(*words, splitted, *i ? *i + 1 : *i);

        free(*words);
        *words = new;
        *i += my_str_array_len(splitted) - 1;
        free(splitted);
    }
}

void find_field_aliases(char **str, list_t *alias, char ***words, int *i)
{
    list_t *save = alias;
    replace_t *to_cmp = NULL;

    if (!(*str) || !save)
        return;
    do {
        to_cmp = save->data;
        if (my_strcmp(*str, to_cmp->name) == 0) {
            *str = to_cmp->value;
            check_multi_words(words, str, i);
            break;
        }
        save = save->next;
    } while (save != alias);
}

void replace_aliases_in_word_parse(char ***word_parse, list_t *alias)
{
    int i = 0;

    if (!alias)
        return;
    find_field_aliases(&(*word_parse)[0], alias, &(*word_parse), &i);
    i++;
    for (; (*word_parse)[i]; i++) {
        if (contain_any_of((*word_parse)[i], ";&|")) {
            find_field_aliases(&(*word_parse)[i + 1],
            alias, &(*word_parse), &i);
        }
    }
}
