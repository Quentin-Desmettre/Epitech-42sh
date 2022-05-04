/*
** EPITECH PROJECT, 2021
** B-PSU-101-LIL-1-1-minishell1-quentin.desmettre
** File description:
** main.c
*/

#include "minishell.h"

static int flush_stdin(char *err_mess)
{
    char t;

    if (my_strcmp(err_mess, NULL_CMD))
        return 0;
    if (!isatty(get_stdin()))
        while (read(get_stdin(), &t, 1) == 1);
    close_stdin();
    open_stdin();
    return 1;
}

static int contain_separator(char str, const char *specifier)
{
    int index = -1;

    for (int i = 0; specifier[i] != '\0'; i++)
        if (str == specifier[i]) {
            index = i;
            break;
        }
    return (index);
}

static void return_clean_string(char **tmp, char *string, int i, int *index)
{
    char *separator = "\n\t\r";

    if (string[i] == ' ') {
        if (i > 0 && contain_separator(string[i - 1], separator) < 0) {
            (*tmp)[*index] = string[i];
            (*index)++;
        }
    } else if (string[i] != ' '){
        (*tmp)[*index] = string[i];
        (*index)++;
    }
}

static char *clear_str(char *string)
{
    char *tmp;
    int index = 0;
    char *separator = "\n\t\r";
    int first_index = 0;
    int second_index = 0;
    size_t size = strlen(string);
    size_t nbr_separator = 0;

    for (int i = 0; string[i]; i++) {
        first_index = contain_separator(string[i], separator);
        if (string[i + 1])
            second_index = contain_separator(string[i + 1], separator);
        if (first_index == second_index && first_index != -1) {
            string[i] = '\\';
            string[i + 1] = ' ';
            while (first_index == second_index && string[i]) {
                i++;
                second_index = contain_separator(string[i], separator);
            }
        }
    }
    for (int i = 0; string[i]; i++) {
        if (contain_separator(string[i], separator) >= 0)
            nbr_separator++;
        if ((string[i] == ' ') && ((i > 0 && string[i - 1] != '\\')))
            nbr_separator++;
    }
    tmp = malloc(sizeof(char) * (size - nbr_separator + 1));
    for (int i = 0; string[i]; i++)
        if (contain_separator(string[i], separator) < 0)
            return_clean_string(&tmp, string, i, &index);
    tmp[index] = '\0';
    free(string);
    return tmp;
}

static char *new_string_index(char *input, int first_index, int second_index)
{
    char *str_return = malloc(sizeof(char) * (second_index - first_index + 1));
    int index = 0;

    for (int i = first_index; input[i] && i <= second_index; i++) {
        str_return[index] = input[i];
        index++;
    }
    str_return[index] = '\0';
    return str_return;
}

char *my_strset(int len_malloc)
{
    char *str = malloc(sizeof(char) * len_malloc);
    if (str == 0) {
        write(2, "Invalid malloc()\n", 17);
        exit(84);
    }
    for (int i = 0; i < len_malloc; str[i] = 0, i++);
    return str;
}

char *my_strndup(char const *src, int nb)
{
    int i = 0;
    char *dest = my_strset(nb + 1);
    while (i < nb) {
        dest[i] = src[i];
        i++;
    }
    return dest;
}

int is_in_str(char to_find, char const *str)
{
    for (int i = 0; str[i]; i++) {
        if (to_find == str[i])
            return 1;
    }
    return 0;
}

char **my_dbstrset(int len_malloc)
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
        if ((is_in_str(str[i], delimiters) || str[i] == 0) && start >= 0 && (i == 0 || str[i - 1] != '\\')) {
            tmp[count] = my_strndup(str + start, i - start);
            start = -1;
            count++;
        }
    }
    return (tmp);
}

static char **parse_input(char *input)
{
    char **word_parse = NULL;
    int last_separator_index = 0;
    int nbr_of_word = 1;
    char *str_separator = " ;&|";
    int index_separator = -1;
    int i;

    input = clear_str(input);
    word_parse = str_to_word_array(input, " ");
    return (word_parse);
}

static char **init_env(char **env)
{
    int len = my_str_array_len(env);
    char **new = malloc(sizeof(char *) * (len + 1));

    new[len] = NULL;
    for (int i = 0; env[i]; i++)
        new[i] = my_strdup(env[i]);
    signal(SIGINT, redirect_sigint);
    return new;
}

void print_my_input_parse(char **str)
{
    if (!str)
        return;
    for (int i = 0; str[i]; i++)
        printf("%s\n", str[i]);
}

int main(int ac, char **av, char **env)
{
    char *input;
    int stop = 0;
    char **tmp = NULL;

    if (ac != 1 || !env || !av)
        return 84;
    env = init_env(env);
    open_stdin();
    while (!stop) {
        set_is_exit(0);
        input = get_shell_input(env, &stop);
        tmp = parse_input(input);
        print_my_input_parse(tmp);
    }
    return get_last_exit();
}
