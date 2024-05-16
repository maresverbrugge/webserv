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

#ifndef Client_HPP
# define Client_HPP

# include "webserv.hpp"
# include "ASocket.hpp"
# include "Server.hpp"
# include <sys/socket.h> // ! needed? for accept()

enum e_readyFor
{
	READ,
	WRITE
};

class Client : public ASocket
{
	private:
		int		_readyFor; // FLAG
		// Epoll&	_epollReference;

	public:
		Client(const Server&);
		~Client();

		void	setReadyForFlag(int readyFor);
		int		getReadyForFlag() const;
		void	clientReceives();
};

std::ostream& operator<<(std::ostream& out_stream, const Client& Client);

#endif