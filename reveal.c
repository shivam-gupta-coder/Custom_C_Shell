
#include "baapheader.h"
void print_colorized_name(const char *name, mode_t mode)
{
    if (S_ISDIR(mode))
    {
        printf("\033[1;34m%s\033[0m\n", name);
    }
    else if (mode & S_IXUSR)
    {
        printf("\033[1;32m%s\033[0m\n", name);
    }
    else
    {
        printf("%s\n", name);
    }
}

void print_file_details(const char *path, const char *name)
{
    struct stat fileStat;
    char full_path[BUFFER_SIZE];
    snprintf(full_path, sizeof(full_path), "%s/%s", path, name);

    if (stat(full_path, &fileStat) < 0)
    {
        perror("stat");
        return;
    }

    printf((S_ISDIR(fileStat.st_mode)) ? "d" : "-");
    printf((fileStat.st_mode & S_IRUSR) ? "r" : "-");
    printf((fileStat.st_mode & S_IWUSR) ? "w" : "-");
    printf((fileStat.st_mode & S_IXUSR) ? "x" : "-");
    printf((fileStat.st_mode & S_IRGRP) ? "r" : "-");
    printf((fileStat.st_mode & S_IWGRP) ? "w" : "-");
    printf((fileStat.st_mode & S_IXGRP) ? "x" : "-");
    printf((fileStat.st_mode & S_IROTH) ? "r" : "-");
    printf((fileStat.st_mode & S_IWOTH) ? "w" : "-");
    printf((fileStat.st_mode & S_IXOTH) ? "x" : "-");

    printf(" %lu", fileStat.st_nlink);
    printf(" %s", getpwuid(fileStat.st_uid)->pw_name);
    printf(" %s", getgrgid(fileStat.st_gid)->gr_name);
    printf(" %ld", fileStat.st_size);

    char timebuff[80];
    struct tm *timeinfo = localtime(&fileStat.st_mtime);
    strftime(timebuff, sizeof(timebuff), "%b %d %H:%M", timeinfo);
    printf(" %s ", timebuff);

    print_colorized_name(name, fileStat.st_mode);
}

void reveal_directory(const char *path, int show_all, int show_details)
{
    DIR *dir;
    struct dirent *entry;

    if (!(dir = opendir(path)))
    {
        perror("opendir");
        return;
    }

    char *files[BUFFER_SIZE];
    int file_count = 0;

    while ((entry = readdir(dir)) != NULL)
    {
        if (!show_all && entry->d_name[0] == '.')
        {
            continue;
        }

        files[file_count++] = strdup(entry->d_name);
    }
    closedir(dir);
    qsort(files, file_count, sizeof(char *), (int (*)(const void *, const void *))strcmp);

    for (int i = 0; i < file_count; i++)
    {
        if (show_details)
        {
            print_file_details(path, files[i]);
        }
        else
        {
            struct stat fileStat;
            char full_path[BUFFER_SIZE];
            snprintf(full_path, sizeof(full_path), "%s/%s", path, files[i]);
            stat(full_path, &fileStat);
            print_colorized_name(files[i], fileStat.st_mode);
        }
    }
}

void reveal(const char *path, int show_all, int show_details)
{
    if (!path)
    {
        path = ".";
    }

    reveal_directory(path, show_all, show_details);
}