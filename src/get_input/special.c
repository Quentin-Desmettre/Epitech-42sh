/*
** EPITECH PROJECT, 2022
** B-PSU-210-LIL-2-1-42sh-quentin.desmettre
** File description:
** special
*/

#include "minishell.h"

char *get_next_line(char *base)
{
    size_t s;

    free(base);
    base = NULL;
    if (getline(&base, &s, stdin) < 0) {
        free(base);
        return NULL;
    }
    if (index_of('\n', base) >= 0)
        base[index_of('\n', base)] = 0;
    return base;
}

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

void special_char(input_t *input, char c, char const *prompt, hist_t **hist)
{
    if (c == 1)
        input->key_pos = 0;
    if (c == 4) {
        if (input->key_pos != input->buf_size)
            suppr_char(input);
        else {
            write(1, "\n\r", 2);
            write(1, prompt, strlen(prompt));
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
    if (input->up || input->down)
        input->up ? up_arrow(input, hist) : down_arrow(input, hist);
}

char *special_input(input_t *input, char c, int *stop)
{
    tcsetattr(0, TCSANOW, original_termios(NULL));
    if (c == EOF) {
        stop ? (*stop) = 1 : 0;
        return input->buffer;
    }
    if (c == CTRL_C) {
        set_reset_buffer(1);
        set_last_exit(1);
        write(1, "\n\r", 2);
        write(1, "\33[s", 3);
        write(1, "\33[u", 3);
        free(input->buffer);
        return my_strdup("");
    }
    if (c == CTRL_D) {
        stop ? (*stop) = 1 : 0;
        return end_command(input);
    }
    return NULL;
}
