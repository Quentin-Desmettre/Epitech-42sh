/*
** EPITECH PROJECT, 2022
** B-PSU-210-LIL-2-1-42sh-quentin.desmettre
** File description:
** special
*/

#include "minishell.h"

char *special_input(input_t *input, char c, int *stop)
{
    if (c == EOF) {
        (*stop) = 1;
        tcsetattr(0, TCSANOW, original_termios(NULL));
        return input->buffer;
    }
    if (c == 3) {
        write(1, "\n\r", 2);
        write(1, "\33[s", 3);
        write(1, "\33[u", 3);
        free(input->buffer);
        return my_strdup("");
    }
    if (c == 4) {
        (*stop) = 1;
        return end_command(input);
    }
    return NULL;
}
