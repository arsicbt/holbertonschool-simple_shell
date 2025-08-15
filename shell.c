#include "shell.h"

/**
 * main - Entry point of a simple shell-like program.
 *
 * This function runs an infinite loop where it:
 * - Checks if the input is from an interactive terminal using isatty().
 * - Displays a custom prompt (✿) if interactive.
 * - Reads a command from the user using a custom _getline function.
 *
 * Parameters:
 * @argc: Number of command-line arguments (unused).
 * @argv: Array of command-line arguments (unused).
 * @envp: Array of environment variables, passed to _getline.
 *
 * Return: Always returns 0.
 */

int main(int argc, char *argv[], char *envp[])
{
	size_t size;
	char *command = NULL;
	(void) argc;

	size = 0;

	if (argc > 1)
	{
		printf("%s: command does not exist\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	while (1)
	{
		if (isatty(STDIN_FILENO))
		{
			printf("✿  ");
			fflush(stdout);
		}
		read_command(&command, &size, envp, argv[0]);
	}
	return (0);
}
