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
            buffer->up = (c == 65);
            buffer->down = (c == 66);
            c == 68 && buffer->key_pos > 0 ? buffer->key_pos-- : 0;
            c == 67 && buffer->key_pos < buffer->buf_size ?
            buffer->key_pos++ : 0;
            c == 51 ? c = getchar() : 0;
            c == 126 ? suppr_char(buffer) : 0;
        }
    }
    return c;
}

void put_in_buffer(char c, input_t *buf, char const *prompt, hist_t **history)
{
    if (c < 32 || buf->up || buf->down)
        return special_char(buf, c, prompt, history);
    if (c == 127) {
        if (!buf->buf_size || buf->key_pos == 0)
            return;
        for (int i = buf->key_pos - 1; buf->buffer[i]; i++)
            buf->buffer[i] = buf->buffer[i + 1];
    } else {
        if (buf->buf_size + 1 >= buf->buff_limit) {
            buf->buffer = realloc(buf->buffer,
            buf->buff_limit * 2 + !buf->buff_limit);
            buf->buff_limit = buf->buff_limit * 2 + !buf->buff_limit;
        }
        for (int i = buf->buf_size; i > buf->key_pos; i--)
            buf->buffer[i] = buf->buffer[i - 1];
        buf->buffer[buf->buf_size + 1] = 0;
        buf->buffer[buf->key_pos] = c;
    }
    buf->buf_size += (c == 127 ? -1 : 1);
    buf->key_pos += (c == 127 ? -1 : 1);
}

char *get_command(int *stop, char const *prompt, hist_t **history)
{
    static struct termios raw;
    input_t input = {0, BUFFER_SIZE, 0, 0, 0, 0, 0, 0, 0};
    raw.c_cc[VMIN] = 1;
    tcsetattr(0, TCSANOW, &raw);
    input.buffer = calloc(1, sizeof(char) * BUFFER_SIZE);
    for (int c = 0, send = 1; ;) {
        print_buffer(&input, prompt);
        c == 9 ? globing_all_file(&input, prompt, history) : 0;
        c = get_char_wait_for_keypress(&input, &send);
        if (c == EOF || c == CTRL_C || (c == CTRL_D && input.buf_size == 0))
            return special_input(&input, c, stop);
        if (c == '\r' || c == '\n')
            break;
        if (send || input.up || input.down)
            put_in_buffer(c, &input, prompt, history);
    }
    write(1, "\n\r", 2);
    tcsetattr(0, TCSANOW, original_termios(NULL));
    !input.buf_size ? write(1, "\33[s\33[u", 6) : 0;
    return input.buffer;
}

char *get_shell_input(env_t *vars, int *stop)
{
    char *str = my_strdup("");
    char *prompt = get_prompt(vars->env);

    if (!isatty(0))
        return get_next_line(str);
    write(1, "\33[s", 3);
    while (str[0] == 0) {
        free(str);
        reset_history(&vars->history);
        write(1, prompt, strlen(prompt));
        str = get_command(stop, prompt, &vars->history);
        if (!str) {
            print("exit\n");
            exit(0);
        }
    }
    free(prompt);
    return str;
}
