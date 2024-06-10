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

void error_exit(std::string message, int status_code)
{
	std::cerr << BOLD RED "Fatal error: " RESET << message << std::endl;
	exit(status_code);
}

void set_to_cloexec(int fd)
{
	int fd_flags = fcntl(fd, F_GETFD);
	if (fd_flags < 0)
	{
		perror("get fd_flags failed");
		close(fd);
		exit(EXIT_FAILURE);
	}
	fd_flags |= FD_CLOEXEC;
	int result = fcntl(fd, F_SETFD, fd_flags);
	if (result < 0)
	{
		perror("set FD_CLOEXEC failed");
		close(fd);
		exit(EXIT_FAILURE);
	}
}

void set_to_non_blocking(int fd)
{
	int status_flags = fcntl(fd, F_GETFL);
	if (status_flags < 0)
	{
		perror("get status_flags failed");
		close(fd);
		exit(EXIT_FAILURE);
	}
	status_flags |= O_NONBLOCK;
	int result = fcntl(fd, F_SETFL, status_flags);
	if (result < 0)
	{
		perror("set O_NONBLOCK failed");
		close(fd);
		exit(EXIT_FAILURE);
	}
}

void set_fd_to_non_blocking_and_cloexec(int fd)
{
	set_to_non_blocking(fd);
	set_to_cloexec(fd);
}

// A StatusCodeException is thrown when a status code is not 200 OK.
StatusCodeException::StatusCodeException(const std::string& message, const e_status& status_code)
	: _message(message), _status_code(status_code) {}

const char* StatusCodeException::what() const noexcept
{
	return _message.c_str();
}

const e_status& StatusCodeException::status() const noexcept
{
	return _status_code;
}

// A FatalException is thrown when a fatal error occurs.
FatalException::FatalException(const std::string& message) 
	: _message(message) {}

const char* FatalException::what() const noexcept 
{
	return _message.c_str();
}