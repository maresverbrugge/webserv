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

# include "ASocket.hpp"
# include "webserv.hpp"
# include <sys/socket.h> // ! needed? for accept()

#include "Server.hpp"

class Epoll;

enum e_readyFor
{
	READ,
	WRITE
};

class Client : public ASocket
{
	private:
		int _readyFor; // FLAG

	public:
		Client(const Server&);
		~Client();
};

std::ostream& operator<<(std::ostream& out_stream, const Client& Client);

#endif