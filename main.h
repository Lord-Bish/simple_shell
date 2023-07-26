#ifndef MAIN_H
#define MAIN_H

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <stdbool.h>

extern char **environ;

ssize_t custom_getline(char **lineptr, size_t *n);
void add_alias(const char *name, const char *value);
void print_aliases(void);
void free_aliases(void);
char *get_alias_value(const char *name);
void print_env(char **env);
void _setenv(char *command, char *env_val, char *env_var);
void _cd(char *command, char *dir);
void ex(char *command, char **args, pid_t pid, char *argv, int *j, char **env);

#endif
