#ifndef REVEAL_H
#define REVEAL_H

#include "baapheader.h"
void print_colorized_name(const char *name, mode_t mode);
void print_file_details(const char *path, const char *name);
void reveal_directory(const char *path, int show_all, int show_details);
void reveal(const char *path, int show_all, int show_details);

#endif
