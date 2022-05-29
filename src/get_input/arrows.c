/*
** EPITECH PROJECT, 2022
** B-PSU-210-LIL-2-1-42sh-quentin.desmettre
** File description:
** arrows
*/

#include "minishell.h"

void set_input(input_t *input)
{
    input->buf_size = my_strlen(input->buffer);
    if (input->buf_size > 0 && input->buffer[input->buf_size - 1] == '\n') {
        input->buffer[input->buf_size - 1] = 0;
        input->buf_size--;
    }
    input->key_pos = input->buf_size;
    input->buff_limit = input->buf_size > 1 ? input->buf_size - 1 : BUFFER_SIZE;
    input->down = 0;
    input->up = 0;
}

void up_arrow(input_t *input, hist_t **history)
{
    struct winsize w;

    ioctl(0, TIOCGWINSZ, &w);
    if (!input->in_history)
        input->tmp_str = input->buffer;
    input->buffer = get_up(history);
    if (input->buffer == NULL) {
        input->buffer = input->tmp_str;
        input->tmp_str = NULL;
        input->up = 0;
        return;
    }
    input->in_history = 1;
    input->buffer = my_strdup(input->buffer);
    set_input(input);
}

void down_arrow(input_t *input, hist_t **history)
{
    struct winsize w;

    ioctl(0, TIOCGWINSZ, &w);
    if (!input->in_history || !history) {
        input->down = 0;
        return;
    }
    input->buffer = get_down(history);
    if (input->buffer == NULL) {
        input->buffer = input->tmp_str;
        input->in_history = 0;
        input->tmp_str = NULL;
        set_input(input);
        return;
    }
    input->buffer = my_strdup(input->buffer);
    set_input(input);
}
