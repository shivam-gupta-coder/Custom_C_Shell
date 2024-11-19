#include "baapheader.h"

char *logFilePath = "/home/shivamgupta/Desktop/proj1/comms.txt";
char *pids = "/home/shivamgupta/Desktop/proj1/spec13.txt";
char *aliasfile = "/home/shivamgupta/Desktop/proj1/.myshrc";
char commlog[LOG_NUM][MAX_STRING_LENGTH];
int commands = 0;
int idx = 0;
char currdir[PATH_MAX];
char prevdir[PATH_MAX];
char home[PATH_MAX];
int flag2sec = 0;
char command2sec[PATH_MAX];
int timeprocess = 0;
int pcount = 0;
int neo_flag = 0;
pid_t fg_pid = -1;
char controlzcommand[1024];
int is_alias = 0;
int alias_count = 0;
typedef struct
{
    bool only_dirs;
    bool only_files;
    bool execute_action;
} Flags;
void handle_sigint(int sig)
{
    printf("%d", fg_pid);
    if (fg_pid != -1)
    {
        kill(fg_pid, SIGKILL);
        status_of_process();
        printf("\nForeground process %d interrupted.\n", fg_pid);
        fg_pid = -1;
    }
    else
    {
        printf("\nNo foreground process to interrupt.\n");
    }
}
void handle_sigtstp(int sig)
{
    if (fg_pid != -1)
    {
        push_process(fg_pid, controlzcommand, 1);
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

void handle_sigchld(int sig)
{
    int status;
    pid_t pid;
    status_of_process();
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0)
    {
        if (WIFEXITED(status))
        {

            printf("Process %d exited with status %d\n", pid, WEXITSTATUS(status));
        }
        else if (WIFSIGNALED(status))
        {
            // update_process_states();
            printf("Process %d was terminated by signal %d\n", pid, WTERMSIG(status));
        }
        else
        {
            //  update_process_states();
            printf("Process %d did not exit normally\n", pid);
        }
    }
}
void setup_signal_handlers()
{
    struct sigaction sa_int, sa_tstp;
    sa_int.sa_handler = handle_sigint;
    sigemptyset(&sa_int.sa_mask);
    sa_int.sa_flags = SA_RESTART;
    sigaction(SIGINT, &sa_int, NULL); // Ctrl-C

    sa_tstp.sa_handler = handle_sigtstp;
    sigemptyset(&sa_tstp.sa_mask);
    sa_tstp.sa_flags = SA_RESTART;
    sigaction(SIGTSTP, &sa_tstp, NULL); // Ctrl-Z

    struct sigaction sa;
    sa.sa_handler = &handle_sigchld;
    sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;
    sigaction(SIGCHLD, &sa, 0);
    status_of_process();
}
int main()
{

    gethome();
    char input[BUFFER_SIZE];

    FILE *file = fopen(logFilePath, "r");
    if (file)
    {
        while (idx < 15 && fgets(commlog[idx], sizeof(commlog[idx]), file))
        {
            commlog[idx][strcspn(commlog[idx], "\n")] = '\0';
            if (idx < 15)
            {
                idx++;
            }
        }
        fclose(file);
    }
    setup_signal_handlers();
    while (1)
    {
        int logfg = 0;
        shellprompt();
        flag2sec = 0;
        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            break;
        }
        char ip[BUFFER_SIZE];
        strcpy(ip, input);
        input[strcspn(input, "\n")] = '\0';
        if (strstr(input, "log") != NULL)
        {
            logfg = 1;
        }
        else
            logfg = 0;
        ip[strcspn(ip, "\n")] = '\0';
        if (!logfg)
            storeloginarray(input);
        char *save1;
        char *command11 = strtok_r(ip, ";", &save1);
        while (command11 != NULL)
        {
            command11 = trim_spaces(command11);
            if (command11 == NULL || *command11 == '\0')
            {
                command11 = strtok_r(NULL, ";", &save1);
                continue;
            }
            if (strstr(command11, ">") != NULL || strstr(command11, "<") != NULL || strstr(command11, ">>") != NULL || strstr(command11, "|") != NULL)
            {
                ampercent(command11);
            }
            else
            {

                while (strchr(command11, '&') != NULL)
                {
                    char *background_command = strchr(command11, '&');
                    *background_command = '\0';
                    char *background_trimmed_command = trim_spaces(command11);
                    if (background_trimmed_command == NULL || *background_trimmed_command == '\0')
                    {
                        break;
                    }
                    // push_process(getpid,background_trimmed_command,1);
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
                            execvp(args[0], args);
                            perror("execvp");
                            exit(1);
                        }
                        else if (pid > 0)
                        {
                            push_process(pid, background_trimmed_command, 1);
                            struct sigaction sa;
                            sa.sa_handler = &handle_sigchld;
                            sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;
                            sigaction(SIGCHLD, &sa, 0);
                            printf("[%d] %d\n", pid, pid);
                            // pid bg_trimmed_command running
                        }
                        else
                        {
                            int status;
                            waitpid(pid, &status, 0);
                            if (WIFEXITED(status))
                            {
                                printf("PROCESSS EXITED\n");
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
                            sa.sa_handler = &handle_sigchld;
                            sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;
                            sigaction(SIGCHLD, &sa, 0);
                            printf("[%d] %d\n", pid, pid);
                            // pid bg_trimmed_command running
                        }
                        else
                        {
                            int status;
                            waitpid(pid, &status, 0);
                            if (WIFEXITED(status))
                            {
                                printf("PROCESSS EXITED\n");
                            }
                            perror("fork");
                        }
                    }

                    command11 = background_command + 1;
                    command11 = trim_spaces(command11);
                }

                char *tokens11[BUFFER_SIZE];
                int tokenct1 = 0;
                char *save2;

                char *token11 = strtok_r(command11, " \t\n ", &save2);

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
                    is_alias = 0;
                    checkalias(tokens11[0]);
                    if (!is_alias)
                    {
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
                                    return 1;
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
                                    executelog(commlog[execindex]);
                                }
                            }
                        }
                        else if (strcmp(tokens11[0], "activities") == 0)
                        {
                            spec13();
                        }
                        else if (strcmp(tokens11[0], "neonate") == 0)
                        {
                            char strcommand[256];
                            snprintf(strcommand, sizeof(strcommand), "%s %s %s", tokens11[0], tokens11[1],tokens11[2]);
                            strcpy(controlzcommand, strcommand);
                            int time_arg = atoi(tokens11[2]);
                            if (time_arg <= 0)
                            {
                                fprintf(stderr, "Invalid time argument\n");
                                return EXIT_FAILURE;
                            }
                             struct timeval start, end;
                             gettimeofday(&start, NULL);
                            neonate(time_arg);
                             gettimeofday(&end, NULL);
                             double time_taken = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
                                if (time_taken > 2.00)
                                {
                                    timeprocess = (int)time_taken;
                                    flag2sec = 1;
                                    strcpy(command2sec, command11);
                                }
                        }
                        else if (strcmp(tokens11[0], "fg") == 0)
                        {
                            int procid = atoi(tokens11[1]);
                            fg_command(procid);
                            // update_process_states();
                        }
                        else if (strcmp(tokens11[0], "bg") == 0)
                        {
                            int procid = atoi(tokens11[1]);
                            bg_command(procid);
                            // update_process_states();
                        }
                        else if (strcmp(tokens11[0], "ping") == 0)
                        {
                            int procid = atoi(tokens11[1]);
                            int signo = atoi(tokens11[2]);
                            ping_command(procid, signo);
                        }
                        else if (strcmp(tokens11[0], "iMan") == 0)
                        {
                            retrieve_and_show_man_page(tokens11[1]);
                        }
                        else
                        {
                            char strcommand[256];
                            snprintf(strcommand, sizeof(strcommand), "%s %s", tokens11[0], tokens11[1]);
                            strcpy(controlzcommand, strcommand);

                            int f = fork();
                            if (f == 0)
                            {
                                fg_pid = getpid();
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
                                int stat;
                                waitpid(f, &stat, WUNTRACED);
                                gettimeofday(&end, NULL);
                                double time_taken = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
                                if (time_taken > 2.00)
                                {
                                    timeprocess = (int)time_taken;
                                    flag2sec = 1;
                                    strcpy(command2sec, command11);
                                }
                                // printf("%d",timeprocess);
                            }
                            else
                            {
                                perror("fork");
                            }
                        }
                    }
                }
            }
            command11 = strtok_r(NULL, ";", &save1);
        }
    }
    return 0;
}
