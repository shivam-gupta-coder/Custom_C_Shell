#include "baapheader.h"
void savetofile()
{
    FILE *file = fopen(logFilePath, "w");
    if (file)
    {
        for (int i = 0; i < idx; i++)
        {
            fprintf(file, "%s\n", commlog[i]);
        }
        fclose(file);
    }
    else
    {
        perror("Failed to open log file for writing");
    }
}
void storeloginarray(char *command)
{
    if (idx > 0 && strcmp(commlog[idx - 1], command) == 0)
    {
        return;
    }
    if (idx == 15)
    {
        for (int i = 0; i < 14; i++)
        {
            strcpy(commlog[i], commlog[i + 1]);
        }
        strcpy(commlog[14], command);
    }
    else
    {
        strcpy(commlog[idx++], command);
    }
    savetofile();
}

void executelog(char *execcomm)
{
    char ip2[BUFFER_SIZE];
    strcpy(ip2, execcomm);
    char *save12;
    char *command12 = strtok_r(ip2, ";", &save12);
    while (command12 != NULL)
    {
        command12 = trim_spaces(command12);
        if (command12 == NULL || *command12 == '\0')
        {
            command12 = strtok_r(NULL, ";", &save12);
            continue;
        }
        while (strchr(command12, '&') != NULL)
        {
            char *background_command = strchr(command12, '&');
            *background_command = '\0';
            char *background_trimmed_command = trim_spaces(command12);
            if (background_trimmed_command == NULL || *background_trimmed_command == '\0')
            {
                break;
            }

            // strcpy(controlzcommand, background_trimmed_command);
            is_alias = 0;
            char *stringalias = funcalias(background_trimmed_command);
            if (stringalias != NULL)
            {
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
                    spec101122(background_trimmed_command);
                    exit(1);
                }
                else if (pid > 0)
                {
                    push_process(pid, background_trimmed_command, 1);
                    struct sigaction sa;
                    sa.sa_handler = &handle_sigchld2;
                    sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;
                    sigaction(SIGCHLD, &sa, 0);
                    printf("[%d] %d\n", pid, pid);
                    // push_process(pid, background_trimmed_command, "running");
                    //  pid bg_trimmed_command running
                }
                else
                {
                    int status;
                    waitpid(pid, &status, 0);
                    if (WIFEXITED(status))
                    {
                        printf("PROCESSS EXITED\n");
                        // update_process_states();
                    }
                    perror("fork");
                }
            }
            else
            {
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
                    execvp(args[0], args);
                    perror("execvp");
                    exit(1);
                }
                else if (pid > 0)
                {
                    push_process(pid, background_trimmed_command, 1);
                    struct sigaction sa;
                    sa.sa_handler = &handle_sigchld2;
                    sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;
                    sigaction(SIGCHLD, &sa, 0);
                    printf("[%d] %d\n", pid, pid);
                    // push_process(pid, background_trimmed_command, "running");
                    // pid bg_trimmed_command running
                }
                else
                {
                    int status;
                    waitpid(pid, &status, 0);
                    if (WIFEXITED(status))
                    {
                        printf("PROCESSS EXITED\n");
                        // update_process_states();
                    }
                    perror("fork");
                }
            }

            command12 = background_command + 1;
            command12 = trim_spaces(command12);
        }
        if (strstr(command12, ">") != NULL || strstr(command12, "<") != NULL || strstr(command12, ">>") != NULL || strstr(command12, "|") != NULL)
        {
            spec101122(command12);
        }
        else
        {
            while (strchr(command12, '&') != NULL)
            {
                char *bgcomm = strchr(command12, '&');
                *bgcomm = '\0';
                char *bgtrimcomm = trim_spaces(command12);
                if (bgtrimcomm == NULL || *bgtrimcomm == '\0')
                {
                    break;
                }
                pid_t pid12 = fork();
                if (pid12 == 0)
                {
                    printf("Background process started with PID %d\n", getpid());
                    char *args[] = {"/bin/sh", "-c", bgtrimcomm, NULL};
                    for (int i = 0; i < sizeof(args); i++)
                    {
                        if (args[i] == NULL)
                        {
                            break;
                        }
                    }
                    int x = fork();
                    if (x == 0)
                    {
                        execvp(args[0], args);
                    }
                    else if (x > 0)
                    {
                        wait(NULL);
                        printf("Background process %d finished with status %d\n", x, WEXITSTATUS(x));
                    }
                    else
                    {
                        perror("fork");
                    }
                    perror("execvp");
                    exit(1);
                }
                else if (pid12 > 0)
                {
                    printf("[%d] %d\n", pid12, pid12);
                }
                else
                {
                    perror("fork");
                }
                command12 = bgcomm + 1;
                command12 = trim_spaces(command12);
            }
            char *tokens12[BUFFER_SIZE];
            int tokenct2 = 0;
            char *save22;
            char *token12 = strtok_r(command12, " \t\n", &save22);
            while (token12 != NULL)
            {
                char *c = malloc(sizeof(char) * 40000);
                c = trim_spaces(token12);
                strcpy(token12, c);
                tokens12[tokenct2++] = token12;
                token12 = strtok_r(NULL, " \t\n", &save22);
            }
            if (tokenct2 > 0 && tokens12[0] != NULL)
            {
                if (strcmp(tokens12[0], "hop") == 0)
                {
                    hop(tokens12, tokenct2);
                }
                else if (strcmp(tokens12[0], "reveal") == 0)
                {
                    char *path11 = ".";
                    int show_hidden11 = 0;
                    int show_details11 = 0;
                    for (int j11 = 1; j11 < tokenct2; j11++)
                    {
                        if (tokens12[j11][0] == '-')
                        {
                            char *flag11 = tokens12[j11] + 1;
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
                            path11 = tokens12[j11];
                        }
                    }
                    reveal(path11, show_hidden11, show_details11);
                }
                else if (strcmp(tokens12[0], "proclore") == 0)
                {
                    if (tokenct2 == 1)
                    {
                        char current_pid[32];
                        snprintf(current_pid, sizeof(current_pid), "%d", getpid());
                        proclore(current_pid);
                    }
                    else if (tokenct2 == 2)
                    {
                        proclore(tokens12[1]);
                    }
                    else
                        printf("INVALID COMMAND!!\n");
                }
                else if (strcmp(tokens12[0], "seek") == 0)
                {
                    int only_directories = 0;
                    int only_files = 0;
                    int execute_flag = 0;
                    char *target_name = NULL;
                    char *search_directory = ".";
                    int token_index = 1;
                    while (token_index < tokenct2 && tokens12[token_index][0] == '-')
                    {
                        for (char *flag = tokens12[token_index] + 1; *flag; ++flag)
                        {
                            if (*flag == 'd')
                            {
                                only_directories = 1;
                            }
                            else if (*flag == 'f')
                            {
                                only_files = 1;
                            }
                            else if (*flag == 'e')
                            {
                                execute_flag = 1;
                            }
                            else
                            {
                                printf("Invalid flag: -%c\n", *flag);
                            }
                        }
                        token_index++;
                    }

                    if (only_directories && only_files)
                    {
                        printf("Invalid flags! Cannot combine -d and -f.\n");
                    }
                    else
                    {
                        if (token_index < tokenct2)
                        {
                            target_name = tokens12[token_index];
                            token_index++;
                        }
                        else
                        {
                            printf("Usage: seek [flags] target [directory]\n");
                        }
                        if (token_index < tokenct2)
                        {
                            search_directory = expand_path(tokens12[token_index]);
                        }

                        char results[1000][PATH_MAX];
                        int match_count = 0;
                        seek_in_directory(search_directory, target_name, only_directories, only_files, results, &match_count);

                        if (execute_flag)
                        {
                            if (match_count == 1)
                            {

                                struct stat path_stat;
                                stat(results[0], &path_stat);

                                if (S_ISDIR(path_stat.st_mode))
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
                                else if (S_ISREG(path_stat.st_mode))
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
                                for (int i = 0; i < match_count; i++)
                                {
                                    struct stat path_stat;
                                    stat(results[i], &path_stat);
                                    if (S_ISDIR(path_stat.st_mode))
                                    {
                                        printf("\033[1;34m%s\033[0m\n", results[i]);
                                    }
                                    else if (S_ISREG(path_stat.st_mode))
                                    {
                                        printf("\033[1;32m%s\033[0m\n", results[i]);
                                    }
                                }
                            }
                        }
                        else
                        {
                            if (match_count == 0)
                            {
                                printf("No match found!\n");
                            }
                            else
                            {
                                for (int i = 0; i < match_count; i++)
                                {
                                    struct stat path_stat;
                                    stat(results[i], &path_stat);
                                    if (S_ISDIR(path_stat.st_mode))
                                    {
                                        printf("\033[1;34m%s\033[0m\n", results[i]);
                                    }
                                    else if (S_ISREG(path_stat.st_mode))
                                    {
                                        printf("\033[1;32m%s\033[0m\n", results[i]);
                                    }
                                }
                            }
                        }
                    }
                }
                else if (strcmp(tokens12[0], "neonate") == 0)
                {
                    int interval = atoi(tokens12[2]);
                    neonate(interval);
                }
                else if (strcmp(tokens12[0], "fg") == 0)
                {
                    int procid = atoi(tokens12[1]);
                    fg_command(procid);
                    // update_process_states();
                }
                else if (strcmp(tokens12[0], "bg") == 0)
                {
                    int procid = atoi(tokens12[1]);
                    bg_command(procid);
                    // update_process_states();
                }
                else if (strcmp(tokens12[0], "ping") == 0)
                {
                    int procid = atoi(tokens12[1]);
                    int signo = atoi(tokens12[2]);
                    ping_command(procid, signo);
                }
                else if(strcmp(tokens12[0],"iMan")==0) {
                    retrieve_and_show_man_page(tokens12[1]);
                }
                else
                {
                    int f2 = fork();
                    if (f2 == 0)
                    {
                        execvp(tokens12[0], tokens12);
                        perror("execvp");
                        exit(1);
                    }
                    else if (f2 > 0)
                    {
                        wait(NULL);
                    }
                    else
                    {
                        perror("fork");
                    }
                }
            }
        }
        command12 = strtok_r(NULL, ";", &save12);
    }
}