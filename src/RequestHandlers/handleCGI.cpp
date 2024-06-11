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
	std::vector<std::string> body_split = getRequest().splitQueryString(body_as_string);
	char **envp = convert_to_envp(body_split);

	int pipe_fd[2];
	if (pipe(pipe_fd) == -1)
	{
		delete_envp(envp);
		throw StatusCodeException("pipe() failed", INTERNAL_SERVER_ERROR);
	}
	if (_client.setFlags(pipe_fd[READ]) != EXIT_SUCCESS || _client.setFlags(pipe_fd[WRITE]) != EXIT_SUCCESS)
	{
		delete_envp(envp);
		throw StatusCodeException("Error setting flags for pipe", INTERNAL_SERVER_ERROR);
	}
	
	pid_t process_id = fork();

	if (process_id < 0)
	{
		delete_envp(envp);
		throw StatusCodeException("fork() failed", INTERNAL_SERVER_ERROR);
	}
	else if (process_id == CHILD_PID)
	{
		Epoll::getInstance().setIsChildProcess(true);
		_client.newWriteCGI(pipe_fd[WRITE], envp, _absPath, _extension);
	}
	else
	{
		delete_envp(envp);
		close(pipe_fd[WRITE]);
		int child_exit_status;
		waitpid(process_id, &child_exit_status, 0);
		if (WIFEXITED(child_exit_status) && WEXITSTATUS(child_exit_status) != EXIT_SUCCESS)
			throw StatusCodeException("CGI script failed", INTERNAL_SERVER_ERROR);
		_client.newReadCGI(pipe_fd[READ]);
	}
}

void RequestHandler::handleCGI()
{
	int method = this->getRequest().getMethod();

	if (method == GET || method == POST)
		fork_process();
	else
		throw StatusCodeException("Method not allowed in CGI request", METHOD_NOT_ALLOWED);
}