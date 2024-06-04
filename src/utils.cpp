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

void throw_error(std::string message, const e_status& status_code)
{
	std::cerr << BOLD RED "Error: " RESET << message << std::endl;
	throw (status_code);
}

void error_exit(std::string message, int status_code)
{
	std::cerr << BOLD RED "Error: " RESET << message << std::endl;
	exit(status_code);
}

void set_to_non_blocking(int fd)
{
    int result = fcntl(fd, F_SETFL, O_NONBLOCK);
    if (result < 0)
    {
        perror("set_to_non_blocking failed");
        close(fd);
        exit(EXIT_FAILURE);
    }
}