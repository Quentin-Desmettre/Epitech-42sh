/*
** EPITECH PROJECT, 2021
** B-PSU-101-LIL-1-1-minishell1-quentin.desmettre
** File description:
** shell_print.c
*/

#include "minishell.h"
#include <glob.h>

void print_tab(char **command,
char const *prompt, int wrd_per_line, int integ[2])
{
    int count = 0;
    int biggest_wrd = integ[0];
    int size = integ[1];

    dprintf(1, "\n\r");
    for (int i = 0; command[i]; i++, count++) {
        if (count >= wrd_per_line) {
            my_putstr("\n\r");
            count = 0;
        }
        dprintf(1, "%-*s\t", biggest_wrd, command[i] + size);
    }
    dprintf(1, "\n\r%s\33[s\33[u", prompt);
}

void set_print_tab(char **command, input_t *input, char const *prompt,
char *buffer)
{
    struct winsize w;
    int biggest_wrd = 0;
    int tmp;
    int size;

    for (size = strlen(buffer); buffer[size] != '/' && size > 0; size--);
    if (size != 0 || buffer[0] == '/')
        size++;
    ioctl(0, TIOCGWINSZ, &w);
    for (int i = 0; command[i]; i++) {
        tmp = strlen(command[i]);
        if (tmp > biggest_wrd)
            biggest_wrd = tmp;
    }
    tmp = w.ws_col / (biggest_wrd + 8 - biggest_wrd % 8);
    print_tab(command, prompt, tmp, (int [2]){biggest_wrd, size});
    print_buffer(input, prompt);
}

void replace_buffer(input_t *input, char *str[2], int space,
hist_t **history)
{
    char *command = str[0];
    char *prompt = str[1];
    int wrd;
    int i;

    for (wrd = input->key_pos; wrd > 0; wrd--)
        if (contain("\t| ;&<>", input->buffer[wrd])) {
            wrd++;
            break;
        }
    for (i = input->key_pos; i > wrd; i--)
        put_in_buffer(127, input, prompt, history);
    for (i = 0; i < (int)strlen(command) && strcmp(command + i, "*"); i++)
        put_in_buffer(command[i], input, prompt, history);
    if (i != 0 && command[i - 1] != '/' && space == 1)
        put_in_buffer(' ', input, prompt, history);
    print_buffer(input, prompt);
}

void rest_replace_buffer(input_t *input, char **command, char const *prompt,
hist_t **history)
{
    char *tmp = strdup(command[0]);
    int boolean = 1;

    for (int i = 0; boolean; i++) {
        for (int j = 0; command[j] && boolean; j++) {
            (tmp[i] == command[j][i]) ? (boolean = 1) : (boolean = 0,
            tmp[i] = 0);
        }
    }
    if (strlen(tmp) != 0)
        replace_buffer(input, (char *[2]){tmp, (char *)prompt}, 0, history);
}

int is_in_arr(char **arr, char *str)
{
    for (int i = 0; arr[i]; i++) {
        if (strcmp(str, arr[i]) == 0)
            return 1;
    }
    return 0;
}
