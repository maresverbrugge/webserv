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
# include "ASocket.hpp"
# include "Server.hpp"
# include "Response.hpp"
# include "Request.hpp"
# include "RequestHandler.hpp"
# include "webserv.hpp"

# include <sys/socket.h> // ! needed? for accept()
# include <chrono>

# define TIMEOUT 10 // seconds

enum e_readyFor
{
	READ,
	WRITE
};

class Server;
class Request;
class Response;

class Client : public ASocket
{
	private:
		Server&						_server;
		int							_readyFor; // FLAG
		std::unique_ptr<Request>	_request;
		std::string					_response{};
		std::string					_fullBuffer{};
		
		std::chrono::time_point<std::chrono::steady_clock> _startTime;

	public:
		Client(Server& server);
		~Client();

		int				getReadyForFlag() const;
		std::string		getResponse() const;
		Server&			getServer() const;
	
		void	setReadyForFlag(int readyFor);
		void	setResponse(char *buffer);

		int		clientReceives();
		void	clientWrites();

		bool	requestHasTimedOut();
		bool	headersComplete();
		bool	requestIsComplete();
};

std::ostream& operator<<(std::ostream& out_stream, const Client& Client);

#endif
