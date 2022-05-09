/*
** EPITECH PROJECT, 2022
** B-PSU-210-LIL-2-1-42sh-quentin.desmettre
** File description:
** termios
*/

#include "minishell.h"

struct termios *original_termios(struct termios *new)
{
    static struct termios termios;

    if (new != NULL)
        termios = *new;
    return &termios;
}

char *end_command(input_t *input)
{
    if (input->buffer)
        free(input->buffer);
    if (isatty(0))
        write(1, "\n\r", 2);
    tcsetattr(0, TCSANOW, original_termios(NULL));
    return NULL;
}

void reset_input_buffer(input_t *buf)
{
    buf->buffer[0] = 0;
    buf->buf_size = 0;
    buf->key_pos = 0;
    buf->line_offset = 0;
}

void print_buffer(input_t *buf, char **env)
{
    char go_left[3] = {27, 91, 68};
    struct winsize w;

    if (!isatty(0))
        return;
    write(1, "\33[2K\r", 5);
    print_input(env);
    for (int i = 0; i < buf->line_offset - get_input_len(env); i++)
        write(1, "\033[A\33[2K\r", 8);
    write(1, buf->buffer, buf->buf_size);
    ioctl(0, TIOCGWINSZ, &w);
    buf->line_offset = (buf->buf_size - 1) / w.ws_col;
    for (int i = 0; i < buf->buf_size - buf->key_pos; i++)
        write(1, go_left, 3);
}

