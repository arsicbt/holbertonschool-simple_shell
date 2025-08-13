#include "shell.h"

void _getline(char **command, size_t *size)
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
		exit(EXIT_SUCCESS);
	}
}

int main(int argc, char *argv[], char *envp[])
{
	size_t size;
	char *command = NULL;

	(void) argv;
	while (1)
	{
    	size = 0;

    	if (isatty(STDIN_FILENO))
		{
        	printf("✿ ");
		}
		_getline(&command, &size);

	}
	return (0);
}
