#include "shell.h"

void _getline(char **command, size_t *size)
{
	size_t size_byte;
	size_byte = getline(command, size, stdin);

	if ((int) size_byte == EOF)
	{
		// when Ctrl +D, \n
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
}


