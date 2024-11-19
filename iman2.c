#include "baapheader.h"

void fetch_man_page(char *cmd_input)
{
    char link[256];
    snprintf(link, sizeof(link), "http://man.he.net/?topic=%s&section=all", cmd_input);
    char *args[] = {"wget", "-q", "-O", "/tmp/manpage.html", link, NULL};

    execvp(args[0], args); 
    perror("Error: exec failed for wget");
    exit(1);
}

void remove_html_tags(char *html_string)
{
    char *source = html_string;
    char *destination = html_string;
    int inside_tag = 0;

    while (*source)
    {
        if (*source == '<')
            inside_tag = 1;
        else if (*source == '>')
            inside_tag = 0;
        else if (!inside_tag)
            *destination++ = *source;
        source++;
    }
    *destination = '\0';
}

void show_cleaned_man_page()
{
    FILE *html_file = fopen("/tmp/manpage.html", "r");
    if (!html_file)
    {
        perror("Error opening file");
        exit(1);
    }

    fseek(html_file, 0, SEEK_END);
    long file_size = ftell(html_file);
    fseek(html_file, 0, SEEK_SET);

    char *buffer = malloc(file_size + 1);
    if (!buffer)
    {
        perror("Memory allocation error");
        fclose(html_file);
        exit(1);
    }

    fread(buffer, 1, file_size, html_file);
    buffer[file_size] = '\0';
    fclose(html_file);

    remove_html_tags(buffer);

    int idx = 0;
    int prev_space = 0;

    while (buffer[idx] != '\0')
    {
        char current_char = buffer[idx];

        if (current_char == '\n')
        {
            if (!prev_space)
                putchar('\n');
            prev_space = 0;
        }
        else if (current_char == '\r')
        {
            if (!prev_space)
                putchar('\n');
            prev_space = 0;
        }
        else if (current_char == ' ')
        {
            if (!prev_space)
                putchar(' ');
            prev_space = 1;
        }
        else if (current_char == '\t')
        {
            if (!prev_space)
                putchar(' ');
            prev_space = 1;
        }
        else
        {
            putchar(current_char);
            prev_space = 0;
        }

        idx++;
    }
    free(buffer);
}

void retrieve_and_show_man_page(char *cmd_input)
{
    pid_t pid = fork();
    if (pid == 0)
    {
        fetch_man_page(cmd_input); 
    }
    else if (pid > 0)
    {
        wait(NULL); 

        show_cleaned_man_page(); 
    }
    else
    {
        perror("Fork failed");
        exit(1);
    }
}