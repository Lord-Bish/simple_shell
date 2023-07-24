#include "main.h"

#define MAX_INPUT_LENGTH 100

extern char **environ;

void display_prompt() {
    printf("($) ");
    fflush(stdout);
}

void execute_command(char* command) {
    char *args[3];
    int status;
    pid_t pid;
    char* env_var;
    char* env_val;
    char* status_str;
    int exit_status;

    if (strcmp(command, "exit") == 0) {
        exit(EXIT_SUCCESS);
    } else if (strncmp(command, "exit ", 5) == 0) {
        status_str = command + 5;
        exit_status = atoi(status_str);
        exit(exit_status);
    } else if (strncmp(command, "setenv ", 7) == 0) {
        env_var = strtok(command + 7, " ");
        env_val = strtok(NULL, " ");
        if (env_var != NULL && env_val != NULL) {
            if (setenv(env_var, env_val, 1) != 0) {
                fprintf(stderr, "Failed to set environment variable\n");
            }
        } else {
            fprintf(stderr, "Invalid setenv syntax\n");
        }
        return;
    } else if (strncmp(command, "unsetenv ", 9) == 0) {
        env_var = strtok(command + 9, " ");
        if (env_var != NULL) {
            if (unsetenv(env_var) != 0) {
                fprintf(stderr, "Failed to unset environment variable\n");
            }
        } else {
            fprintf(stderr, "Invalid unsetenv syntax\n");
        }
        return;
    } else if (strncmp(command, "cd ", 3) == 0) {
        char* dir = strtok(command + 3, " ");
        if (dir == NULL || strcmp(dir, "-") == 0) {
            dir = getenv("HOME");
            if (dir == NULL) {
                fprintf(stderr, "Failed to get home directory\n");
                return;
            }
        }
        if (chdir(dir) != 0) {
            fprintf(stderr, "Failed to change directory\n");
            return;
        }
        if (setenv("PWD", dir, 1) != 0) {
            fprintf(stderr, "Failed to update PWD environment variable\n");
        }
        return;
    } else if (strcmp(command, "env") == 0) {
        char **env = environ;
        while (*env) {
            printf("%s\n", *env);
            env++;
        }
        return;
    }

    pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        /* Child process */
        args[0] = command;
        args[1] = NULL;

        /* Check if the command exists in the PATH */
        if (access(command, X_OK) != 0) {
            printf("./hsh: 1: %s: command not found\n", args[0]);
            exit(EXIT_FAILURE);
        }

        if (execve(command, args, environ) == -1) {
            perror("execve");
            exit(EXIT_FAILURE);
        }
    } else {
        /* Parent process */
        waitpid(pid, &status, 0);
    }
}

int main() {
    char *line = NULL;
    size_t bufsize = 0;
    ssize_t chars_read;

    while (1) {
        display_prompt();

        chars_read = getline(&line, &bufsize, stdin);

        if (chars_read == -1) {
            /* End of file (Ctrl+D) or error occurred */
            if (feof(stdin)) {
                printf("\n");
                break;
            } else {
                perror("getline");
                exit(EXIT_FAILURE);
            }
        }

        /* Remove the trailing newline character */
        line[strcspn(line, "\n")] = '\0';

        execute_command(line);
    }

    free(line);
    exit(EXIT_SUCCESS);
}
