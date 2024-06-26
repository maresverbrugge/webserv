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

#ifndef CLIENT_HPP
# define CLIENT_HPP

# define TIMEOUT 10

# include "webserv.hpp"
# include "AFileDescriptor.hpp"
# include "Request.hpp"
# include "CGI.hpp"

# include <memory>
# include <sys/socket.h>
# include <chrono>

class Server;

class Client : public AFileDescriptor
{
	private:
		Server&						_server;
		int							_readyFor;
		std::unique_ptr<Request>	_request;
		std::string					_response{};
		std::string					_fullBuffer{};
		std::unique_ptr<CGI>		_cgi;

		bool												_timerStarted;
		std::chrono::time_point<std::chrono::steady_clock> 	_startTime;

	public:
		Client(Server& server);
		~Client();

		void	setReadyForFlag(int readyFor);
		void	setRequest(Request *request) = delete;
		void	setResponse(char *buffer);
		void	setFullBuffer(std::string full_buffer) = delete;
		void	setCGI(CGI *cgi) = delete;

		Server&			getServer() const;
		int				getReadyForFlag() const;
		Request&		getRequest() const;
		std::string		getResponse() const;
		std::string		getFullBuffer() const;
		CGI&			getCGI() const;

		void	newReadCGI(int read_end);
		void	newWriteCGI(int write_end, char** envp, std::string script_string, std::string extension);
		void	deleteCGI();

		int		receiveFromClient();
		void	writeToClient();

		bool	requestHasTimedOut();
		bool	headersComplete();
		bool	requestIsComplete();
};

#endif
