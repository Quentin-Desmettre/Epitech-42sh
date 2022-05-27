/*
** EPITECH PROJECT, 2021
** B-PSU-101-LIL-1-1-minishell1-quentin.desmettre
** File description:
** shell_print.c
*/

#include "minishell.h"
#include <glob.h>
#include <sys/types.h>
#include <sys/stat.h>

void replace_buffer(input_t *input, char **command, char const *prompt)
{
    struct winsize w;

    ioctl(0, TIOCGWINSZ, &w);
    for (size_t i = input->key_pos; i < strlen(command[0]); i++)
        put_in_buffer(command[0][i], input, prompt);
    put_in_buffer(' ', input, prompt);
    print_buffer(input, prompt);
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

void remove_string_arr(char **arr, int nb)
{
    free(arr[nb]);
    for (int i = nb; arr[i]; i++) {
        arr[i] = arr[i + 1];
    }
}

int curent_file_tab(char *tmp, glob_t *glob_buf)
{
    struct stat sb;
    int start;

    glob_buf->gl_offs = 0;
    glob(tmp, GLOB_DOOFFS, NULL, glob_buf);
    for (int i = 0; glob_buf->gl_pathv[i]; i++) {
        start = stat(glob_buf->gl_pathv[i], &sb);
        if (!start && S_ISDIR(sb.st_mode)) {
            glob_buf->gl_pathv[i] = strcat(realloc(glob_buf->gl_pathv[i],
            strlen(glob_buf->gl_pathv[i]) + 2), "/");
            continue;
        }
        if (!access(glob_buf->gl_pathv[i], X_OK)) {
            glob_buf->gl_pathv[i] = strcat(realloc(glob_buf->gl_pathv[i],
            strlen(glob_buf->gl_pathv[i]) + 2), "*");
        }
    }
    return glob_buf->gl_pathc;
}

char **do_glob(char **env, int wrd, char *tmp, input_t *input)
{
    glob_t glob_buf;
    struct stat sb;
    char **path = my_str_to_word_array(get_field(env, "PATH="), ":");
    char **commands;
    int start = curent_file_tab(tmp, &glob_buf);
    if (strlen(tmp) != 1)
        for (int i = 0; path[i]; i++) {
            chdir(path[i]);
            glob(tmp, GLOB_DOOFFS | GLOB_APPEND, NULL, &glob_buf);
        }
    for (int i = start; glob_buf.gl_pathv[i]; i++) {
        start = stat(glob_buf.gl_pathv[i], &sb);
        if (!start && S_ISDIR(sb.st_mode)) {
            remove_string_arr(glob_buf.gl_pathv, i--);
            glob_buf.gl_pathc--;
        }
    }
    commands = one_word(glob_buf.gl_pathv);
    my_free("P", path);
    return commands;
}

void globing_all_file(char **env, input_t *input, char const *prompt)
{
    char *tmp = malloc(sizeof(char) * (input->buf_size + 2));
    char *wd = malloc(sizeof(char) * 4096);
    char **commands;
    int wrd;
    for (wrd = input->key_pos; wrd > 0; wrd--)
        if (contain("\t| ;&<>", input->buffer[wrd])) {
            wrd++;
            break;
        }
    strncpy(tmp, input->buffer + wrd, input->buf_size - wrd);
    tmp[input->key_pos - wrd] = '\0';
    strcat(tmp, "*");
    getcwd(wd, 4096);
    commands = do_glob(env, wrd, tmp, input);
    chdir(wd);
    if (my_str_array_len(commands) > 1)
        set_print_tab(commands, input, prompt);
    else if (my_str_array_len(commands) == 1)
        replace_buffer(input, commands, prompt);
    my_free("pp", wd, commands);
}
