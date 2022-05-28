/*
** EPITECH PROJECT, 2021
** B-PSU-101-LIL-1-1-minishell1-quentin.desmettre
** File description:
** shell_print.c
*/

#include "minishell.h"
#include <glob.h>

void print_tab(char **command,
char const *prompt, int wrd_per_line, int biggest_wrd)
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
    my_putstr(prompt);
    dprintf(1, "\33[s\33[u");
}

void set_print_tab(char **command, input_t *input, char const *prompt)
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
    print_tab(command, prompt, tmp, biggest_wrd);
    print_buffer(input, prompt);
}

void replace_buffer(input_t *input, char **command, char const *prompt,
hist_t **history)
{
    struct winsize w;

    ioctl(0, TIOCGWINSZ, &w);
    for (size_t i = input->key_pos; i < strlen(command[0]) &&
    command[0][i] != '*'; i++)
        put_in_buffer(command[0][i], input, prompt, history);
    put_in_buffer(' ', input, prompt, history);
    print_buffer(input, prompt);
}

int is_in_arr(char **arr, char *str)
{
    for (int i = 0; arr[i]; i++) {
        if (strcmp(str, arr[i]) == 0)
            return 1;
    }
    return 0;
}
