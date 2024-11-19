#include "baapheader.h"

typedef struct
{
    char alias_name[50];
    char alias_command[200];
} Alias;

Alias aliases[100];

void add_alias(const char *alias_name, const char *command)
{
    if (alias_count >= MAX_ALIASES)
    {
        printf("Alias limit reached. Cannot add more aliases.\n");
        return;
    }
    for (int i = 0; i < alias_count; i++)
    {
        if (strcmp(aliases[i].alias_name, alias_name) == 0)
        {
            strncpy(aliases[i].alias_command, command, MAX_ALIAS_COMMAND_LENGTH);
            return;
        }
    }

    // Add new alias
    strncpy(aliases[alias_count].alias_name, alias_name, MAX_ALIAS_NAME_LENGTH);
    strncpy(aliases[alias_count].alias_command, command, MAX_ALIAS_COMMAND_LENGTH);
    alias_count++;
    // printf("Alias '%s' added as '%s'\n", alias_name, command);
}
void load_myshrc()
{
    FILE *file = fopen(".myshrc", "r");
    if (!file)
    {
        printf("No .myshrc file found.\n");
        return;
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file))
    {
        char *trimmed_line = trim_spaces(line);

        // Ignore empty lines and comments
        if (trimmed_line[0] == '\0' || trimmed_line[0] == '#')
        {
            continue;
        }

        // Check if it's an alias definition
        if (strncmp(trimmed_line, "alias ", 6) == 0)
        {
            char *alias_name = strtok(trimmed_line + 6, "=");
            char *command = strtok(NULL, "\n");

            if (alias_name && command)
            {
                alias_name = trim_spaces(alias_name);
                command = trim_spaces(command);

                if (command[0] == '\'' && command[strlen(command) - 1] == '\'')
                {
                    command[strlen(command) - 1] = '\0';
                    command++;                           
                }

                if (alias_name[0] != '\0' && command[0] != '\0')
                {
                    add_alias(alias_name, command);
                }
            }
        }
    }

    fclose(file);
}

void exec_alias_comm(char *input)
{
    char *command = strdup(input);
    if (!command)
    {
        perror("strdup failed");
        return;
    }
    command = trim_spaces(command);
    for (int i = 0; i < alias_count; i++)
    {
        if (strcmp(command, aliases[i].alias_name) == 0)
        {
            free(command);
            command = strdup(aliases[i].alias_command);
            if (!command)
            {
                perror("strdup failed");
                return;
            }
            break;
        }
    }
    command = trim_spaces(command);
    char *tokens[BUFFER_SIZE];
    int tokenct = 0;
    char *save;
    char *token = strtok_r(command, " \t\n", &save);
    while (token != NULL)
    {
        char *c = malloc(sizeof(char) * 40000);
        c = trim_spaces(token);
        strcpy(token, c);
        tokens[tokenct++] = token;
        token = strtok_r(NULL, " \t\n", &save);
    }
    if (tokenct > 0 && tokens[0] != NULL)
    {
        if (strcmp(tokens[0], "hop") == 0)
        {
            hop(tokens, tokenct);
        }
        else if (strcmp(tokens[0], "reveal") == 0)
        {
            char *path11 = ".";
            int show_hidden11 = 0;
            int show_details11 = 0;
            for (int j11 = 1; j11 < tokenct; j11++)
            {
                if (tokens[j11][0] == '-')
                {
                    char *flag11 = tokens[j11] + 1;
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
                    path11 = tokens[j11];
                }
            }
            reveal(path11, show_hidden11, show_details11);
        }
        else if (strcmp(tokens[0], "proclore") == 0)
        {
            if (tokenct == 1)
            {
                char current_pid[32];
                snprintf(current_pid, sizeof(current_pid), "%d", getpid());
                proclore(current_pid);
            }
            else if (tokenct == 2)
            {
                proclore(tokens[1]);
            }
            else
                printf("INVALID COMMAND!!\n");
        }
        else if (strcmp(tokens[0], "seek") == 0)
        {
            int flagexecution = 0;
            int filesonly = 0;
            int dirsonly = 0;
            char *nametarget = NULL;
            char *search_directory = ".";
            int token_index = 1;
            while (token_index < tokenct && tokens[token_index][0] == '-')
            {
                for (char *fg = tokens[token_index] + 1; *fg; ++fg)
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
                if (token_index < tokenct)
                {
                    nametarget = tokens[token_index];
                    token_index++;
                }
                else
                {
                    printf("Usage: seek [flags] target [directory]\n");
                }
                if (token_index < tokenct)
                {
                    search_directory = expand_path(tokens[token_index]);
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
        else if (strcmp(tokens[0], "log") == 0)
        {

            if (tokenct == 2)
            {
                if (strcmp(tokens[1], "purge") == 0)
                {
                    idx = 0;
                    savetofile();
                }
            }
            else if (tokenct == 1)
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
            else if (tokenct == 3)
            {
                if (strcmp(tokens[1], "execute") == 0)
                {
                    int execindex = 0;
                    execindex = atoi(tokens[2]);
                    execindex = idx - execindex;
                    executelog(commlog[execindex]);
                }
            }
        }
        else if (strcmp(tokens[0], "fg") == 0)
        {
            pid_t pid = atoi(tokens[1]);
            fg_command(pid);
        }
        else if (strcmp(tokens[0], "bg") == 0)
        {
            pid_t pid = atoi(tokens[1]);
            bg_command(pid);
        }
        else if (strcmp(tokens[0], "neonate") == 0)
        {
            int interval = atoi(tokens[2]);
            neonate(interval);
        }
        else if(strcmp(tokens[0],"iMan")==0) {
            retrieve_and_show_man_page(tokens[1]);
        }
        else
        {
            int f = fork();
            if (f == 0)
            {
                execvp(tokens[0], tokens);
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
                // printf("%d",timeprocess);
            }
            else
            {
                perror("fork");
            }
        }
    }
    free(command);
}
void checkalias(char *str)
{
    for (int pt = 0; pt < alias_count; pt++)
    {
        if (strcmp(str, aliases[pt].alias_name) == 0)
        {
            exec_alias_comm(aliases[pt].alias_command);
            is_alias = 1;
            break;
        }
    }
}
char *funcalias(char *str)
{
    for (int pt = 0; pt < alias_count; pt++)
    {
        if (strcmp(str, aliases[pt].alias_name) == 0)
        {
            is_alias = 1;
            return aliases[pt].alias_command;
        }
    }
}
