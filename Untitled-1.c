/**
 * check_command - Reads a line from standard input and executes the command
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
void check_command(char **command, size_t *size, char **envp, char *prog_name)
{
	ssize_t read_len;
	char *args_cmd[20], *token;
	int i;

	read_len = getline(command, size, stdin);
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
		while (token != NULL && i < 20)
		{
			args_cmd[i++] = token;
			token = strtok(NULL, " ");
		}
		args_cmd[i] = NULL;
		execute(args_cmd, envp, prog_name);
	}
}

printf("✿ ");

#include "shell.h"

/**
 * print_error - Prints a standard error message when a command is not found
 *
 * This function:
 * - Displays an error message to the standard error stream (stderr),
 *   indicating that the given command could not be found
 * - The message format matches the common shell error style
 *
 * Parameters:
 * @prog_name: Name of the executing program
 * @command: An array of strings where command[0] is the name of the command
 * @error_code: The adapted correct error code to return
 *
 * Return: standard exit status: correct error code
 */

int print_error(char *prog_name, char *command, int error_code)
{
	fprintf(stderr, "%s: 1: %s: not found\n", prog_name, command);
	return (error_code);
}

/**
 * print_env - Prints the environment variables to standard output.
 *
 * This function:
 * - Iterates through the array of environment variables.
 * - Prints each variable on a new line using printf().
 *
 * Parameters:
 * @envp: A NULL-terminated array of strings containing environment variables.
 *
 * Return:
 * - 0 on success.
 * - -1 if the envp array is NULL.
 */

int print_env(char **envp)
{
	int i = 0;

	if (envp == NULL)
		return (-1);

	while (envp[i])
	{
		printf("%s\n", envp[i++]);
	}
	return (0);
}

/**
 * _getenv - Retrieves the value of an environment variable.
 *
 * This function:
 * - Iterates through the given environment variable array
 * - Looks for a variable that matches the provided name.
 * - If found, returns a pointer to the value part
 *
 * Parameters:
 * @name: The name of the environment variable to search for
 * @envp: The environment variables array
 *
 * Return:
 * - A pointer to the value of the variable if found
 * - NULL if the variable is not found or if envp is invalid.
 */

char *_getenv(const char *name, char **envp)
{
	char **env, *separator;

	for (env = envp; *env != NULL; env++)
	{
		separator = strchr(*env, '=');
		if (separator != NULL)
		{
			if (strncmp(*env, name, separator - *env) == 0)
			{
				return (separator + 1);
			}
		}
	}
	return (NULL);
}

/**
 * pathfind - Resolves the full path of a given command
 * using the PATH environment variable.
 *
 * This function:
 * - Checks if the command contains a '/' and is directly accessible
 *     - If so, it returns the original command unchanged.
 * - Otherwise, it retrieves the PATH environment variable using _getenv()
 * - It tokenizes the PATH using ':' as a delimiter.
 * - For each directory in PATH, it builds a full path by appending '/' + cmd
 *     - If the built path exists, it updates command[0]
 *       to point to this full path and returns the updated command array
 * - If no valid path is found, it returns NULL
 *
 * Parameters:
 * @cmd: The command name
 * @envp: Array of environment variables
 *
 * Return:
 * - If a valid full path is found: returns the modified command array
 * - If not found: returns NULL
 */

char *pathfind(char *cmd, char **envp)
{
	char *fullpath = NULL, *current_path, *temp_path, *tokken_path;

	if (strchr(cmd, '/') != NULL && access(cmd, X_OK) == 0)
	{
		return (strdup(cmd));
	}

	current_path = _getenv("PATH", envp);
	if (!current_path)
		return (NULL);

	temp_path = strdup(current_path);
	if (!temp_path)
		return (NULL);

	tokken_path = strtok(temp_path, ":");

	while (tokken_path)
	{
		fullpath = malloc(strlen(tokken_path) + strlen(cmd) + 2);
		if (!fullpath)
		{
			free(temp_path);
			return (NULL);
		}
		sprintf(fullpath, "%s/%s", tokken_path, cmd);

		if (access(fullpath, X_OK) == 0)
		{
			free(temp_path);
			return (fullpath);
		}
		free(fullpath);
		tokken_path = strtok(NULL, ":");
	}
	free(temp_path);
	return (NULL);
}

