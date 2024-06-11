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
		bool							_isChildProcess;
		Signal							_signal;
		
		Epoll();
	
	public:
		Epoll(const Epoll&) = delete;
        Epoll& operator=(const Epoll&) = delete;
		~Epoll();

		static Epoll& 	getInstance();
		bool			isChild() const;
		const Signal&	getSignal() const;

		void 	setIsChildProcess(bool isChild);

		int		handleInEvents(AFileDescriptor* ptr);
		void 	handleOutEvents(AFileDescriptor* ptr);

		int 	addFDToEpoll(AFileDescriptor *ptr, int event_to_poll_for, int fdToAdd);
		void 	EpollLoop();
		void 	runScript(CGI* cgi);
};

std::ostream& operator<<(std::ostream& out_stream, const Epoll& Epoll);

#endif
