
#include "baapheader.h"

void seek_in_directory(const char *base_dir, const char *target_name, int only_directories, int only_files, char results[1000][PATH_MAX], int *match_count)
{
    struct dirent *entry;
    DIR *dp = opendir(base_dir);

    if (dp == NULL)
    {
        perror("opendir");
        return;
    }

    while ((entry = readdir(dp)) != NULL)
    {
        char path[PATH_MAX];
        snprintf(path, sizeof(path), "%s/%s", base_dir, entry->d_name);

        if (strncmp(entry->d_name, target_name,strlen(target_name)) == 0)
        {
            struct stat path_stat;
            stat(path, &path_stat);

            if ((only_directories && S_ISDIR(path_stat.st_mode)) ||
                (only_files && S_ISREG(path_stat.st_mode)) ||
                (!only_directories && !only_files))
            {
                strncpy(results[*match_count], path, PATH_MAX);
                (*match_count)++;
            }
        }

        if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
        {
            seek_in_directory(path, target_name, only_directories, only_files, results, match_count);
        }
    }

    closedir(dp);
}

char *expand_path(const char *path)
{
    static char expanded_path[PATH_MAX];
    if (path[0] == '~')
    {
        snprintf(expanded_path, sizeof(expanded_path), "%s%s", getenv("HOME"), path + 1);
    }
    else
    {
        realpath(path, expanded_path);
    }
    return expanded_path;
}