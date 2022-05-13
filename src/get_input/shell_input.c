/*
** EPITECH PROJECT, 2021
** B-PSU-101-LIL-1-1-minishell1-quentin.desmettre
** File description:
** shell_print.c
*/

#include "minishell.h"

void suppr_char(input_t *buf)
{
    if (buf->key_pos == buf->buf_size)
        return;
    for (int i = buf->key_pos; i < buf->buf_size; i++)
        buf->buffer[i] = buf->buffer[i + 1];
    buf->buf_size--;
    if (buf->key_pos > buf->buf_size)
        buf->key_pos--;
}

char get_char_wait_for_keypress(input_t *buffer, int *send)
{
    char c = getchar();

    *send = c != 9 && c != 27;
    if (c == 27) {
        c = getchar();
        if (c == 91) {
            c = getchar();
            c == 68 && buffer->key_pos > 0 ? buffer->key_pos-- : 0;
            c == 67 && buffer->key_pos < buffer->buf_size ?
            buffer->key_pos++ : 0;
            c == 51 ? c = getchar() : 0;
            c == 126 ? suppr_char(buffer) : 0;
        }
    }
    return c;
}

void put_in_buffer(char c, input_t *buf)
{
    if (c == 127) {
        if (!buf->buf_size || buf->key_pos == 0)
            return;
        for (int i = buf->key_pos - 1; buf->buffer[i]; i++)
            buf->buffer[i] = buf->buffer[i + 1];
        buf->buf_size--;
        buf->key_pos--;
    } else {
        if (buf->buf_size >= buf->buff_limit - 1) {
            buf->buffer = realloc(buf->buffer, buf->buf_size * 2);
            buf->buff_limit = buf->buf_size * 2;
        }
        for (int i = buf->buf_size; i > buf->key_pos; i--)
            buf->buffer[i] = buf->buffer[i - 1];
        buf->buffer[buf->buf_size + 1] = 0;
        buf->buffer[buf->key_pos] = c;
        buf->buf_size++;
        buf->key_pos++;
    }
}

static char *get_command(int *stop, char **env)
{
    int send = 1;
    static struct termios raw;
    input_t input = {0, BUFFER_SIZE, 0, 0, 0};

    raw.c_cc[VMIN] = 1;
    tcsetattr(0, TCSANOW, &raw);
    input.buffer = malloc(sizeof(char) * BUFFER_SIZE);
    for (char c; ;) {
        print_buffer(&input, env);
        c = get_char_wait_for_keypress(&input, &send);
        if (c == EOF || c == 4 || c == 3)
            return end_command(&input);
        if (c == '\r' || c == '\n')
            break;
        if (send)
           put_in_buffer(c, &input);
    }
    tcsetattr(0, TCSANOW, original_termios(NULL));
    isatty(0) ? write(1, "\n\r", 2) : 0;
    return input.buffer;
}

char *get_shell_input(char **env, int *stop)
{
    char *str = my_strdup("");

    while (str[0] == 0) {
        free(str);
        if (is_reset_buf()) {
            set_reset_buffer(0);
            open_stdin();
        }
        if (isatty(0))
            print_input(env);
        str = get_command(stop, env);
        if (!str) {
            free_str_array(env, 1);
            print("%s", isatty(get_stdin()) ? "exit\n" : "");
            exit(0);
        }
    }
    return str;
}
