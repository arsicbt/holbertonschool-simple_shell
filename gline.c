#include "shell.h"

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
		/*execute(args_cmd, envp);*/
		exit(EXIT_SUCCESS);
	}
	if (p)
    	*p = '\0';

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
