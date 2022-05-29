/*
** EPITECH PROJECT, 2022
** REPLACE_VARIABLE_C
** File description:
** replace_variable.c
*/

#include "minishell.h"

static char const *noa_better_get_field(char **env, char const *field,
char spec)
{
    int f_len = my_strlen(field);

    for (int i = 0; env[i]; i++)
        if (my_strncmp(env[i], field, f_len) == 0 && env[i][f_len] == spec)
            return env[i] + f_len + 1;
    return NULL;
}

static void get_variable(char **str, const char *source, int *len_var,
int index_var)
{
    for (int i = index_var; source[i]; i++, (*len_var)++)
        if (!char_is_alphanum(source[i]) && source[i] != '?')
            break;
    (*str) = malloc(sizeof(char) * (*len_var + 1));
    for (int i = 0; i < (*len_var) && source[index_var + i]; i++)
        (*str)[i] = source[index_var + i];
    (*str)[*len_var] = '\0';
}

static void loop_to_replace(char **str, int *next_index, char **env, char spec)
{
    char *tmp = NULL;
    char *str_to_cmp = NULL;
    int len_to_replace = 0;

    for (; (*str)[*next_index]; (*next_index)++)
        if (*next_index && (*str)[*next_index - 1] == '$')
            break;
    get_variable(&str_to_cmp, (*str), &len_to_replace, *next_index);
    tmp = (char *)noa_better_get_field(env, str_to_cmp, spec);
    if (tmp) {
        tmp = replace((*str), *next_index - 1, len_to_replace + 1, tmp);
        free((*str));
        (*str) = tmp;
    }
}

static void set_loop_to_replace(char **str, char **env, char spec)
{
    int nbr_of_var = 0;
    int next_index = 0;

    for (int j = 0; (*str)[j]; j++)
        nbr_of_var += (*str)[j] == '$' ? 1 : 0;
    while (nbr_of_var) {
        loop_to_replace(str, &next_index, env, spec);
        nbr_of_var--;
    }

}

void replace_all_variable(char **env, char **str, char spec)
{
    for (int i = 0; str[i]; i++)
        if (contain(str[i], '$'))
            set_loop_to_replace(&str[i], env, spec);
}
