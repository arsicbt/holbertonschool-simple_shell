#include "shell.h"

int main(int __attribute__((unused)) argc, char *argv[], char *envp[])
{
	size_t size;
	char *command = NULL;

	(void) argv;
	while (1)
	{
    	size = 0;

    	if (isatty(STDIN_FILENO))
		{
        	printf("✿  ");
		}
		_getline(&command, &size, *envp);
	}
	return (0);
}
