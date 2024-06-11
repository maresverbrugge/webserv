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
# include "AFileDescriptor.hpp"
# include "Signal.hpp"

# include <memory>
# include <sys/epoll.h>
# include <unistd.h>

class CGI;

class Epoll : public AFileDescriptor
{
	private:
		static std::unique_ptr<Epoll>	_instance;
		Signal							_signal;
		bool							_isChildProcess;
		
		Epoll();
	
	public:
		Epoll(const Epoll&) = delete;
        Epoll& operator=(const Epoll&) = delete;
		~Epoll();

		void 	setIsChildProcess(bool isChild);

		static Epoll& 	getInstance();
		const Signal&	getSignal() const;
		bool			isChild() const;

		int 	addFDToEpoll(AFileDescriptor *ptr, int event_to_poll_for, int fdToAdd);
		void 	runScript(CGI* cgi);
		int		handleInEvents(AFileDescriptor* ptr);
		void 	handleOutEvents(AFileDescriptor* ptr);
		void 	EpollLoop();
};

std::ostream& operator<<(std::ostream& out_stream, const Epoll& Epoll);

#endif
