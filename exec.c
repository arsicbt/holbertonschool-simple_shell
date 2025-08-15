#include "shell.h"

/**
 * print_error - print an error message
 * @command: the command to insert in the message
 *
 * Return: void
 */ 

void print_error(char *command[])
{
	fprintf(stderr, "%s: not found\n", command[0]);
	free(*command);
	exit(127);
}

/**
 * _getenv - get environment variable
 * @name: name of environment variable
 * @envp: pointer to environment variables
 * Return: pointer to environment variable
 */

char *_getenv(const char *name, char **envp)
{
	char **env, *separator;

	for (env = envp; *env != NULL; env++)
	{
		separator = strchr(*env, '=');
		if (separator != NULL)
		{
			if (strncmp(*env, name, separator - *env) == 0)
			{
				return (separator + 1);
			}
		}
	}
	return (NULL);
}

/**
 * pathfind - find the correct path for given input
 * @command: given input to chack for path
 * @cmd: argument
 * @envp: pointer to environment variables
 *
 * Return: a pointer to the string array or NULL if failed
 */
char **pathfind(char *cmd, char **command, char **envp)
{
	char *fullpath = NULL, *current_path, *temp_path, *tokken_path;

	if (strchr(cmd, '/') != NULL && access(cmd, F_OK) == 0)
	{
		command[0] = cmd;
		return (command);
	}
	current_path = _getenv("PATH", envp);
	temp_path = strdup(current_path);
	tokken_path = strtok(temp_path, ":");

	while (tokken_path)
	{
		fullpath = malloc(strlen(cmd) + strlen(tokken_path) + 2);
		sprintf(fullpath, "%s/%s", tokken_path, cmd);
		if (access(fullpath, F_OK) == 0)
		{
			command[0] = fullpath;
			free(temp_path);
			return (command);
		}
		tokken_path = strtok(NULL, ":");
		free(fullpath);
	}
	free(temp_path);
	return (NULL);
}

/**
 * execute - function to execute commands
 * @command: input froom user
 * @envp: enviroment path
 * Return: -1 if failed and 0 if success
 */

int execute(char *command[], char **envp)
{
	pid_t pid;
	int status;
	int i;
	char **temp = pathfind(command[0], command, envp);

	if (temp != NULL)
	{
		pid = fork();
		if (pid < 0)
		{
			perror("fork failed");
			exit(EXIT_FAILURE);
		}
		else if (pid == 0)
		{
			if (_getenv("PATH", envp) == NULL && access(command[0], F_OK) != 0)
			{
				print_error(command);
			}
			if (execve(temp[0], command, envp) == -1)
			{
				perror("error:");
				exit(EXIT_FAILURE);
			}
		}
		wait(&status);
	}
	else
	{
		print_error(command);
	}
	return (0);
}
