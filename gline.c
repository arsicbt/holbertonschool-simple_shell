#include "shell.h"

/**
 * space_tab - Checks if a string contains only spaces and/or tabs
 * @str: The string to be checked
 *
 * Description:
 * This function iterates through the given string to verify
 * whether it is composed solely of space (' ') and tab ('\t')
 * characters. If the string is NULL or contains only spaces/tabs,
 * the function returns 1. Otherwise, it returns 0.
 *
 * Return: 1 if the string is NULL or contains only spaces/tabs
 * 0 if the string has at least one non-space and non-tab character
 */
int space_tab(char *str)
{
	int i = 0;

	if (str == NULL)
		return (1);

	while (str[i] != '\0')
	{
		if (str[i] != ' ' && str[i] != '\t')
			return (0);
		i++;
	}
	return (1);
}

/**
 * read_command - Reads a command from stdin
 * @command: Pointer to buffer that stores the command
 * @size: Pointer to size of the buffer
 *
 * Return: number of characters read, or -1 on failure
 */
ssize_t read_command(char **command, size_t *size)
{
	ssize_t read_len = getline(command, size, stdin);

	if (read_len == -1)
	{
		if (feof(stdin))
		{
			printf("\n");
			exit(EXIT_SUCCESS);
		}
		else
		{
			exit(EXIT_FAILURE);
		}
	}
	if ((*command)[read_len - 1] == '\n')
		(*command)[read_len - 1] = '\0';

	return (read_len);
}

/**
 * handle_builtin - Handles built-in commands (exit, env)
 * @command: The command string
 * @envp: Environment variables
 */
void handle_builtin(char *command, char **envp)
{
	if (strcmp(command, "exit") == 0)
	{
		free(command);
		exit(EXIT_SUCCESS);
	}
	if (strcmp(command, "env") == 0)
	{
		print_env(envp);
	}
}

/**
 * parse_and_execute - Tokenizes the command and executes it
 * @command: The command string
 * @envp: Environment variables
 * @prog_name: Program name
 */
void parse_and_execute(char *command, char **envp, char *prog_name)
{
	char *args_cmd[20], *token;
	int i = 0;

	if (space_tab(command))
		return;

	if (command[0] != '\0')
	{
		token = strtok(command, " ");
		while (token != NULL && i < 20)
		{
			args_cmd[i++] = token;
			token = strtok(NULL, " ");
		}
		args_cmd[i] = NULL;
		execute(args_cmd, envp, prog_name);
	}
}

/**
 * check_command - Main function to process a command
 * @command: Pointer to buffer that stores the command
 * @size: Pointer to size of the buffer
 * @envp: Environment variables
 * @prog_name: Program name
 */
void check_command(char **command, size_t *size, char **envp, char *prog_name)
{
	read_command(command, size);

	handle_builtin(*command, envp);

	parse_and_execute(*command, envp, prog_name);
}
