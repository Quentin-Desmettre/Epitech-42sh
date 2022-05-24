/*
** EPITECH PROJECT, 2021
** B-PSU-101-LIL-1-1-minishell1-quentin.desmettre
** File description:
** shell_print.c
*/

#include "minishell.h"
#include <glob.h>

void print_tab(char **command, char **env, int wrd_per_line, int biggest_wrd)
{
    int count = 0;

    dprintf(1, "\n\r");
    for (int i = 0; command[i]; i++, count++) {
        if (count >= wrd_per_line) {
            my_putstr("\n\r");
            count = 0;
        }
        dprintf(1, "%-*s\t", biggest_wrd, command[i]);
    }
    my_putstr("\n\r");
    print_input(env);
    dprintf(1, "\33[s\33[u");
}

void set_print_tab(char **command, char **env, input_t *input)
{
    struct winsize w;
    int biggest_wrd = 0;
    int tmp;

    ioctl(0, TIOCGWINSZ, &w);
    for (int i = 0; command[i]; i++) {
        tmp = strlen(command[i]);
        if (tmp > biggest_wrd)
            biggest_wrd = tmp;
    }
    tmp = w.ws_col / (biggest_wrd + 8 - biggest_wrd % 8);
    print_tab(command, env, tmp, biggest_wrd);
    clear_term(input, w, env);
}

void replace_buffer(input_t *input, char **command, char **env)
{
    struct winsize w;

    ioctl(0, TIOCGWINSZ, &w);
    for (size_t i = input->buf_size; i < strlen(command[0]); i++)
        put_in_buffer(command[0][i], input);
    clear_term(input, w, env);
}

int is_in_arr(char **arr, char *str)
{
    for (int i = 0; arr[i]; i++) {
        if (strcmp(str, arr[i]) == 0)
            return 1;
    }
    return 0;
}

char **one_word(char **first)
{
    int size = my_str_array_len(first);
    char **result = malloc(sizeof(char *) * (size + 1));
    int j = 0;

    result[0] = NULL;
    for (int i = 0; i < size; i++) {
        if (!is_in_arr(result, first[i])) {
            result[j] = first[i];
            j++;
            result[j] = NULL;
        }
    }
    return result++;
}

int globing_all_file(char **env, input_t *input)
{
    glob_t glob_buf;
    char *tmp = malloc(sizeof(char) * (input->buf_size + 2));
    char **path = my_str_to_word_array(get_field(env, "PATH="), ":");
    char *wd = malloc(sizeof(char) * 4096);
    char **commands;
    int last_wrd;

    for (int i = 0; i < input->buf_size + 2; i++)
        tmp[i] = '\0';
    for (last_wrd = input->buf_size; last_wrd > 0; last_wrd--)
        if (input->buffer[last_wrd] == ' ') {
            last_wrd += 1;
            break;
        }
    strcat(strncpy(tmp, input->buffer + last_wrd, input->buf_size - last_wrd),
    "*");
    glob_buf.gl_offs = 0;
    glob(tmp, GLOB_DOOFFS, NULL, &glob_buf);
    getcwd(wd, 4096);
    if (input->buf_size - last_wrd > 0) {
        for (int i = 0; path[i]; i++) {
            chdir(path[i]);
            glob(tmp, GLOB_DOOFFS | GLOB_APPEND, NULL, &glob_buf);
        }
    }
    commands = one_word(glob_buf.gl_pathv);
    chdir(wd);
    if (my_str_array_len(commands) > 1)
        set_print_tab(commands, env, input);
    else if (my_str_array_len(commands) == 1)
        replace_buffer(input, commands, env);
    my_free("Ppp", path, wd, commands);
}
