#include "shell.h"

void _getline(char **command, size_t *size, char *envp[])
{
	size_t size_byte;
	size_byte = getline(command, size, stdin);

	if ((int) size_byte == EOF)
	{
		if (isatty(STDIN_FILENO) != 0)
		{
			printf("\n");
		}
		if (*command != NULL)
		{
			free(*command);
		}
		execute(*command[], envp);
		exit(EXIT_SUCCESS);
	}
}
