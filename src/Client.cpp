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
	// give reference of serverPool to constructor of Client so we can access EpollInstance
	Epoll& EpollInstance = server.getServerPool().getEpollInstance();
	if (EpollInstance.addFDToEpoll(this, EPOLLIN, _socketFD) < 0)
	{
		close(_socketFD); // close server socket
		throw std::runtime_error("Error adding fd to epoll");
	}
}

Client::~Client()
{
	// close(this->_ClientSocket); // ?
	std::cout << "Client destructor called" << std::endl;
}

// std::ostream& operator<<(std::ostream& out_stream, const Client& Client)
// {
// 	// out_stream << GREEN BOLD "\nClient: " RESET << Client.getHost() << ":" << Client.getPort() << std::endl;
// 	return out_stream;
// }