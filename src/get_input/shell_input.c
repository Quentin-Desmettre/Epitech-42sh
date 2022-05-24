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
    static struct termios raw;
    input_t input = {0, BUFFER_SIZE, 0, 0, 0};

    raw.c_cc[VMIN] = 1;
    tcsetattr(0, TCSANOW, &raw);
    input.buffer = calloc(1, sizeof(char) * BUFFER_SIZE);
    for (int c, send = 1; ;) {
        print_buffer(&input, env);
        if (c == 9)
            globing_all_file(env, &input);
        c = get_char_wait_for_keypress(&input, &send);
        if (c == EOF || c == 3 || (c == 4 && input.buf_size == 0))
            return special_input(&input, c, stop);
        if (c == '\r' || c == '\n')
            break;
        if (send && c != 4)
           put_in_buffer(c, &input);
    }
    isatty(0) ? write(1, "\n\r", 2) : ((*stop) = 1);
    isatty(0) ? tcsetattr(0, TCSANOW, original_termios(NULL)) : 0;
    !input.buf_size ? write(1, "\33[s\33[u", 6) : 0;
    return input.buffer;
}

char *get_shell_input(env_t *vars, int *stop)
{
    char *str = my_strdup("");

    if (!isatty(0))
        return get_next_line(str);
    write(1, "\33[s", 3);
    while (str[0] == 0) {
        free(str);
        if (isatty(0))
            print_input(vars->env);
        str = get_command(stop, vars->env);
        if (!str) {
            my_free("PPppp", vars->vars, vars->env, vars->aliases,
            vars->history, vars);
            print("%s", isatty(get_stdin()) ? "exit\n" : "");
            exit(0);
        }
    }
    return str;
}
