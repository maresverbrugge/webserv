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

# include "Signal.hpp"

Signal::Signal()
{
	std::cout << "Signal constructor called" << std::endl;

	sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGTERM);
    sigaddset(&mask, SIGINT);

    if (sigprocmask(SIG_BLOCK, &mask, NULL) < 0)
        throw_error("Error sigprocmask()", INTERNAL_SERVER_ERROR);
    _socketFD = signalfd(-1, &mask, 0);
    if (_socketFD < 0 )
        throw_error("Error signalfd()", INTERNAL_SERVER_ERROR);
}

Signal::~Signal()
{
	std::cout << "Signal destructor called" << std::endl;
	close(_socketFD); // close Signal socket
}

void Signal::readSignal()
{
	std::cout << "readSignal called" << std::endl;
	// read from _socketFD
	struct signalfd_siginfo fdsi
    {
    };

    ssize_t signal = read(_socketFD, &fdsi, sizeof(fdsi));
	// if what we read < 0, throw error
	// if what we read == SIGINT or == SIGQUIT
	// delete servers and clients
	// put serverIsRunning to false
	 if (signal != sizeof(fdsi))
        throw_error("Error read signal()", INTERNAL_SERVER_ERROR);
    if (fdsi.ssi_signo == SIGINT || fdsi.ssi_signo == SIGQUIT)
	{
		std::cout << "----------------------------" << std::endl;
		std::cout << "Signal received" << std::endl;
		std::cout << "----------------------------" << std::endl;
		g_serverIsRunning = false;
	}
	// if what we read == something else, output something like:
	// std::cout << "Unhandeled signal received. Continuing the Wonderful Webserver...\n";
}