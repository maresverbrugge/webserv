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

#include "AFileDescriptor.hpp"

AFileDescriptor::~AFileDescriptor()
{
	close(_FD);
}

int AFileDescriptor::getFD() const
{
	return _FD;
}

int AFileDescriptor::setToCloseOnExec(int fd)
{
	int fd_flags = fcntl(fd, F_GETFD);
	if (fd_flags < 0)
	{
		perror("get fd_flags failed");
		return EXIT_FAILURE;
	}
	fd_flags |= FD_CLOEXEC;
	int result = fcntl(fd, F_SETFD, fd_flags);
	if (result < 0)
	{
		perror("set FD_CLOEXEC failed");
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

int AFileDescriptor::setToNonBlocking(int fd)
{
	int status_flags = fcntl(fd, F_GETFL);
	if (status_flags < 0)
	{
		perror("get status_flags failed");
		return EXIT_FAILURE;
	}
	status_flags |= O_NONBLOCK;
	int result = fcntl(fd, F_SETFL, status_flags);
	if (result < 0)
	{
		perror("set O_NONBLOCK failed");
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

int AFileDescriptor::setFlags(int fd)
{
	if (setToNonBlocking(fd) != EXIT_SUCCESS)
		return EXIT_FAILURE;
	if (setToCloseOnExec(fd) != EXIT_SUCCESS)
		return EXIT_FAILURE;
	return EXIT_SUCCESS;
}
