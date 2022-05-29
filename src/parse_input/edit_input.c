/*
** EPITECH PROJECT, 2022
** EDIT_INPUT_C
** File description:
** edit_input.c
*/

#include "minishell.h"

static char *add_separator(char *separator, char *input)
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

char **error_on_variable(char **str)
{
    int i;
    int index = 0;
    char *tmp = NULL;

    for (i = 0; str[i]; i++)
        if (contain(str[i], '$')) {
            tmp = str[i];
            index = index_of('$', str[i]);
            break;
        }
    if (!tmp)
        return (str);
    for (int j = index + 1; str[i][j] && str[i][j] != ' ' &&
    str[i][j] != '$'; j++)
        write(2, &(str[i][j]), 1);
    write(2, ": Undefined variable.\n", 22);
    set_last_exit(1);
    return (NULL);
}

char **split_words(char *input, env_t *vars)
{
    char **word_parse = NULL;
    char *str_separator = ";&|><";

    input = clear_str(input);
    input = add_separator(str_separator, input);
    word_parse = str_to_word_array(input, " ");
    find_all_back_slash(word_parse);
    replace_aliases_in_word_parse(&word_parse, vars->aliases->commands);
    replace_all_variable(vars->vars, word_parse, '\t');
    replace_all_variable(vars->env, word_parse, '=');
    for (int i = 0; word_parse[i]; i++)
        word_parse[i] = replace_history(word_parse[i], vars);
    if (word_parse && !word_parse[0])
        return (NULL);
    return (error_on_variable(word_parse));
}
