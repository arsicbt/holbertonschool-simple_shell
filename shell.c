#include "shell.h"

int main(int argc, char *argv[], char *envp[])
{
	size_t size;
	char *command = NULL;

	(void) argc;
	(void) argv;

	size = 0;

	while (1)
	{
    	if (isatty(STDIN_FILENO))
		{
        	printf("✿  ");
			_getline(&command, &size, envp);
		}
	}
	free(command);
	return (0);
}
