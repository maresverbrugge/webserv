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

# include "configuration.hpp"
# include "Response.hpp"
# include "RequestHandler.hpp"
# include "ErrorHandler.hpp"
# include "configuration.hpp"
# include "Epoll.hpp"

std::atomic<bool> g_serverPoolIsRunning{true};

static void run_serverpool()
{
	try
	{
		Epoll& epoll_instance = Epoll::getInstance();
		epoll_instance.EpollWait();
	}
	catch (const std::exception& exception)
	{
		std::cout << RED BOLD "Fatal error: " RESET << exception.what() << std::endl;
		exit(EXIT_FAILURE);
	}
}

int main(int argc, char** argv)
{
	if (argc != 2)
		error_exit("Incorrect argument count. Usage: ./webserv [CONFIG]", EXIT_FAILURE);
	if (configure_serverpool(argv[1]) != EXIT_SUCCESS)
		error_exit("Error configuring serverpool", EXIT_FAILURE);
	run_serverpool();
	exit(EXIT_SUCCESS);
}
