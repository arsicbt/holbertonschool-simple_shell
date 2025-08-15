#include "shell.h"

/**
 * _getline - Reads a line from standard input and executes the command
 *
 * This function:
 * - Removes the newline character if present
 * - Splits the command into tokens (arguments) using strtok
 * - Stores the tokens into an array (args_cmd)
 * - If the input is EOF (Ctrl+D), it handles cleanup and exits
 * - If at least one argument is parsed, it calls the execute() function
 *   to run the command with the given environment
 *
 * Parameters:
 * @command: Pointer to a buffer containing the user's input
 * @size: Pointer to a variable holding the size of the buffer
 * @envp: Environment variables passed to the execute() function
 *
 * Return: void (but calls exit() on EOF)
 */

void _getline(char *command[], size_t *size, char **envp)
{
	ssize_t read_len = getline(command, size, stdin);
	char *p = strchr(*command, '\n');
	char *token;
	char *args_cmd[64];
	int i = 0;

	if (read_len == EOF)
	{
		if (isatty(STDIN_FILENO) != 0)
		{
			printf("\n");
		}
		if (*command != NULL)
		{
			free(*command);
		}
		exit(EXIT_SUCCESS);
	}

	if (p)
	{
		*p = '\0';
	}

	if (strcmp(*command, "exit") == 0)
	{
		free(*command);
		exit(EXIT_SUCCESS);
	}
	if (strcmp(command, "env") == 0)
	{
		print_env(envp);
		free(command);
		exit(EXIT_SUCCESS);
	}
	token = strtok(*command, " ");
	while (token != NULL && i < 63)
	{
		args_cmd[i] = token;
		i++;
		token = strtok(NULL, " ");
	}
	args_cmd[i] = NULL;

	if (i > 0)
		execute(args_cmd, envp);
}
