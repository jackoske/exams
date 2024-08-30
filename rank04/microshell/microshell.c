#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

// Function to write an error message to stderr
void err(char *str)
{
	while (*str) {
		write(2, str++, 1);
	}
}

// Function to change the current working directory
int cd(char **argv, int i)
{
	if (i != 2) {
		err("error: cd: bad arguments\n");
		return 1;
	}
	if (chdir(argv[1]) == -1) {
		err("error: cd: cannot change directory to ");
		err(argv[1]);
		err("\n");
		return 1;
	}
	return 0;
}

// Function to set up the pipe
// end == 1 sets stdout to act as the write end of the pipe
// end == 0 sets stdin to act as the read end of the pipe
void set_pipe(int has_pipe, int *fd, int end)
{
	if (has_pipe && (dup2(fd[end], end) == -1 || close(fd[0]) == -1 || close(fd[1]) == -1)) {
		err("error: fatal\n");
		exit(1);
	}
}

// Function to execute a command
int exec(char **argv, int i, char **envp)
{
	int has_pipe;
	int fd[2];
	int pid;
	int status;

	has_pipe = argv[i] && !strcmp(argv[i], "|");

	// Handle 'cd' command separately
	if (!has_pipe && !strcmp(*argv, "cd")) {
		return cd(argv, i);
	}

	// Create a pipe if needed
	if (has_pipe && pipe(fd) == -1) {
		err("error: fatal\n");
		exit(1);
	}

	// Fork the process
	pid = fork();
	if (pid == -1) {
		err("error: fatal\n");
		exit(1);
	}

	// Child process
	if (pid == 0) {
		argv[i] = 0; // Null-terminate the arguments
		set_pipe(has_pipe, fd, 1); // Set up pipe for the child

		if (!strcmp(*argv, "cd")) {
			exit(cd(argv, i)); // If 'cd' is called in child, execute it
		}

		execve(*argv, argv, envp); // Execute the command
		err("error: cannot execute ");
		err(*argv);
		err("\n");
		exit(1);
	}

	// Parent process
	waitpid(pid, &status, 0); // Wait for the child process to finish
	set_pipe(has_pipe, fd, 0); // Set up pipe for the parent

	// Return child's exit status
	if (WIFEXITED(status)) {
		return WEXITSTATUS(status);
	} else {
		return 1;
	}
}

int main(int argc, char **argv, char **envp)
{
	int i = 0;
	int status = 0;

	// Loop through each command line argument
	while (argv[i]) {
		argv += i + 1;
		i = 0;

		// Loop until a pipe or semicolon is found
		while (argv[i] && strcmp(argv[i], "|") && strcmp(argv[i], ";")) {
			i++;
		}

		// If there are arguments, execute them
		if (i > 0) {
			status = exec(argv, i, envp);
		}
	}
	return status;
}
