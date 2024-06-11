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
	_FD = read_end;
	if (Epoll::getInstance().addFDToEpoll(this, EPOLLIN, _FD) < 0)
	{
		close(_FD);
		throw StatusCodeException("Error adding CGI pipe read-FD to epoll", INTERNAL_SERVER_ERROR);
	}
}

CGI::CGI(int write_end, Client& client, char **envp, std::string script_string, std::string extension) : _client(client), _envp(envp), _script_string(script_string), _extension(extension)
{
	std::cout << "CGI constructor called" << std::endl;
	_FD = write_end;
	if (Epoll::getInstance().addFDToEpoll(this, EPOLLOUT, _FD) < 0)
	{
		close(_FD);
		throw FatalException("Error adding CGI pipe write-FD to epoll");
	}
}

CGI::~CGI()
{
	std::cerr << "CGI destructor called" << std::endl;
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

const std::string CGI::getExtension() const
{
	return (_extension);
}

void CGI::setEnvp(char **envp)
{
	_envp = envp;
}

int CGI::readFromPipe()
{
	char response[BUFSIZ]{};
	ssize_t bytes_read{};

	bytes_read = read(_FD, response, BUFSIZ - 1);
	if (bytes_read <= 0)
		return (ERROR);
	_client.setResponse(response);
	_client.setReadyForFlag(WRITE);
	return (SUCCESS);
}
