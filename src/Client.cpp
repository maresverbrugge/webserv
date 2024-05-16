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

# include "Client.hpp"
# include "ServerPool.hpp"
# include "Epoll.hpp"

Client::Client(const Server& server) : _readyFor(READ)
{
	std::cout << "Client constructor called" << std::endl;
	if ((_socketFD = accept(server.getSocketFD(), server.getServerInfo()->ai_addr, &server.getServerInfo()->ai_addrlen)) < 0)
		std::cout << "Error: failed to accept new connection (Client class constructor) with accept()" << std::endl;
	std::cout << "_readyFor flag in constructor = " << _readyFor << std::endl; //! for testing
	// give reference of Server to constructor of Client so we access Epoll instance through reference
	if (server.getEpollReference().addFDToEpoll(this, EPOLLIN, _socketFD) < 0)
	{
		close(_socketFD); // close server socket
		throw std::runtime_error("Error adding fd to epoll");
	}
}

Client::~Client()
{
	close(_socketFD); // close client socket
	std::cout << "Client destructor called" << std::endl;
}

void Client::setReadyForFlag(int readyFor)
{
	_readyFor = readyFor;
}

int Client::getReadyForFlag() const
{
	return _readyFor;
}

/*
Recv() is use to receive data from a socket
*/
void Client::clientReceives()
{
	char buffer[BUFSIZ]{}; // buffer to hold client data, BUFSIZ = 1024
	ssize_t recv_return{};

	recv_return = recv(_socketFD, buffer, BUFSIZ - 1, 0);

	// TO TEST:
	std::cout << "reading from socket. bytes read: " << recv_return << std::endl;
    buffer[recv_return] = '\0'; // it this necessary to do ourselves?
    std::cout << buffer << std::endl;
	// END OF TEST

	// TODO: add check for:
	// if (recv_return <= 0)
	// remove client from epoll!

	// TODO:
	// call parse request
	// call process request
	// if no errors: change flag to WRITE
}