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

// Define the maximum number of events to be returned from epoll_wait()
# define MAX_EVENTS 10

class Server;
// class Client;

class Epoll : public ASocket
{
	private:
		// struct epoll_event _event{};
	public:
		Epoll();
		~Epoll();

		// int addFDToEpoll(int event_to_poll_for, int fdToAdd);
		// ! OR:
		int addFDToEpoll(ASocket *ptr, int event_to_poll_for, int fdToAdd);

		int delFDFromEpoll(int event_to_poll_for, int fdToDel);
		// ! OR:
		// int delFDFromEpoll(ASocket *ptr, int event_to_poll_for, int fdToAdd)

		int modFDInEpoll(int event_to_poll_for, int fdToMod);
		// ! OR:
		// int modFDInEpoll(ASocket *ptr, int event_to_poll_for, int fdToMod)

		void EpollWait();
};

std::ostream& operator<<(std::ostream& out_stream, const Epoll& Epoll);

#endif
