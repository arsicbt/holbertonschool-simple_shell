#include "shell.h"

/**
 * print_error - Prints a standard error message when a command is not found
 *
 * This function:
 * - Displays an error message to the standard error stream (stderr),
 *   indicating that the given command could not be found
 * - The message format matches the common shell error style
 *
 * Parameters:
 * @command: An array of strings where command[0] is the name of the command
 *
 * Return: standard exit status: 127
 */

int print_error(char *prog_name, char *command[])
{
	fprintf(stderr, "%s: 1: %s: not found\n", prog_name, command[0]);
	return (127);
}

/**
 * print_env - Prints the environment variables to standard output.
 *
 * This function:
 * - Iterates through the array of environment variables.
 * - Prints each variable on a new line using printf().
 *
 * Parameters:
 * @envp: A NULL-terminated array of strings containing environment variables.
 *
 * Return:
 * - 0 on success.
 * - -1 if the envp array is NULL.
 */

int print_env(char **envp)
{
	int i = 0;

	if (envp == NULL)
		return (-1);
	while (envp[i])
	{
		printf("%s\n", envp[i++]);
	}
	return (0);
}

/**
 * _getenv - Retrieves the value of an environment variable.
 *
 * This function:
 * - Iterates through the given environment variable array
 * - Looks for a variable that matches the provided name.
 * - If found, returns a pointer to the value part
 *
 * Parameters:
 * @name: The name of the environment variable to search for
 * @envp: The environment variables array
 *
 * Return:
 * - A pointer to the value of the variable if found
 * - NULL if the variable is not found or if envp is invalid.
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
 * pathfind - Resolves the full path of a given command
 * using the PATH environment variable.
 *
 * This function:
 * - Checks if the command contains a '/' and is directly accessible
 *     - If so, it returns the original command unchanged.
 * - Otherwise, it retrieves the PATH environment variable using _getenv()
 * - It tokenizes the PATH using ':' as a delimiter.
 * - For each directory in PATH, it builds a full path by appending '/' + cmd
 *     - If the built path exists, it updates command[0]
 *       to point to this full path and returns the updated command array
 * - If no valid path is found, it returns NULL
 *
 * Parameters:
 * @cmd: The command name
 * @command: The array of command arguments
 * @envp: Array of environment variables
 *
 * Return:
 * - If a valid full path is found: returns the modified command array
 * - If not found: returns NULL
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
 * execute - Executes a command using fork and execve.
 *
 * This function:
 * - Tries to resolve the command's full path using pathfind()
 * - If a valid path is found:
 *     - It creates a child process using fork()
 *     - In the child process:
 *         - It checks if PATH is set and if the command is accessible
 *         - If not accessible, it calls print_error()
 *         - It then attempts to execute the command using execve()
 *         - If execve() fails, it prints an error and exits
 *     - The parent process waits for the child to finish
 * - If the command path is not found, it calls print_error()
 *
 * Parameters:
 * @command: Array of strings representing the command and its arguments
 * @envp: Array of environment variables
 *
 * Return: Always returns 0
 */

int execute(char *command[], char **envp, char *prog_name)
{
	pid_t pid;
	int status;
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
				return print_error(prog_name, command);
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
		return print_error(prog_name, command);
	}
	return (0);
}
