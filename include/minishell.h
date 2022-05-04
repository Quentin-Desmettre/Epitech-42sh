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

typedef struct {
    char **args;
    char *input_file;
    char *output_file;
    int input_fd;
    int output_fd;
    char redir_type;
} command_t;

void cd_pipe(char **args, char ***env, int o_fd, int is_pipe);
int print_flags(int *flags, char const *home, int len_home, int o_fd);
char *get_current_dir(void);
int get_cd_flags(char *str, int *flags);
char *get_dir(char const *dir, char **env, char const *home);
int exec_builtin_fd(char **args, char ***env, int fds[2], int is_pipe);
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
void exec_commands(list_t *commands, char ***env, list_t **all_commands);
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
char *get_next_word(char const *cmd, int *start, char **err_mess);
int can_get_next_word(char const *cmd, int start, char **err_mess);
int get_heredoc(char const *stop);
int free_exit(list_t **commands, command_t *tmp, char *cmd, int status);
char *get_error_message(char *cmd, char const *command, int i, command_t *tmp);
list_t *get_command_list(char const *command, char **err_mess);
list_t *split_semicolons(char const *input);
char const *get_field(char **env, char const *field);
void print_input(char **env);
void redirect_sigint(int id);
char *get_shell_input(char **env, int *stop);

#endif
