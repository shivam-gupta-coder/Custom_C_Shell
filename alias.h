#ifndef ALIAS_H
#define ALIAS_H

#include "baapheader.h"
void add_alias(const char *alias_name, const char *command);
void load_myshrc();
void exec_alias_comm(char* input);
void checkalias(char* str);
char* funcalias(char* str);

#endif 