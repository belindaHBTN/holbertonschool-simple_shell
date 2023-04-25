#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <ctype.h>

/**
 * interactive_shell - check if it an interactive or not
 *
 * Description: check if it an interactive or not
 * Return: nothing
 **/
void interactive_shell(void)
{
	if (isatty(STDIN_FILENO) == 1)
	{
		printf("#cisfun$ ");
	}

	return;
}


/**
 * execute_cmd - execute the command
 * @cmd: command
 *
 * Description: execute the command
 * Return: On success, no return, on erro, return -1
 **/
int execute_cmd(char **cmd_arr)
{ 
	extern char **environ;
	if (cmd_arr == NULL || cmd_arr[0] == NULL)
	{
		return (-1);
	}

	if (execve(cmd_arr[0], cmd_arr, environ) == -1)
	{
		return (-1);
	}

	free(cmd_arr);
	return (0);
}


/**
 * user_getline - get the user input
 *
 * Description: get the user input
 * Return: On success return the buf, on error return NULL
 **/
char *user_getline(void)
{
	char *buf;
	size_t bufsize;
	ssize_t getline_return;

	buf = NULL;
	bufsize = 0;

	getline_return = getline(&buf, &bufsize, stdin);
	if (getline_return == EOF)
	{
		free(buf);
		return (NULL);
	}
	return (buf);
}

/**
 * trim_whitespace - remove delimiters from the input
 * @str: user input
 *
 * Description - remove delimiters from the input
 * Return: trimed user input
 **/
char *trim_whitespace(char *str)
{
	char *endp;

	while (isspace((char) *str) != 0)
	{
		str = str + 1;
	}

	if (*str == '\0')
	{
		return (str);
	}

	endp = str + strlen(str) -1;
	while (endp > str && (isspace((char) *endp) != 0))
	{
		endp = endp - 1;
	}
	endp[1] = '\0';

	return (str);
}

int count_token(char *str, char *delim )
{
	char *tokenized_str;
	char *token;
	int i;

	tokenized_str = strdup(str);

	i = 0;
	token = strtok(tokenized_str, delim);
	while (token != NULL)
	{
		token = strtok(NULL, delim);
		i = i + 1;
	}

	free(tokenized_str);	
	return (i);
}

/**
 * get_input_arr - split the user input and make an array of strings
 * @str: user input
 * @delim: the delimiter
 *
 * Description - split the user input and make an array of strings
 * Return: an array of tokens
 **/
char **get_input_arr(char *str, char *delim)
{
	char **token_arr;
	int num_token;
	int i;
	char *token;

	num_token = count_token(str, delim);

	token_arr = malloc(sizeof(*token_arr) * (num_token + 1));
	if (token_arr == NULL)
	{
		free(token_arr);
		return (NULL);
	}
	
	i = 0;
	token = strtok(str, delim);
	while (token != NULL)
	{
		token_arr[i] = token;
		token = strtok(NULL, delim);
		i = i + 1;
	}
	token_arr[i] = NULL;

	return (token_arr);
}

/**
 * find_path - find the full path of a command
 * @cmd: the command that need to be searched
 *
 * Description - find the full path of a command
 * Return: on success, return command, on failure, return NULL
 **/
char *find_path(char* cmd)
{
  to do
}

/**
 * create_child - fork a child process and execute program
 * @cmd_arr: an array of commands
 *
 * Description - fork a child process and execute program
 * Return: nothing
 **/
void create_child(char **cmd_arr)
{
	pid_t pid;
	int status;
	int is_exist;
	struct stat st;
	char *cmd_tmp;
	char *cmd;

	pid = fork();
	if (pid == -1)
	{
		perror("Error: ");
		return;
	}

	if (pid == 0)
	{
		is_exist = stat(cmd_arr[0], &st);
		if (is_exist == 0)
            {
                execute_cmd(cmd_arr);
            }
            else
            {
                cmd = find_path(cmd_arr[0]);
                if (cmd == NULL)
                {
                    printf("error message");
                    return;
                }
                cmd_arr[0] = cmd;
                execute_cmd(cmd_arr);
            }
		}
		else
		{
			wait(&status);
		}
}	



/**
 * main - entry point
 *
 * Return: Always 0.
 */
int main(void)
{
	char *buf;
	char *trimed_buf;
	char **cmd_arr;
	
	while (1)
	{
		interactive_shell();

		buf = user_getline();
		if (buf == NULL)
		{
			return (0);
		}

		trimed_buf = trim_whitespace(buf);
		cmd_arr = get_input_arr(trimed_buf, " ");
		create_child(cmd_arr);

		free(buf);
	}

	return (0);
}
