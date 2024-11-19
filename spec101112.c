#include "baapheader.h"

void handle_sigint2(int sig)
{
    printf("%d", fg_pid);
    if (fg_pid != -1)
    {
        kill(fg_pid, SIGKILL);
        printf("\nForeground process %d interrupted.\n", fg_pid);
        fg_pid = -1;
    }
    else
    {
        printf("\nNo foreground process to interrupt.\n");
    }
}
void handle_sigtstp2(int sig)
{
    if (fg_pid != -1)
    {
        kill(fg_pid, SIGTSTP);
        status_of_process();
        printf("\nForeground process %d stopped and moved to background.\n", fg_pid);
        fg_pid = -1;
    }
    else
    {
        printf("\nNo foreground process to stop.\n");
    }
}

void handle_sigchld2(int sig)
{
    int status;
    pid_t pid;

    while ((pid = waitpid(-1, &status, WNOHANG)) > 0)
    {
        if (WIFEXITED(status))
        {
            // update_process_states();
            //  modify_status(pid,0);
            status_of_process();
            printf("Process %d exited with status %d\n", pid, WEXITSTATUS(status));
        }
        else if (WIFSIGNALED(status))
        {
            // update_process_states();
            status_of_process();
            printf("Process %d was terminated by signal %d\n", pid, WTERMSIG(status));
        }
        else
        {
            //  update_process_states();
            status_of_process();
            printf("Process %d did not exit normally\n", pid);
        }
    }
}
void setup_signal_handlers2()
{
    struct sigaction sa_int, sa_tstp;
    sa_int.sa_handler = handle_sigint2;
    sigemptyset(&sa_int.sa_mask);
    sa_int.sa_flags = SA_RESTART;
    sigaction(SIGINT, &sa_int, NULL); // Ctrl-C

    sa_tstp.sa_handler = handle_sigtstp2;
    sigemptyset(&sa_tstp.sa_mask);
    sa_tstp.sa_flags = SA_RESTART;
    sigaction(SIGTSTP, &sa_tstp, NULL); // Ctrl-Z

    struct sigaction sa;
    sa.sa_handler = &handle_sigchld2;
    sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;
    sigaction(SIGCHLD, &sa, 0);
    status_of_process();
}
void spec10112(char *command)
{

    char *commands[BUFFER_SIZE];
    int command_count = 0;
    char *saveptr;

    char *pipe_token = strtok_r(command, "|", &saveptr);
    while (pipe_token != NULL)
    {
        commands[command_count++] = pipe_token;
        pipe_token = strtok_r(NULL, "|", &saveptr);
    }

    if (command_count == 0)
    {
        fprintf(stderr, "Invalid use of pipe\n");
        return;
    }

    int in_fd = 0;

    for (int i = 0; i < command_count; ++i)
    {
        int pipe_fds[2];
        if (i < command_count - 1)
        {
            if (pipe(pipe_fds) == -1)
            {
                perror("pipe failed");
                return;
            }
        }
        pid_t pid = fork();
        if (pid == 0)
        {
            if (in_fd != 0)
            {
                dup2(in_fd, STDIN_FILENO);
                close(in_fd);
            }
            if (i < command_count - 1)
            {
                close(pipe_fds[0]);
                dup2(pipe_fds[1], STDOUT_FILENO);
                close(pipe_fds[1]);
            }
            char *args[BUFFER_SIZE];
            char *input_file = NULL;
            char *output_file = NULL;
            int append_mode = 0;
            int arg_count = 0;
            char *saveptr_cmd;
            char *result[BUFFER_SIZE];
            int resultct = 0;
            char *token = strtok_r(commands[i], " \t\n", &saveptr_cmd);
            while (token != NULL)
            {
                if (strcmp(token, "<") == 0)
                {
                    token = strtok_r(NULL, " \t\n", &saveptr_cmd);
                    input_file = token;
                }
                else if (strcmp(token, ">") == 0)
                {
                    token = strtok_r(NULL, " \t\n", &saveptr_cmd);
                    output_file = token;
                }
                else if (strcmp(token, ">>") == 0)
                {
                    token = strtok_r(NULL, " \t\n", &saveptr_cmd);
                    output_file = token;
                    append_mode = 1;
                }
                else
                {
                    args[arg_count++] = token;
                }
                token = strtok_r(NULL, " \t\n", &saveptr_cmd);
            }
            args[arg_count] = NULL;
            if (input_file != NULL)
            {
                int fd = open(input_file, O_RDONLY);
                if (fd < 0)
                {
                    perror("No such input file found!");
                    exit(EXIT_FAILURE);
                }
                dup2(fd, STDIN_FILENO);
                close(fd);
            }
            if (output_file != NULL)
            {
                int fd;
                if (append_mode)
                {
                    fd = open(output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
                }
                else
                {
                    fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                }
                if (fd < 0)
                {
                    perror("Error opening output file");
                    exit(EXIT_FAILURE);
                }
                dup2(fd, STDOUT_FILENO);
                close(fd);
            }
            char* stringspec=funcalias(args[0]);
            if(stringspec!=NULL) {
                exec_alias_comm(stringspec);
            }
            else {
            if (strcmp(args[0], "hop") == 0)
            {
                hop(args, arg_count);
                fflush(stdout);
                exit(EXIT_SUCCESS);
            }
            else if (strcmp(args[0], "reveal") == 0)
            {
                char *path11 = ".";
                int show_hidden11 = 0;
                int show_details11 = 0;
                for (int j11 = 1; j11 < arg_count; j11++)
                {
                    if (args[j11][0] == '-')
                    {
                        char *flag11 = args[j11] + 1;
                        while (*flag11)
                        {
                            if (*flag11 == 'a')
                            {
                                show_hidden11 = 1;
                            }
                            else if (*flag11 == 'l')
                            {
                                show_details11 = 1;
                            }
                            flag11++;
                        }
                    }
                    else
                    {
                        path11 = args[j11];
                    }
                }
                reveal(path11, show_hidden11, show_details11);
                fflush(stdout);
                exit(EXIT_SUCCESS);
            }
            else if (strcmp(args[0], "activities") == 0)
            {
                spec13();
                fflush(stdout);
                exit(EXIT_SUCCESS);
            }
            else if (strcmp(args[0], "log") == 0)
            {
                if (arg_count == 2)
                {
                    if (strcmp(args[1], "purge") == 0)
                    {
                        idx = 0;
                        savetofile();
                    }
                }
                else if (arg_count == 1)
                {
                    FILE *file;
                    char line[MAX_STRING_LENGTH];

                    file = fopen(logFilePath, "r");
                    if (file == NULL)
                    {
                        perror("Error opening file");
                        return;
                    }

                    while (fgets(line, sizeof(line), file) != NULL)
                    {
                        printf("%s", line);
                    }

                    fclose(file);
                }
                else if (arg_count == 3)
                {
                    if (strcmp(args[1], "execute") == 0)
                    {
                        int execindex = 0;
                        execindex = atoi(args[2]);
                        execindex = idx - execindex;
                        executelog(commlog[execindex]);
                    }
                }
                fflush(stdout);
                exit(EXIT_SUCCESS);
            }
            // else if (strcmp(args[0], "seek") == 0)
            // {

            //     int only_directories = 0;
            //     int only_files = 0;
            //     int execute_flag = 0;
            //     char *target_name = NULL;
            //     char *search_directory = ".";
            //     int token_index = 1;
            //     while (token_index < arg_count && args[token_index][0] == '-')
            //     {
            //         for (char *flag = args[token_index] + 1; *flag; ++flag)
            //         {
            //             if (*flag == 'd')
            //             {
            //                 only_directories = 1;
            //             }
            //             else if (*flag == 'f')
            //             {
            //                 only_files = 1;
            //             }
            //             else if (*flag == 'e')
            //             {
            //                 execute_flag = 1;
            //             }
            //             else
            //             {
            //                 printf("Invalid flag: -%c\n", *flag);
            //             }
            //         }
            //         token_index++;
            //     }

            //     if (only_directories && only_files)
            //     {
            //         printf("Invalid flags! Cannot combine -d and -f.\n");
            //     }
            //     else
            //     {
            //         if (token_index < arg_count)
            //         {
            //             target_name = args[token_index];
            //             token_index++;
            //         }
            //         else
            //         {
            //             printf("Usage: seek [flags] target [directory]\n");
            //         }
            //         if (token_index < arg_count)
            //         {
            //             search_directory = expand_path(args[token_index]);
            //         }

            //         char results[1000][PATH_MAX];
            //         int match_count = 0;
            //         seek_in_directory(search_directory, target_name, only_directories, only_files, results, &match_count);

            //         if (execute_flag)
            //         {
            //             if (match_count == 1)
            //             {

            //                 struct stat path_stat;
            //                 stat(results[0], &path_stat);

            //                 if (S_ISDIR(path_stat.st_mode))
            //                 {
            //                     if (access(results[0], X_OK) != 0)
            //                     {
            //                         printf("Missing permissions for task!\n");
            //                     }
            //                     else
            //                     {
            //                         chdir(results[0]);
            //                         printf("%s/\n", results[0]);
            //                     }
            //                 }
            //                 else if (S_ISREG(path_stat.st_mode))
            //                 {
            //                     if (access(results[0], R_OK) != 0)
            //                     {
            //                         printf("Missing permissions for task!\n");
            //                     }
            //                     else
            //                     {
            //                         FILE *file = fopen(results[0], "r");
            //                         if (file != NULL)
            //                         {
            //                             char line[PATH_MAX];
            //                             printf("%s\n", results[0]);
            //                             while (fgets(line, sizeof(line), file))
            //                             {
            //                                 printf("%s", line);
            //                             }
            //                             fclose(file);
            //                         }
            //                     }
            //                 }
            //             }
            //             else
            //             {
            //                 for (int i = 0; i < match_count; i++)
            //                 {
            //                     struct stat path_stat;
            //                     stat(results[i], &path_stat);
            //                     if (S_ISDIR(path_stat.st_mode))
            //                     {
            //                         printf("\033[1;34m%s\033[0m\n", results[i]);
            //                     }
            //                     else if (S_ISREG(path_stat.st_mode))
            //                     {
            //                         printf("\033[1;32m%s\033[0m\n", results[i]);
            //                     }
            //                 }
            //             }
            //         }
            //         else
            //         {
            //             if (match_count == 0)
            //             {
            //                 printf("No match found!\n");
            //             }
            //             else
            //             {
            //                 for (int i = 0; i < match_count; i++)
            //                 {
            //                     struct stat path_stat;
            //                     stat(results[i], &path_stat);
            //                     if (S_ISDIR(path_stat.st_mode))
            //                     {
            //                         printf("\033[1;34m%s\033[0m\n", results[i]);
            //                     }
            //                     else if (S_ISREG(path_stat.st_mode))
            //                     {
            //                         printf("\033[1;32m%s\033[0m\n", results[i]);
            //                     }
            //                 }
            //             }
            //         }
            //     }
            // fflush(stdout);
            // exit(EXIT_SUCCESS);
            // }
            else if (strcmp(args[0], "fg") == 0)
            {
                pid_t pid = atoi(args[1]);
                fg_command(pid);
                fflush(stdout);
            }
            else if (strcmp(args[0], "bg") == 0)
            {
                pid_t pid = atoi(args[1]);
                bg_command(pid);
                fflush(stdout);
            }
            else if (strcmp(args[0], "neonate") == 0)
            {
                int interval = atoi(args[2]);
                neonate(interval);
                fflush(stdout);
            }
            else if (strcmp(args[0], "proclore") == 0)
            {
                if (arg_count == 1)
                {
                    char current_pid[32];
                    snprintf(current_pid, sizeof(current_pid), "%d", getpid());
                    proclore(current_pid);
                }
                else if (arg_count == 2)
                {
                    proclore(args[1]);
                }
                else
                    printf("INVALID COMMAND!!\n");
                fflush(stdout);
                exit(EXIT_SUCCESS);
            }
            else if(strcmp(args[0],"iMan")==0) {
                retrieve_and_show_man_page(args[1]);
                fflush(stdout);
            }
            else
            {
                execvp(args[0], args);
                perror("execvp failed");
                fflush(stdout);
            }
        }
        }
        else if (pid > 0)
        {
            wait(NULL);
            if (in_fd != 0)
            {
                close(in_fd);
            }
            if (i < command_count - 1)
            {
                close(pipe_fds[1]);
                in_fd = pipe_fds[0];
            }
        }
        else
        {
            perror("fork failed");
            return;
        }
    }
}
void spec101122(char *command)
{

    char *commands[BUFFER_SIZE];
    int command_count = 0;
    char *saveptr;

    char *pipe_token = strtok_r(command, "|", &saveptr);
    while (pipe_token != NULL)
    {
        commands[command_count++] = pipe_token;
        pipe_token = strtok_r(NULL, "|", &saveptr);
    }

    if (command_count == 0)
    {
        fprintf(stderr, "Invalid use of pipe\n");
        return;
    }

    int in_fd = 0;

    for (int i = 0; i < command_count; ++i)
    {
        int pipe_fds[2];
        if (i < command_count - 1)
        {
            if (pipe(pipe_fds) == -1)
            {
                perror("pipe failed");
                return;
            }
        }
        pid_t pid = fork();
        if (pid == 0)
        {
            if (in_fd != 0)
            {
                dup2(in_fd, STDIN_FILENO);
                close(in_fd);
            }
            if (i < command_count - 1)
            {
                close(pipe_fds[0]);
                dup2(pipe_fds[1], STDOUT_FILENO);
                close(pipe_fds[1]);
            }
            char *args[BUFFER_SIZE];
            char *input_file = NULL;
            char *output_file = NULL;
            int append_mode = 0;
            int arg_count = 0;
            char *saveptr_cmd;
            char *result[BUFFER_SIZE];
            int resultct = 0;
            char *token = strtok_r(commands[i], " \t\n", &saveptr_cmd);
            while (token != NULL)
            {
                if (strcmp(token, "<") == 0)
                {
                    token = strtok_r(NULL, " \t\n", &saveptr_cmd);
                    input_file = token;
                }
                else if (strcmp(token, ">") == 0)
                {
                    token = strtok_r(NULL, " \t\n", &saveptr_cmd);
                    output_file = token;
                }
                else if (strcmp(token, ">>") == 0)
                {
                    token = strtok_r(NULL, " \t\n", &saveptr_cmd);
                    output_file = token;
                    append_mode = 1;
                }
                else
                {
                    args[arg_count++] = token;
                }
                token = strtok_r(NULL, " \t\n", &saveptr_cmd);
            }
            args[arg_count] = NULL;
            if (input_file != NULL)
            {
                int fd = open(input_file, O_RDONLY);
                if (fd < 0)
                {
                    perror("No such input file found!");
                    exit(EXIT_FAILURE);
                }
                dup2(fd, STDIN_FILENO);
                close(fd);
            }
            if (output_file != NULL)
            {
                int fd;
                if (append_mode)
                {
                    fd = open(output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
                }
                else
                {
                    fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                }
                if (fd < 0)
                {
                    perror("Error opening output file");
                    exit(EXIT_FAILURE);
                }
                dup2(fd, STDOUT_FILENO);
                close(fd);
            }
            char* stringspec=funcalias(args[0]);
            if(stringspec!=NULL) {
                exec_alias_comm(stringspec);
            }
            else {
            if (strcmp(args[0], "hop") == 0)
            {
                hop(args, arg_count);
                fflush(stdout);
                exit(EXIT_SUCCESS);
            }
            else if (strcmp(args[0], "reveal") == 0)
            {
                char *path11 = ".";
                int show_hidden11 = 0;
                int show_details11 = 0;
                for (int j11 = 1; j11 < arg_count; j11++)
                {
                    if (args[j11][0] == '-')
                    {
                        char *flag11 = args[j11] + 1;
                        while (*flag11)
                        {
                            if (*flag11 == 'a')
                            {
                                show_hidden11 = 1;
                            }
                            else if (*flag11 == 'l')
                            {
                                show_details11 = 1;
                            }
                            flag11++;
                        }
                    }
                    else
                    {
                        path11 = args[j11];
                    }
                }
                reveal(path11, show_hidden11, show_details11);
                fflush(stdout);
                exit(EXIT_SUCCESS);
            }
            else if (strcmp(args[0], "activities") == 0)
            {
                //spec13();
                fflush(stdout);
                exit(EXIT_SUCCESS);
            }
            // else if (strcmp(args[0], "log") == 0)
            // {
            //     if (arg_count == 2)
            //     {
            //         if (strcmp(args[1], "purge") == 0)
            //         {
            //             idx = 0;
            //             savetofile();
            //         }
            //     }
            //     else if (arg_count == 1)
            //     {
            //         FILE *file;
            //         char line[MAX_STRING_LENGTH];

            //         file = fopen(logFilePath, "r");
            //         if (file == NULL)
            //         {
            //             perror("Error opening file");
            //             return;
            //         }

            //         while (fgets(line, sizeof(line), file) != NULL)
            //         {
            //             printf("%s", line);
            //         }

            //         fclose(file);
            //     }
            //     else if (arg_count == 3)
            //     {
            //         if (strcmp(args[1], "execute") == 0)
            //         {
            //             int execindex = 0;
            //             execindex = atoi(args[2]);
            //             execindex = idx - execindex;
            //             executelog(commlog[execindex]);
            //         }
            //     }
            //     fflush(stdout);
            //     exit(EXIT_SUCCESS);
            // }
            // else if (strcmp(args[0], "seek") == 0)
            // {

            //     int only_directories = 0;
            //     int only_files = 0;
            //     int execute_flag = 0;
            //     char *target_name = NULL;
            //     char *search_directory = ".";
            //     int token_index = 1;
            //     while (token_index < arg_count && args[token_index][0] == '-')
            //     {
            //         for (char *flag = args[token_index] + 1; *flag; ++flag)
            //         {
            //             if (*flag == 'd')
            //             {
            //                 only_directories = 1;
            //             }
            //             else if (*flag == 'f')
            //             {
            //                 only_files = 1;
            //             }
            //             else if (*flag == 'e')
            //             {
            //                 execute_flag = 1;
            //             }
            //             else
            //             {
            //                 printf("Invalid flag: -%c\n", *flag);
            //             }
            //         }
            //         token_index++;
            //     }

            //     if (only_directories && only_files)
            //     {
            //         printf("Invalid flags! Cannot combine -d and -f.\n");
            //     }
            //     else
            //     {
            //         if (token_index < arg_count)
            //         {
            //             target_name = args[token_index];
            //             token_index++;
            //         }
            //         else
            //         {
            //             printf("Usage: seek [flags] target [directory]\n");
            //         }
            //         if (token_index < arg_count)
            //         {
            //             search_directory = expand_path(args[token_index]);
            //         }

            //         char results[1000][PATH_MAX];
            //         int match_count = 0;
            //         seek_in_directory(search_directory, target_name, only_directories, only_files, results, &match_count);

            //         if (execute_flag)
            //         {
            //             if (match_count == 1)
            //             {

            //                 struct stat path_stat;
            //                 stat(results[0], &path_stat);

            //                 if (S_ISDIR(path_stat.st_mode))
            //                 {
            //                     if (access(results[0], X_OK) != 0)
            //                     {
            //                         printf("Missing permissions for task!\n");
            //                     }
            //                     else
            //                     {
            //                         chdir(results[0]);
            //                         printf("%s/\n", results[0]);
            //                     }
            //                 }
            //                 else if (S_ISREG(path_stat.st_mode))
            //                 {
            //                     if (access(results[0], R_OK) != 0)
            //                     {
            //                         printf("Missing permissions for task!\n");
            //                     }
            //                     else
            //                     {
            //                         FILE *file = fopen(results[0], "r");
            //                         if (file != NULL)
            //                         {
            //                             char line[PATH_MAX];
            //                             printf("%s\n", results[0]);
            //                             while (fgets(line, sizeof(line), file))
            //                             {
            //                                 printf("%s", line);
            //                             }
            //                             fclose(file);
            //                         }
            //                     }
            //                 }
            //             }
            //             else
            //             {
            //                 for (int i = 0; i < match_count; i++)
            //                 {
            //                     struct stat path_stat;
            //                     stat(results[i], &path_stat);
            //                     if (S_ISDIR(path_stat.st_mode))
            //                     {
            //                         printf("\033[1;34m%s\033[0m\n", results[i]);
            //                     }
            //                     else if (S_ISREG(path_stat.st_mode))
            //                     {
            //                         printf("\033[1;32m%s\033[0m\n", results[i]);
            //                     }
            //                 }
            //             }
            //         }
            //         else
            //         {
            //             if (match_count == 0)
            //             {
            //                 printf("No match found!\n");
            //             }
            //             else
            //             {
            //                 for (int i = 0; i < match_count; i++)
            //                 {
            //                     struct stat path_stat;
            //                     stat(results[i], &path_stat);
            //                     if (S_ISDIR(path_stat.st_mode))
            //                     {
            //                         printf("\033[1;34m%s\033[0m\n", results[i]);
            //                     }
            //                     else if (S_ISREG(path_stat.st_mode))
            //                     {
            //                         printf("\033[1;32m%s\033[0m\n", results[i]);
            //                     }
            //                 }
            //             }
            //         }
            //     }
            // fflush(stdout);
            // exit(EXIT_SUCCESS);
            // }
            else if (strcmp(args[0], "fg") == 0)
            {
                pid_t pid = atoi(args[1]);
                fg_command(pid);
                fflush(stdout);
            }
            else if (strcmp(args[0], "bg") == 0)
            {
                pid_t pid = atoi(args[1]);
                bg_command(pid);
                fflush(stdout);
            }
            else if (strcmp(args[0], "neonate") == 0)
            {
                int interval = atoi(args[2]);
                neonate(interval);
                fflush(stdout);
            }
            else if (strcmp(args[0], "proclore") == 0)
            {
                if (arg_count == 1)
                {
                    char current_pid[32];
                    snprintf(current_pid, sizeof(current_pid), "%d", getpid());
                    proclore(current_pid);
                    fflush(stdout);
                }
                else if (arg_count == 2)
                {
                    proclore(args[1]);
                    fflush(stdout);
                }
                else {
                    printf("INVALID COMMAND!!\n");
                    fflush(stdout);
                }
            }
            else if(strcmp(args[0],"iMan")==0) {
                retrieve_and_show_man_page(args[1]);
                fflush(stdout);
            }
            else
            {
                execvp(args[0], args);
                perror("execvp failed");
                fflush(stdout);
            }
        }
        }
        else if (pid > 0)
        {
            wait(NULL);
            if (in_fd != 0)
            {
                close(in_fd);
            }
            if (i < command_count - 1)
            {
                close(pipe_fds[1]);
                in_fd = pipe_fds[0];
            }
        }
        else
        {
            perror("fork failed");
            return;
        }
    }
}
void ampercent(char *command)
{
    while (strchr(command, '&') != NULL)
    {
        char *background_command = strchr(command, '&');
        *background_command = '\0';
        char *background_trimmed_command = trim_spaces(command);
        if (background_trimmed_command == NULL || *background_trimmed_command == '\0')
        {
            break;
        }
        pid_t pid = fork();
        if (pid == 0)
        {
            printf("Background process started with PID %d\n", getpid());
            char *args[] = {"/bin/sh", "-c", background_trimmed_command, NULL};
            for (int i = 0; i < sizeof(args); i++)
            {
                if (args[i] == NULL)
                {
                    break;
                }
            }
            spec10112(background_trimmed_command);
            exit(1);
        }
        else if (pid > 0)
        {
            struct sigaction sa;
            sa.sa_handler = &handle_sigchld2;
            sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;
            sigaction(SIGCHLD, &sa, 0);
            printf("[%d] %d\n", pid, pid);
        }
        else
        {
            int status;
            waitpid(pid, &status, WUNTRACED);
            if (WIFEXITED(status))
            {
                printf("PROCESSS EXITED\n");
                //update_process_states();
            }
            perror("fork");
        }

        command = background_command + 1;
        command = trim_spaces(command);
    }
    if (strstr(command, ">") != NULL || strstr(command, "<") != NULL || strstr(command, ">>") != NULL || strstr(command, "|") != NULL)
    {
        spec10112(command);
    }
    else
    {
        char *tokens11[BUFFER_SIZE];
        int tokenct1 = 0;
        char *save2;

        char *token11 = strtok_r(command, " \t\n ", &save2);
        while (token11 != NULL)
        {
            char *c = malloc(sizeof(char) * 40000);
            c = trim_spaces(token11);
            strcpy(token11, c);
            tokens11[tokenct1++] = token11;
            token11 = strtok_r(NULL, " \t\n", &save2);
        }
        if (tokenct1 > 0 && tokens11[0] != NULL)
        {
            char* stringalias=funcalias(tokens11[0]);
            if(stringalias!=NULL) {
                exec_alias_comm(stringalias);
            }
            else {
            if (strcmp(tokens11[0], "hop") == 0)
            {
                hop(tokens11, tokenct1);
            }
            else if (strcmp(tokens11[0], "reveal") == 0)
            {
                char *path11 = ".";
                int show_hidden11 = 0;
                int show_details11 = 0;
                for (int j11 = 1; j11 < tokenct1; j11++)
                {
                    if (tokens11[j11][0] == '-')
                    {
                        char *flag11 = tokens11[j11] + 1;
                        while (*flag11)
                        {
                            if (*flag11 == 'a')
                            {
                                show_hidden11 = 1;
                            }
                            else if (*flag11 == 'l')
                            {
                                show_details11 = 1;
                            }
                            flag11++;
                        }
                    }
                    else
                    {
                        path11 = tokens11[j11];
                    }
                }
                reveal(path11, show_hidden11, show_details11);
            }
            else if (strcmp(tokens11[0], "proclore") == 0)
            {
                if (tokenct1 == 1)
                {
                    char current_pid[32];
                    snprintf(current_pid, sizeof(current_pid), "%d", getpid());
                    proclore(current_pid);
                }
                else if (tokenct1 == 2)
                {
                    proclore(tokens11[1]);
                }
                else
                    printf("INVALID COMMAND!!\n");
            }
            else if (strcmp(tokens11[0], "activities") == 0)
            {
                 spec13();
            }
            else if (strcmp(tokens11[0], "seek") == 0)
            {
                int flagexecution = 0;
                int filesonly = 0;
                int dirsonly = 0;
                char *nametarget = NULL;
                char *search_directory = ".";
                int token_index = 1;
                while (token_index < tokenct1 && tokens11[token_index][0] == '-')
                {
                    for (char *fg = tokens11[token_index] + 1; *fg; ++fg)
                    {
                        if (*fg == 'd')
                        {
                            dirsonly = 1;
                        }
                        else if (*fg == 'f')
                        {
                            filesonly = 1;
                        }
                        else if (*fg == 'e')
                        {
                            flagexecution = 1;
                        }
                        else
                        {
                            printf("Invalid flag: -%c\n", *fg);
                        }
                    }
                    token_index++;
                }

                if (dirsonly == 1 && filesonly == 1)
                {
                    printf("Invalid flags! Cannot combine -d and -f.\n");
                }
                else
                {
                    if (token_index < tokenct1)
                    {
                        nametarget = tokens11[token_index];
                        token_index++;
                    }
                    else
                    {
                        printf("Usage: seek [flags] target [directory]\n");
                    }
                    if (token_index < tokenct1)
                    {
                        search_directory = expand_path(tokens11[token_index]);
                    }

                    int matchings = 0;
                    char results[1000][PATH_MAX];
                    seek_in_directory(search_directory, nametarget, dirsonly, filesonly, results, &matchings);

                    if (flagexecution)
                    {
                        if (matchings == 1)
                        {

                            struct stat statusofpath;
                            stat(results[0], &statusofpath);

                            if (S_ISDIR(statusofpath.st_mode))
                            {
                                if (access(results[0], X_OK) != 0)
                                {
                                    printf("Missing permissions for task!\n");
                                }
                                else
                                {
                                    chdir(results[0]);
                                    printf("%s/\n", results[0]);
                                }
                            }
                            else if (S_ISREG(statusofpath.st_mode))
                            {
                                if (access(results[0], R_OK) != 0)
                                {
                                    printf("Missing permissions for task!\n");
                                }
                                else
                                {
                                    FILE *file = fopen(results[0], "r");
                                    if (file != NULL)
                                    {
                                        char line[PATH_MAX];
                                        printf("%s\n", results[0]);
                                        while (fgets(line, sizeof(line), file))
                                        {
                                            printf("%s", line);
                                        }
                                        fclose(file);
                                    }
                                }
                            }
                        }
                        else
                        {
                            int pointer = 0;
                            while (pointer < matchings)
                            {
                                struct stat statusofpath;
                                stat(results[pointer], &statusofpath);
                                if (S_ISDIR(statusofpath.st_mode))
                                {
                                    printf("\033[1;34m%s\033[0m\n", results[pointer]);
                                }
                                else if (S_ISREG(statusofpath.st_mode))
                                {
                                    printf("\033[1;32m%s\033[0m\n", results[pointer]);
                                }
                                pointer++;
                            }
                        }
                    }
                    else
                    {
                        if (matchings == 0)
                        {
                            printf("No match found!\n");
                        }
                        else
                        {
                            int pointer2 = 0;
                            while (pointer2 < matchings)
                            {
                                struct stat pathstatus2;
                                stat(results[pointer2], &pathstatus2);
                                if (S_ISDIR(pathstatus2.st_mode))
                                {
                                    printf("\033[1;34m%s\033[0m\n", results[pointer2]);
                                }
                                else if (S_ISREG(pathstatus2.st_mode))
                                {
                                    printf("\033[1;32m%s\033[0m\n", results[pointer2]);
                                }
                                pointer2++;
                            }
                        }
                    }
                }
            }

            else if (strcmp(tokens11[0], "log") == 0)
            {
                if (tokenct1 == 2)
                {
                    if (strcmp(tokens11[1], "purge") == 0)
                    {
                        idx = 0;
                        savetofile();
                    }
                }
                else if (tokenct1 == 1)
                {
                    FILE *file;
                    char line[MAX_STRING_LENGTH];

                    file = fopen(logFilePath, "r");
                    if (file == NULL)
                    {
                        perror("Error opening file");
                        return;
                    }

                    while (fgets(line, sizeof(line), file) != NULL)
                    {
                        printf("%s", line);
                    }

                    fclose(file);
                }
                else if (tokenct1 == 3)
                {
                    if (strcmp(tokens11[1], "execute") == 0)
                    {
                        int execindex = 0;
                        execindex = atoi(tokens11[2]);
                        execindex = idx - execindex;
                        char *commlogexec = commlog[execindex];
                        spec101122(commlogexec);
                    }
                }
            }
            else if (strcmp(tokens11[0], "fg") == 0)
            {
                pid_t pid = atoi(tokens11[1]);
                fg_command(pid);
            }
            else if (strcmp(tokens11[0], "bg") == 0)
            {
                pid_t pid = atoi(tokens11[1]);
                bg_command(pid);
            }
            else if (strcmp(tokens11[0], "neonate") == 0)
            {
                int interval = atoi(tokens11[2]);
                neonate(interval);
            }
            else if(strcmp(tokens11[0],"iMan")==0) {
                retrieve_and_show_man_page(tokens11[1]);
            }
            else
            {
                int f = fork();
                if (f == 0)
                {
                    execvp(tokens11[0], tokens11);
                    perror("INVALID COMMAND");
                    exit(1);
                }
                else if (f > 0)
                {
                    fg_pid = f;
                    int status;
                    struct timeval start, end;
                    gettimeofday(&start, NULL);
                    wait(NULL);
                    gettimeofday(&end, NULL);
                    double time_taken = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
                    if (time_taken > 2.00)
                    {
                        timeprocess = (int)time_taken;
                        flag2sec = 1;
                        strcpy(command2sec, command);
                    }
                }
                else
                {
                    perror("fork");
                }
            }
        }
        }
    }
}