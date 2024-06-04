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

#ifndef EPOLL_HPP
# define EPOLL_HPP

# include "ASocket.hpp"
# include <sys/epoll.h> // for epoll
# include <unistd.h> // for close

# include "Server.hpp"
# include "Client.hpp"
# include "CGI.hpp"
# include "Signal.hpp"

// Define the maximum number of events to be returned from epoll_wait()
# define MAX_EVENTS 10

class Server;
class CGI;
// class Client;

class Epoll : public ASocket
{
	private:
		static std::unique_ptr<Epoll>	_instance;
		bool							_isChildProcess;
		Signal							_signal;
		
		Epoll();
	
	public:
		Epoll(const Epoll&) = delete;
        Epoll& operator=(const Epoll&) = delete;
		static Epoll& getInstance();
		~Epoll();

		int 	addFDToEpoll(ASocket *ptr, int event_to_poll_for, int fdToAdd);
		void 	EpollWait();
		void 	setIsChildProcess(bool isChild);
		void 	runScript(CGI* cgi, epoll_event* event);
};

std::ostream& operator<<(std::ostream& out_stream, const Epoll& Epoll);

#endif
