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

#include "webserv.hpp"
#include "configuration.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "RequestHandler.hpp"
#include "ErrorHandler.hpp"
# include "webserv.hpp"
# include "configuration.hpp"
# include "Request.hpp"
# include "Response.hpp"
# include "Epoll.hpp"
# include "csignal"

std::atomic<bool> g_serverIsRunning{true};

void sigIntHandler(int signal)
{
	std::cout << "SIGINT handler called" << std::endl;
	if (signal == SIGINT)
		g_serverIsRunning = false;
}

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		// handle error
		return (EXIT_FAILURE);
	}
	ServerPool& serverpool = configure_serverpool(argv[1]);
	std::cout << serverpool << std::endl; // for debugging purposes
	
	Epoll& epoll_instance = serverpool.getEpollInstance();
	// std::cout << epoll_instance << std::endl; // for debugging purposes

	std::signal(SIGINT, sigIntHandler);
	epoll_instance.EpollWait();
    close(epoll_instance.getSocketFD());

	// will serverpool get deleted automatically as it is a unique pointer?

	return (EXIT_SUCCESS);
}
