/*
** EPITECH PROJECT, 2021
** B-PSU-101-LIL-1-1-minishell1-quentin.desmettre
** File description:
** shell_print.c
*/

#include "minishell.h"
#include <glob.h>

void replace_buffer(input_t *input, char **command, char **env)
{
    struct winsize w;

    ioctl(0, TIOCGWINSZ, &w);
    for (size_t i = input->buf_size; i < strlen(command[0]); i++)
        put_in_buffer(command[0][i], input);
    clear_term(input, w, env);
}

int is_in_arr(char **arr, char *str)
{
    for (int i = 0; arr[i]; i++) {
        if (strcmp(str, arr[i]) == 0)
            return 1;
    }
    return 0;
}

char **one_word(char **first)
{
    int size = my_str_array_len(first);
    char **result = malloc(sizeof(char *) * (size + 1));
    int j = 0;

    result[0] = NULL;
    for (int i = 0; i < size; i++) {
        if (!is_in_arr(result, first[i])) {
            result[j] = first[i];
            j++;
            result[j] = NULL;
        }
    }
    return result++;
}

char **do_glob(char **env, int wrd, char *tmp, input_t *input)
{
    glob_t glob_buf;
    char **path = my_str_to_word_array(get_field(env, "PATH="), ":");
    char **commands;

    glob_buf.gl_offs = 0;
    glob(tmp, GLOB_DOOFFS, NULL, &glob_buf);
    if (input->buf_size - wrd > 0)
        for (int i = 0; path[i]; i++) {
            chdir(path[i]);
            glob(tmp, GLOB_DOOFFS | GLOB_APPEND, NULL, &glob_buf);
        }
    commands = one_word(glob_buf.gl_pathv);
    my_free("P", path);
    return commands;
}

void globing_all_file(char **env, input_t *input)
{
    char *tmp = malloc(sizeof(char) * (input->buf_size + 2));
    char *wd = malloc(sizeof(char) * 4096);
    char **commands;
    int wrd;

    for (int i = 0; i < input->buf_size + 2; tmp[i] = '\0', i++);
    for (wrd = input->buf_size; wrd > 0; wrd--)
        if (input->buffer[wrd] == ' ') {
            wrd++;
            break;
        }
    strcat(strncpy(tmp, input->buffer + wrd, input->buf_size - wrd), "*");
    getcwd(wd, 4096);
    commands = do_glob(env, wrd, tmp, input);
    chdir(wd);
    if (my_str_array_len(commands) > 1)
        set_print_tab(commands, env, input);
    else if (my_str_array_len(commands) == 1)
        replace_buffer(input, commands, env);
    my_free("pp", wd, commands);
}
