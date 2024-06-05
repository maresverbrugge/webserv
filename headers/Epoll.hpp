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

// Define the maximum number of events to be returned from epoll_wait()
# define MAX_EVENTS 10

# include "webserv.hpp"
# include "ASocket.hpp"
# include "Signal.hpp"

# include <memory> // for unique_ptr
# include <sys/epoll.h> // for epoll
# include <unistd.h> // for close

class CGI;

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

		void	handleInEvents(ASocket* ptr);
		void 	handleOutEvents(ASocket* ptr);

		int 	addFDToEpoll(ASocket *ptr, int event_to_poll_for, int fdToAdd);
		void 	EpollWait();
		void 	setIsChildProcess(bool isChild);
		void 	runScript(CGI* cgi);
};

std::ostream& operator<<(std::ostream& out_stream, const Epoll& Epoll);

#endif
