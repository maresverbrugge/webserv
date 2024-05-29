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

static void run_script(char** envp, int* write_end_of_pipe, std::string script_string)
{
	dup2(*write_end_of_pipe, STDOUT_FILENO); // add WRITE end to epoll! (MARES)

    const char* python_path = "/usr/bin/python3";
    const char* python_script = script_string.c_str();
	char *const argv[] = { const_cast<char *>(python_path), const_cast<char *>(python_script), NULL };

    execve(python_path, argv, envp);
	perror("execve failed");
	exit(EXIT_FAILURE);
}

static void delete_envp(char** envp)
{
	for (size_t i = 0; envp[i] != NULL; i++) 
		delete[] envp[i];
	delete[] envp;
}

static char** convert_to_envp(std::vector<std::string> variables)
{
	char **envp = new char*[variables.size() + 1];
	for (size_t i = 0; i < variables.size(); i++) 
	{
		envp[i] = new char[variables[i].size() + 1];
		std::strcpy(envp[i], variables[i].c_str());
	}
	envp[variables.size()] = NULL;
	return envp;
}

void RequestHandler::fork_process()
{
	char **envp = convert_to_envp(getRequest().getQuery());

	int pipe_fd[2];
	if (pipe(pipe_fd) == -1)
		throw_error("pipe() failed", INTERNAL_SERVER_ERROR);
	
	pid_t process_id = fork();

	if (process_id < 0)
		throw_error("fork() failed", INTERNAL_SERVER_ERROR);
	else if (process_id == CHILD_PID)
		run_script(envp, &pipe_fd[WRITE], _absPath);
	else
	{
		int read_end = pipe_fd[READ]; // add read end to epoll! (MARES)
		
		int child_exit_status;
		waitpid(process_id, &child_exit_status, 0);
	
		char buffer[10000]; // for debugging
		bzero(buffer, sizeof(buffer)); // for debugging
		read(read_end, buffer, sizeof(buffer)); // for debugging, read from pipe
		std::cout << buffer << std::endl; // for debugging, write to client

		delete_envp(envp);
		close(pipe_fd[READ]); // or remove from epoll?
		close(pipe_fd[WRITE]); // or remove from epoll?
		if (WIFEXITED(child_exit_status) && WEXITSTATUS(child_exit_status) != EXIT_SUCCESS) 
			throw_error("CGI script failed", INTERNAL_SERVER_ERROR);
	}
}

void RequestHandler::handleCGI()
{
	std::cout << "handleCGI called" << std::endl;

	int method = this->getRequest().getMethod();

	if (method == GET || method == POST)
		fork_process();
	else
		throw_error("Method not allowed in CGI request", METHOD_NOT_ALLOWED);
}