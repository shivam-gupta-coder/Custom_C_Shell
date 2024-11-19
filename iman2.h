#ifndef IMAN2_H
#define IMAN2_H
#include "baapheader.h"

void fetch_man_page(char* command);
void remove_html_tags(char* str);
void show_cleaned_man_page();
void retrieve_and_show_man_page(char *command);

#endif 