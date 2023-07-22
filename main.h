#ifndef MAIN_H
#define MAIN_H

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

ssize_t custom_getline(char **lineptr, size_t *n);
struct AliasNode {                                          char *name;                                             char *value;                                            struct AliasNode *next;                             };
void add_alias(const char *name, const char *value);
void print_aliases();
void free_aliases();
char* get_alias_value(const char *name);

#endif
