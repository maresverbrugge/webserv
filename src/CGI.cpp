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

#include "CGI.hpp"
#include "Epoll.hpp"
#include "Client.hpp"
#include "Server.hpp"

CGI::CGI(int read_end, Client& client) : _client(client)
{
	std::cout << "CGI constructor called" << std::endl;
	_socketFD = read_end;
	if (Epoll::getInstance().addFDToEpoll(this, EPOLLIN, _socketFD) < 0)
	{
		close(_socketFD);
		throw StatusCodeException("Error adding CGI pipe read-FD to epoll", INTERNAL_SERVER_ERROR);
	}
}

CGI::CGI(int write_end, Client& client, char **envp, std::string script_string) : _client(client), _envp(envp), _script_string(script_string)
{
	std::cout << "CGI constructor called" << std::endl;
	_socketFD = write_end;
	if (Epoll::getInstance().addFDToEpoll(this, EPOLLOUT, _socketFD) < 0)
	{
		close(_socketFD);
		throw FatalException("Error adding CGI pipe write-FD to epoll");
	}
}

CGI::~CGI()
{
	std::cerr << "CGI destructor called" << std::endl;
	// close(_socketFD);
}

Client& CGI::getClient() const
{
	return (_client);
}

char** CGI::getEnvp() const
{
	return (_envp);
}

const std::string CGI::getScriptString() const
{
	return (_script_string);
}

void CGI::setEnvp(char **envp)
{
	_envp = envp;
}

void CGI::run_script()
{
	dup2(_socketFD, STDOUT_FILENO);

    const char* python_path = "/usr/bin/python3";
    const char* python_script = _script_string.c_str();
	char *const argv[] = { const_cast<char *>(python_path), const_cast<char *>(python_script), NULL };

    execve(python_path, argv, _envp);
	perror("execve failed");
	exit(EXIT_FAILURE);
}

int CGI::readFromPipe()
{
	char response[BUFSIZ]{};
	ssize_t bytes_read{};

	bytes_read = read(_socketFD, response, BUFSIZ - 1);
	if (bytes_read <= 0)
		return (ERROR);
	_client.setResponse(response);
	_client.setReadyForFlag(WRITE);
	return (SUCCESS);
}
