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

// int g_state = 1;

// void siginthandler(int signum)
// {
//     if (signum == SIGINT)
//     {
//         g_state = 0;
//     }
// }

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		// handle error
		return (EXIT_FAILURE);
	}
	std::unique_ptr<ServerPool> serverpool = configure_serverpool(argv[1]);
	std::cout << *serverpool << std::endl; // for debugging purposes
	
	Epoll& epoll_instance = serverpool->getEpollInstance();
	std::cout << epoll_instance << std::endl; // for debugging purposes
	epoll_instance.EpollWait();
    close(epoll_instance.getSocketFD());
	return (EXIT_SUCCESS);
}
