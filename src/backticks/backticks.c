/*
** EPITECH PROJECT, 2021
** B-PSU-210-LIL-2-1-42sh-quentin.desmettre
** File description:
** backticks.c
*/

#include "minishell.h"

backtick_t *create_backtick(char *input, int start_pos)
{
    backtick_t *b = malloc(sizeof(backtick_t));

    b->line = strdup("");
    b->length = 0;
    for (int i = start_pos; input[i] != '`'; i++) {
        append_char(&b->line, input[i], 1);
        b->length++;
    }
    return b;
}

char *my_clear_str(char *str)
{
    char **words = my_str_to_word_array(str, " \t\n");
    char *new = calloc(sizeof(char), 10);
    int len = 0;
    int max_len = 10;
    int word_len;

    for (int i = 0; words[i]; i++) {
        word_len = strlen(words[i]);
        while (len + word_len + 3 >= max_len) {
            new = realloc(new, max_len * 2);
            max_len *= 2;
        }
        strncpy(new + len, words[i], word_len);
        new[len + word_len] = ' ';
        new[len + word_len + 1] = 0;
        len += word_len + 1;
    }
    free_str_array(words, 1);
    free(str);
    return new;
}

char *read_fd(int fd)
{
    char *buf = calloc(sizeof(char), 10);
    int len = 0;
    int max_len = 10;
    char tmp;

    while (read(fd, &tmp, 1)) {
        if (len + 2 > max_len) {
            buf = realloc(buf, max_len * 2);
            max_len *= 2;
        }
        buf[len] = tmp;
        buf[len + 1] = 0;
        len++;
    }
    return my_clear_str(buf);
}

char *get_command_output(char *cmd, env_t *vars)
{
    int fds[2];
    char *buf;

    if (pipe(fds)) {
        dprint(2, "Broken pipe.\n");
        return my_strdup("");
    }
    set_final_fd(fds[1]);
    new_parse_input(strdup(cmd), vars);
    close(fds[1]);
    set_final_fd(-1);
    buf = read_fd(fds[0]);
    close(fds[0]);
    return buf;
}

int parse_for_backticks(char **input, env_t *vars)
{
    char *new_string;
    backtick_t *tmp;

    if (count_occurences('`', *input) % 2) {
        dprint(2, "Unmatched '`'.\n");
        set_last_exit(1);
        return 0;
    }
    for (int i = 0; (*input)[i]; i++)
        if ((*input)[i] == '`') {
            tmp = create_backtick(*input, i + 1);
            new_string = get_command_output(tmp->line, vars);
            re_alloc(input, replace(*input, i, tmp->length + 2, new_string), 1);
            i += strlen(new_string) - 1;
            free(new_string);
            free(tmp->line);
            free(tmp);
        }
    return 1;
}
