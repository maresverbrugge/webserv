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

#include "Signal.hpp"

/*
This signal class is used to make sure (SIGINT) ctrl-C and (SIGQUIT) ctrl-\ 
are handled troughout the whole program, also in the epoll loop
*/

Signal::Signal()
{
	std::cout << "Signal constructor called" << std::endl;

	// to make sure signals are not handled by our process's signal handlers
	// during our signal handler with signalfd() is set-up:
	sigset_t mask; // create a set of signals, set is called mask
    sigemptyset(&mask); // empty the set of signals
    sigaddset(&mask, SIGQUIT); // add SIGQUIT to set of signals
    sigaddset(&mask, SIGINT); // add SIGINT to set of signals
	// now block the signals in set
    if (sigprocmask(SIG_BLOCK, &mask, NULL) < 0)
        throw_error("Error sigprocmask()", INTERNAL_SERVER_ERROR);
	
	// create new fd so we can receive signals specified in the set called mask
    _socketFD = signalfd(-1, &mask, 0);
    if (_socketFD < 0 )
        throw_error("Error signalfd()", INTERNAL_SERVER_ERROR);

	
}

Signal::~Signal()
{
	std::cout << "Signal destructor called" << std::endl;
}

void Signal::readSignal()
{
	// std::cout << "readSignal called" << std::endl;
	struct signalfd_siginfo fdsi
    {
    };

	// read from _socketFD
    ssize_t signal = read(_socketFD, &fdsi, sizeof(fdsi));
	// if what we read < 0, throw error
	// if what we read == SIGINT or == SIGQUIT
	// delete servers and clients
	// put serverIsRunning to false
	if (signal != sizeof(fdsi) || signal < 0)
        throw_error("Error read signal()", INTERNAL_SERVER_ERROR);
    if (fdsi.ssi_signo == SIGINT || fdsi.ssi_signo == SIGQUIT)
	{
		std::cout << "----------------------------" << std::endl;
		std::cout << "Signal received" << std::endl;
		std::cout << "----------------------------" << std::endl;
		g_serverIsRunning = false;
		return;
	}
	// if what we read == something else, output something like:
	std::cout << "Unhandeled signal received. Continuing the Wonderful Webserver...\n";
}