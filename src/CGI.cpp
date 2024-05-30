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

# include "Epoll.hpp"
# include "CGI.hpp"
# include "ServerPool.hpp"

CGI::CGI(int flagReadWrite, int pipe_fd, const Client& client) : _client(client)
{
	_socketFD = pipe_fd;
	int event_to_poll_for = EPOLLIN;
	if (flagReadWrite == WRITE)
		event_to_poll_for = EPOLLOUT;
	if (client.getServer().getEpollReference().addFDToEpoll(this, event_to_poll_for, _socketFD) < 0)
	{
		close(_socketFD); // close CGI socket
		throw std::runtime_error("Error adding fd to epoll");
	}
	std::cout << "CGI constructor called" << std::endl;
}

CGI::~CGI()
{
	close(_socketFD); // close CGI socket
	std::cout << "CGI destructor called" << std::endl;
}

void CGI::cgiReads()
{
	char buffer[BUFSIZ]{};
	ssize_t bytes_received{};

	bytes_received = recv(_socketFD, buffer, BUFSIZ - 1, 0);
	_response = buffer;
}