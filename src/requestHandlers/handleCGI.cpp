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
#include "Epoll.hpp"
#include "Client.hpp"

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
	std::vector<char> body = getRequest().getBody();
	std::string body_as_string(body.begin(), body.end());
	std::vector<std::string> body_split = getRequest().splitQueryString(body_as_string); // ! good idea?? using the query parser for the body?
	char **envp = convert_to_envp(body_split);
	// TO TEST:
	// for (unsigned long i = 0; i < body_split.size(); i++)
	// {
	// 	std::cout << body_split[i] << std::endl;
	// }

	int pipe_fd[2];
	if (pipe(pipe_fd) == -1)
		throw_error("pipe() failed", INTERNAL_SERVER_ERROR);
	set_to_non_blocking(pipe_fd[READ]);
	set_to_non_blocking(pipe_fd[WRITE]);
	
	pid_t process_id = fork();

	if (process_id < 0)
		throw_error("fork() failed", INTERNAL_SERVER_ERROR);
	else if (process_id == CHILD_PID)
	{
		Epoll::getInstance().setIsChildProcess(true);
		_client.newCGI(pipe_fd[WRITE], envp, _absPath);
	}
	else
	{
		delete_envp(envp);
		close(pipe_fd[WRITE]);
		_client.newCGI(pipe_fd[READ]);
		int child_exit_status;
		waitpid(process_id, &child_exit_status, 0);
		if (WIFEXITED(child_exit_status) && WEXITSTATUS(child_exit_status) != EXIT_SUCCESS)
			throw_error("CGI script failed", INTERNAL_SERVER_ERROR);
	}
}

void RequestHandler::handleCGI()
{
	// std::cout << "handleCGI called" << std::endl;

	int method = this->getRequest().getMethod();

	if (method == GET || method == POST)
		fork_process();
	else
		throw_error("Method not allowed in CGI request", METHOD_NOT_ALLOWED);
}