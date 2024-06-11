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

Signal::Signal()
{
	std::cout << "Signal constructor called" << std::endl;

	sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGQUIT);
    sigaddset(&mask, SIGINT);
    if (sigprocmask(SIG_BLOCK, &mask, NULL) < 0)
        throw FatalException("Error sigprocmask()");
    _FD = signalfd(-1, &mask, 0);
    if (_FD < 0 )
		throw FatalException("Error signalfd()");
	if (set_fd_to_cloexec(_FD) != EXIT_SUCCESS)
    {
        close(_FD);
        throw FatalException("Error setting signal FD to close-on-exec");
    }
}

Signal::~Signal()
{
	std::cout << "Signal destructor called" << std::endl;
}

void Signal::readSignal()
{
	struct signalfd_siginfo fdsi {};

    ssize_t signal = read(_FD, &fdsi, sizeof(fdsi));
	if (signal != sizeof(fdsi) || signal <= 0)
        throw StatusCodeException("Error read signal()", INTERNAL_SERVER_ERROR);
    else if (fdsi.ssi_signo == SIGINT || fdsi.ssi_signo == SIGQUIT)
		g_serverPoolIsRunning = false;
	else
		std::cout << "Unhandeled signal received. Continuing the Wonderful Webserver..." << std::endl;
}