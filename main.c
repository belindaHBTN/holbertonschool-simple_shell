#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

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
int execute_cmd(char *cmd)
{ 
	char *user_input[] = {cmd, NULL};

	if (execve(user_input[0], user_input, NULL) == -1)
	{
		perror("Error: ");
		return (-1);
	}

	return (0);
}


/**
 * main - fork example
 *
 * Return: Always 0.
 */
int main(void)
{
	char *buf;
	size_t bufsize;
	ssize_t getline_return;
	pid_t pid;
	int status;

	buf = NULL;
	bufsize = 0;
	
	while (1)
	{
		interactive_shell();

		getline_return = getline(&buf, &bufsize, stdin);
		if (getline_return == EOF)
		{
			printf("fail to get the input\n");
			free(buf);
			return (1);
		}
		buf = strtok(buf, "\n");

		pid = fork();
		if (pid == -1)
		{
			perror("Error: ");
			return (1);
		}

		if (pid == 0)
		{
			execute_cmd(buf);
		}
		else
		{
			wait(&status);
		}
	}

	free(buf);
	return (0);
}
