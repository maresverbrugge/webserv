/* ************************************************************************* */
/*      ##       ##      ## ##       ##      ## ##       ##      ##          */
/*       ##     ####    ##   ##     ####    ##   ##     ####    ##           */
/*        ##  ##   ##  ##     ##  ##   ##  ##     ##  ##   ##  ##            */
/*         ####     ####       ####     ####       ####     ####             */
/*          ##       ##         ##       ##         ##       ##              */
/*                                                                           */
/*           WONDERFUL            WEBSERV           WONDERTEAM               */
/*                                                                           */
/*      FELICIA KOOLHOVEN      FLEN HUISMAN       MARES VERBRUGGE            */
/*          fkoolhov             fhuisman             mverbrug               */
/*                                                                           */
/*          Codam Coding College        part of 42 network                   */
/*                            April - May 2024                               */
/* ************************************************************************* */

#include "RequestHandler.hpp"

static char* convert_string_to_char_pointer(std::string str)
{
	char char_array[str.size() + 1];
	std::strcpy(char_array, str.c_str());
	return char_array;
}

static void run_script()
{
    // dup2() to redirect stdout to pipe or to socket?
	// int write = pipe_fd[1]
	// dup2(write, STDOUT_FILENO);

    // get path, args, env for execve
    std::string script_string = "./root/cgi-bin/interior_design.py"; // get from request
    char* python_path = "/usr/bin/python3";
    char* python_script = convert_string_to_char_pointer(script_string);
    char *argv[] = { python_path, python_script, NULL };

    
	char* environment = convert_string_to_char_pointer(envp_string);
    char *envp[] = { environment, NULL };

    // execve to run the script
    execve(python_path, argv, envp);

	// if execve fails, throw error
	perror("execve failed");
	exit(EXIT_FAILURE);
}

static void fork_process()
{
	int pipe_fd[2];
	// fd[0] is read end of pipe.
	// fd[1] is write end of pipe.

	// create pipe to communicate between parent and child process?
	if (pipe(pipe_fd) == -1)
		throw_error("pipe() failed", INTERNAL_SERVER_ERROR);
	
	pid_t process_id = fork(); // create a new process where the script will be run

	if (process_id < 0)
	{
		throw_error("fork() failed", INTERNAL_SERVER_ERROR);
	}
	else if (process_id == CHILD_PID) // child process
	{
		run_script();
	}
	else // parent process
	{
		int child_exit_status;

		waitpid(process_id, &child_exit_status, 0);
		// close pipe?
		if (WIFEXITED(child_exit_status) && WEXITSTATUS(child_exit_status) != EXIT_SUCCESS) 
			throw_error("CGI script failed", INTERNAL_SERVER_ERROR);
	}
}

void RequestHandler::handleCGI()
{
	std::cout << "handleCGI called" << std::endl;

	int method = this->getRequest().getMethod();
	if (method == GET)
	{
		// epoll?
		fork_process();
		// verify that response is correct?
	}
	else if (method == POST)
	{
		std::vector<std::string> variables = this->getRequest().getQuery();
		// epoll?
		// get variables from the query string -> worden ENV variables in new proces (or ARGV?)
		fork_process();
		// verify that response is correct?
	}
	else
	{
		throw_error("Method not allowed in CGI request", METHOD_NOT_ALLOWED);
	}
}