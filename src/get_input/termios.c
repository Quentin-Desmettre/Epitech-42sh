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

void clear_term(input_t *buf, struct winsize w, char const *prompt)
{
    int tot_size = buf->buf_size - 1 + strlen(prompt);

    write(1, "\33[u", 3);
    write(1, "\33[2K\r", 5);
    for (int i = 0; i < buf->line_offset; i++)
        write(1, "\033[A\33[2K\r", 8);
    write(1, prompt, strlen(prompt));
    write(1, buf->buffer, buf->buf_size);
    write(1, "\33[s", 3);
    buf->line_offset = tot_size / w.ws_col;
}

void print_buffer(input_t *buf, char const *prompt)
{
    int tot_size = buf->buf_size + 1 + strlen(prompt);
    int index = buf->key_pos + 2 + strlen(prompt);
    int cursor_l = 0;
    struct winsize w;

    ioctl(0, TIOCGWINSZ, &w);
    clear_term(buf, w, prompt);
    cursor_l = (buf->buf_size - buf->key_pos) % w.ws_col;
    for (int i = 0; i < buf->line_offset - (index / w.ws_col); i++)
        write(1, "\033[A\r", 3);
    if (cursor_l > tot_size % w.ws_col + 1) {
        for (int i = 0; i < w.ws_col - cursor_l; i++)
            write(1, "\033[C\r", 3);
    } else {
        for (int i = 0; i < cursor_l; i++)
            write(1, "\033[D\r", 3);
    }
}
