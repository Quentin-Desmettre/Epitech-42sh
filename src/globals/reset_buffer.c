/*
** EPITECH PROJECT, 2021
** B-PSU-101-LIL-1-1-minishell1-quentin.desmettre
** File description:
** reset_buffer.c
*/

int reset_buffer(int new_val, int v)
{
    static int buf = 0;

    if (new_val)
        buf = v;
    return buf;
}

void set_reset_buffer(int new_val)
{
    reset_buffer(1, new_val);
}

int is_reset_buf(void)
{
    return reset_buffer(0, 0);
}
