#ifndef shell_h
#define shell_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

int execute(char *command[], char **envp);
char **pathfind(char *cmd, char **command, char **envp);
char *_getenv(const char *name, char **envp);
void _getline(char *command[], size_t *size, char **envp);

#endif