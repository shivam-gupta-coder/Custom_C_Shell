#ifndef SEEK_H
#define SEEK_H

#include "baapheader.h"  // Include the main header with standard libraries and global variables

// Function declarations
void seek_in_directory(const char *base_dir, const char *target_name, int only_directories, int only_files, char results[1000][PATH_MAX], int *match_count);
char *expand_path(const char *path);

#endif // SEEK_H
