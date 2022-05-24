/*
** EPITECH PROJECT, 2021
** B-PSU-101-LIL-1-1-minishell1-quentin.desmettre
** File description:
** minishell.h
*/

#ifndef MINISHELL_H
    #define MINISHELL_H
    #include "libmy.h"
    #include <string.h>
    #include <sys/types.h>
    #include <sys/wait.h>
    #include <signal.h>
    #include <unistd.h>
    #include <fcntl.h>
    #include <errno.h>
    #include <sys/ioctl.h>
    #include <stdio.h>
    #include <termios.h>
    #include <stdlib.h>
    #include <string.h>

    #define BUFFER_SIZE 10

    #define NULL_CMD "Invalid null command."
    #define AMBIG_OUT "Ambiguous output redirect."
    #define AMBIG_IN "Ambiguous input redirect."

    #define REDIR_NONE 0
    #define REDIR_OUT 1
    #define REDIR_OUT_APP 2
    #define REDIR_IN 4
    #define REDIR_IN2 8

    #define IS_REDIR_OUT(x) ((x) & 1)
    #define IS_REDIR_OUT_APP(x) ((x) & 2)
    #define IS_REDIR_IN(x) ((x) & 4)
    #define IS_REDIR_IN2(x) ((x) & 8)
    #define HAS_REDIR(x) ((x))

    #define COMMAND(x) ((command_t *)(x))
    #define CRASH_STR(status) (WCOREDUMP(status) ? " (core dumped)" : "")

int is_exit_glob(int change, int new_val);

    #define is_exit() (is_exit_glob(0, 0))
    #define set_is_exit(x) (is_exit_glob(1, x))

    #define ERROR 0
    #undef CONTINUE
    #define CONTINUE 1
    #define NOTHING 2

    #define MAJOR_ERROR 4

    #define HAS_REDIR_OUT(cmd) ((cmd)->redir_type & 1 || (cmd)->redir_type & 2)
    #define HAS_REDIR_IN(cmd) ((cmd)->redir_type & 4 || (cmd)->redir_type & 8)

    #define IS_DOUBLE(c, i, t) ((c)[i] == (t) && (c)[i + 1] == (t))
    #define IS_DB_REDIR(s, i) (IS_DOUBLE(s, i, '>') || IS_DOUBLE(s, i, '<'))

    #define AND_TYPE 0
    #define OR_TYPE 1
    #define NO_TYPE 2

    #define NOTHING_DONE 1
    #define CONNECTION_DONE 2

typedef struct {
    int link_type;
    list_t *commands;
} command_link_t;

typedef struct {
    char **args;
    char *input_file;
    char *output_file;
    int input_fd;
    int output_fd;
    char redir_type;
} command_t;

typedef struct history
{
    char *command;
    int select;
    struct history *next;
}histo_t;

typedef struct {
    char *buffer;
    int buff_limit;
    int buf_size;
    int key_pos;
    int line_offset;
} input_t;

typedef struct {
    histo_t *history;
    char *name;
    char *value;
} replace_t;

typedef struct {
    list_t *commands;
} aliases_t;

typedef struct {
    histo_t *history;
    aliases_t *aliases;
    char **vars;
    char **env;
} env_t;

typedef struct {
    char *line;
    int length;
} backtick_t;

env_t *init_vars(char **env);
void cd_pipe(char **args, char ***env, int o_fd, int is_pipe);
int print_flags(int *flags, char const *home, int len_home, int o_fd);
char *get_current_dir(void);
int get_cd_flags(char *str, int *flags);
char *get_dir(char const *dir, char **env, char const *home);
int exec_builtin_fd(char **args, env_t *vars, int fds[2], int is_pipe);
int is_builtin(char const *word);
void env_pipe(char **args, char ***e, int o_fd);
void set_exit_status(int val);
int get_exit_status(void);
void exit_pipe(char **args, int is_pipe);
void setenv_pipe(char **args, char ***env, int o_fd, int is_pipe);
int index_of_key(char **env, char const *key);
void unsetenv_pipe(char **args, char ***e, int o_fd, int is_pipe);
command_t *create_command(void);
void free_command(void *commnd);
void exec_commands(list_t *commands, env_t *vars, list_t **all_commands);
char const *find_command(char const *str, char **env);
void exec_args(char const *prog, char **args, char **env);
void close_pipe(int fds[2]);
int redirect_pipe(int fds[2]);
void kill_childs(int *pids, int size);
int get_output_fd(command_t *cur, int fds[2], int *is_pipe, list_t *commands);
int get_input_fd(command_t *cur, int fds[2], int *pids);
int exec_command(command_t *cur, void *params[6], int is_pipe, int i);
int is_exit_glob(int change, int new_val);
int last_exit(int new_status, int val);
void set_last_exit(int status);
int get_last_exit(void);
int reset_buffer(int new_val, int v);
void set_reset_buffer(int new_val);
int is_reset_buf(void);
int std_in(int new_val, int new);
void open_stdin(void);
int get_stdin(void);
void close_stdin(void);
void check_exit_status_fd(int status, int *has_crashed);
int main(int ac, char **av, char **env);
int check_redirections(void *strings[3],
int *i, list_t **list, command_t **tmp);
int get_heredoc(char const *stop);
int free_exit(list_t **commands, command_t *tmp, char *cmd, int status);
char *get_error_message(char *cmd, char const *command, int i, command_t *tmp);
list_t *get_command_list(char const *command, char **err_mess);
list_t *split_semicolons(char const *input);
char const *get_field(char **env, char const *field);
void print_input(char **env);
char *get_shell_input(env_t *vars, int *stop);
struct termios *original_termios(struct termios *new);
int get_input_len(char **env);
char *end_command(input_t *input);
void print_buffer(input_t *buf, char **env);
void reset_input_buffer(input_t *buf);
void free_vars(env_t *vars);
int get_final_fd(void);
void set_final_fd(int fd);
void new_parse_input(char *input, env_t *vars);
int parse_for_backticks(char **input, env_t *vars);
char *special_input(input_t *input, char c, int *stop);
void alias(char **args, env_t *e, int o_fd, int is_pipe);
void unalias(char **args, env_t *e, int o_fd, int is_pipe);
void rm_alias(void *alias);

void setvar_pipe(char **args, char ***var, int o_fd, int is_pipe);
void unsetvar_pipe(char **args, char ***e, int o_fd, int is_pipe);
int index_of_key(char **env, char const *key);
int var_is_key(char const *test, char const *field);
int var_index_of_key(char **env, char const *key);
void var_setter(char ***var, int index, char **args);
int set(char **args, char ***var, int o_fd, int is_pipe);
int place_in_local(char **new, char ***array, int i, char *val);
void append_var_array(char ***array, char *val);
void set_var(char ***var, int index, char const *key, char const *val);
void create_var(char ***var, char const *key, char const *val);
int var_args_valid(char **args);

//parse_input
int contain_separator(char str, const char *specifier);
char *clear_str(char *string);
char **str_to_word_array(char const *str, char *delimiters);
char *add_separator(char *separator, char *input);
char *get_next_line(char *base);

int globing_all_file(char **env, input_t *input);
void clear_term(input_t *buf, struct winsize w, char **env);
void put_in_buffer(char c, input_t *buf);
void print_tab(char **command, char **env, int wrd_per_line, int biggest_wrd);
void set_print_tab(char **command, char **env, input_t *input);

#endif
