/*
** EPITECH PROJECT, 2022
** B-PSU-210-LIL-2-1-42sh-quentin.desmettre
** File description:
** arrows
*/

#include "minishell.h"

void up_arrow(input_t *input, char const *prompt, histo_t *history)
{
    struct winsize w;

    ioctl(0, TIOCGWINSZ, &w);
    input->tmp_str = input->buffer;
    input->buffer = is_up(history);
    if (input->buffer == NULL) {
        input->buffer = input->tmp_str;
        input->tmp_str = NULL;
        return;
    }
    if (input->buffer[my_strlen(input->buffer) - 1] == '\n') {
        input->buffer[my_strlen(input->buffer) - 1] = 0;
    }
    input->key_pos = input->buf_size;
    input->buff_limit = input->buf_size;
    input->up = 0;
}

void down_arrow(input_t *input, char const *prompt, histo_t *history)
{
    struct winsize w;

    ioctl(0, TIOCGWINSZ, &w);
    input->tmp_str = input->buffer;
    input->buffer = is_down(history);
    if (input->buffer == NULL) {
        input->buffer = input->tmp_str;
        input->tmp_str = NULL;
        return;
    }
    if (input->buffer[my_strlen(input->buffer) - 1] == '\n') {
        input->buffer[my_strlen(input->buffer) - 1] = 0;
    }
    input->key_pos = input->buf_size;
    input->buff_limit = input->buf_size;
    input->up = 0;
}
