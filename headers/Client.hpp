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

# include "Epoll.hpp"
# include "webserv.hpp"
# include "ASocket.hpp"
# include "Server.hpp"
# include <sys/socket.h> // ! needed? for accept()
# include "Response.hpp"
# include "Request.hpp"
# include "RequestHandler.hpp"

# define NOT_INITIALIZED -1

enum e_readyFor
{
	READ,
	WRITE
};

class Server;
class Response;

class Client : public ASocket
{
	private:
		const Server&				_server;
		int							_readyFor; // FLAG
		std::unique_ptr<Response>	_response;
		std::string					_fullBuffer;
		long long					_contentLength;

	public:
		Client(const Server&);
		~Client();

		int		getReadyForFlag() const;
		void	setReadyForFlag(int readyFor);

		void	clientReceives();
		void	clientWrites();

		bool	requestIsComplete();
};

std::ostream& operator<<(std::ostream& out_stream, const Client& Client);

#endif
