/*
** EPITECH PROJECT, 2021
** B-PSU-101-LIL-1-1-minishell1-quentin.desmettre
** File description:
** shell_print.c
*/

#include "minishell.h"

static void update_input(char buf, char **str, int *len, int c)
{
    if (!c)
        return;
    if (*str)
        append_char(str, buf, 1);
    else
        *str = char_to_str(buf);
    *len += 1;
}

static char *get_command(int *stop)
{
    int c = -1;
    char t;
    char *str = NULL;
    int len = 0;

    while (!str || !len || str[len - 1] != '\n') {
        c = read(get_stdin(), &t, 1);
        if ((c <= 0 && !str) || c < 0 || is_reset_buf()) {
            free(str);
            return is_reset_buf() ? my_strdup("") : NULL;
        }
        if (!c && !isatty(0)) {
            len++;
            *stop = 1;
            break;
        }
        update_input(t, &str, &len, c);
    }
    str[len - 1] = 0;
    return str;
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
        str = get_command(stop);
        if (!str) {
            free_str_array(env, 1);
            print("%s", isatty(get_stdin()) ? "exit\n" : "");
            exit(get_last_exit());
        }
    }
    return str;
}
