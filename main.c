#include "shell.h"

int main(int argc, char *argv[], char *envp[])
{
	size_t size;
	char *command = NULL;
    pid_t pid;

	(void) argv;
	while (1)
	{
    	size = 0;

    	if (isatty(STDIN_FILENO))
		{
        	printf("✿ ");
		}
		_getline(&command, &size);
        execute(&command, envp);
	}
	return (0);
}
