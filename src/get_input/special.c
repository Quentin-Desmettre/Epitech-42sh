/*
** EPITECH PROJECT, 2022
** B-PSU-210-LIL-2-1-42sh-quentin.desmettre
** File description:
** special
*/

#include "minishell.h"

void clear_window(void)
{
    write(1, "\033[H\033[2J", 7);
    write(1, "\33[9999A\33[s", 10);
}

void suppr_all_left(input_t *input)
{
    while (input->key_pos > 0) {
        for (int i = input->key_pos - 1; input->buffer[i]; i++)
            input->buffer[i] = input->buffer[i + 1];
        input->buf_size--;
        input->key_pos--;
    }
}

void special_char(input_t *input, char c, char **env)
{
    if (c == 1)
        input->key_pos = 0;
    if (c == 4) {
        if (input->key_pos != input->buf_size)
            suppr_char(input);
        else {
            write(1, "\n\r", 2);
            print_input(env);
            write(1, input->buffer, input->buf_size);
            write(1, "\33[s", 3);
        }
    }
    if (c == 5)
        input->key_pos = input->buf_size;
    if (c == 12)
        clear_window();
    if (c == 21)
        suppr_all_left(input);
}

char *special_input(input_t *input, char c, int *stop)
{
    if (c == EOF) {
        (*stop) = 1;
        tcsetattr(0, TCSANOW, original_termios(NULL));
        return input->buffer;
    }
    if (c == 3) {
        set_last_exit(1);
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