/**
 * execute - Executes a command using fork and execve.
 *
 * This function:
 * - Tries to resolve the command's full path using pathfind()
 * - If a valid path is found:
 *     - It creates a child process using fork()
 *     - In the child process:
 *         - It checks if PATH is set and if the command is accessible
 *         - If not accessible, it calls print_error()
 *         - It then attempts to execute the command using execve()
 *         - If execve() fails, it prints an error and exits
 *     - The parent process waits for the child to finish
 * - If the command path is not found, it calls print_error()
 *
 * Parameters:
 * @command: Array of strings representing the command and its arguments
 * @envp: Array of environment variables
 * @prog_name: Name of the executing program
 *
 * Return: Always returns 0
 */

int execute(char *command[], char **envp, char *prog_name)
{
	pid_t pid;
	int status;
	char *temp = pathfind(command[0], envp);

	if (!temp)
		return (print_error(prog_name, command[0], 127));

	if (access(temp, X_OK) != 0)
	{
		free(temp);
		return (print_error(prog_name, command[0], 126));
	}

	pid = fork();
	if (pid < 0)
	{
		perror("fork failed");
		free(temp);
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		if (execve(temp, command, envp) == -1)
		{
			perror("error");
			free(temp);
			exit(127);
		}
	}
	else
    {
        waitpid(pid, &status, 0);
        free(temp);

        if (WIFEXITED(status))
            return (WEXITSTATUS(status));  /* <--- renvoie le vrai code de sortie */
        else
            return (EXIT_FAILURE);
    }
	return (EXIT_FAILURE);
}


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
 *
 * Return: 1 if the command is a built-in (exit or env),
 * 0 otherwise.
 */
int handle_builtin(char *command, char **envp)
{
	char *copy, *arg, *token;
	int status;

	if (!command)
		return (0);

	copy = strdup(command);
	if (!copy)
	{
		return (0);
	}
	token = strtok(copy, " \t");
	if (token && strcmp(token, "exit") == 0)
	{
		arg = strtok(NULL, " \t");
		if (arg)
            status = atoi(arg);

		else
            status = EXIT_SUCCESS;
		
		free(copy);
        free(command);
        exit(status);
	}
	
	if (token && strcmp(command, "env") == 0)
	{
		print_env(envp);
		free(copy);
		return (1);
	}
	free(copy);
	return (0);
}

/**
 * parse_and_execute - Tokenizes the command and executes it
 * @command: The command string
 * @envp: Environment variables
 * @prog_name: Program name
 */
int parse_and_execute(char *command, char **envp, char *prog_name)
{
	char *args_cmd[20], *token;
	int i = 0;

	if (space_tab(command) && command[0] == '\0')
		return (0);

	if (handle_builtin(command, envp))
		return (0);

	token = strtok(command, " ");
	while (token != NULL && i < 20)
	{
		args_cmd[i++] = token;
		token = strtok(NULL, " ");
	}
	args_cmd[i] = NULL;

	return (execute(args_cmd, envp, prog_name));
}

/**
 * check_command - Main function to process a command
 * @command: Pointer to buffer that stores the command
 * @size: Pointer to size of the buffer
 * @envp: Environment variables
 * @prog_name: Program name
 */
int check_command(char **command, size_t *size, char **envp, char *prog_name)
{
	read_command(command, size);

	return (parse_and_execute(*command, envp, prog_name));
}


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
	char *command = NULL, status = 0;
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
			printf("✿ ");
			fflush(stdout);
		}
		status = check_command(&command, &size, envp, argv[0]);

		if (!isatty(STDIN_FILENO))
            break;
	}

	return (status);
}

