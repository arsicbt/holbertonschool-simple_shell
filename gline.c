#include "shell.h"

/**
 * read_command - Reads a line from standard input and executes the command
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
 * @prog_name: Name of the executing program
 *
 * Return: void (but calls exit() on EOF)
 */

void read_command(char **command, size_t *size, char **envp, char *prog_name)
{
	ssize_t read_len;
	char *args_cmd[2], *token;
	int i;

	read_len = getline(command, size, stdin);
	if (read_len == -1)
	{
		if (isatty(STDIN_FILENO) != 0)
			printf("\n");
		free(*command);
		exit(EXIT_SUCCESS);
	}
	if ((*command)[read_len - 1] == '\n')
		(*command)[read_len - 1] = '\0';

	if (strcmp(*command, "exit") == 0)
	{
		free(*command);
		exit(EXIT_SUCCESS);
	}
	if (strcmp(*command, "env") == 0)
	{
		print_env(envp);
		return;
	}
	if ((*command)[0] != '\0')
	{
		token = strtok(*command, " ");
		i = 0;
		while (token != NULL && i < 2)
		{
			args_cmd[i] = token;
			i++;
			token = strtok(NULL, " ");
		}
        args_cmd[i] = NULL;

		execute(args_cmd, envp, prog_name);
	}
}
