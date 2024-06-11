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

static void fatal_error(const std::string& message)
{
	std::cerr << RED BOLD "Fatal error: " RESET << message << std::endl;
}

static int run_serverpool()
{
	try
	{
		Epoll& epoll_instance = Epoll::getInstance();
		epoll_instance.EpollLoop();
		return EXIT_SUCCESS;
	}
	catch (const std::exception& exception)
	{
		fatal_error(exception.what());
		return EXIT_FAILURE;
	}
}

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		fatal_error("Incorrect argument count. Usage: ./webserv [CONFIG]");
		return EXIT_FAILURE;
	}
	std::string config_file = argv[1];
	if (configure_serverpool(config_file) != EXIT_SUCCESS)
	{
		fatal_error("Error configuring serverpool");
		return EXIT_FAILURE;
	}
	int status = run_serverpool();
	return status;
}
