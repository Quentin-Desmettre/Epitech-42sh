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
