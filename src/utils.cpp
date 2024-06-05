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
    int flags = O_NONBLOCK;
    int result = fcntl(fd, F_SETFL, flags);
    if (result < 0)
    {
        perror("set_to_non_blocking failed");
        close(fd);
        exit(EXIT_FAILURE);
    }
    flags = FD_CLOEXEC;
    result = fcntl(fd, F_SETFD, flags);
    if (result < 0)
    {
        perror("set FD_CLOEXEC failed");
        close(fd);
        exit(EXIT_FAILURE);
    }
}

void set_to_cloexec(int fd)
{
    int flags = FD_CLOEXEC;
    int result = fcntl(fd, F_SETFD, flags);
    if (result < 0)
    {
        perror("set FD_CLOEXEC failed");
        close(fd);
        exit(EXIT_FAILURE);
    }
}