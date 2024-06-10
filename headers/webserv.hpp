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

#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include <iostream>
# include <atomic>

# define RESET "\033[0m"
# define BOLD "\033[1m"
# define RED "\033[31m"
# define GREEN "\033[32m"
# define YELLOW "\033[33m"
# define BLUE "\033[34m"
# define PURPLE "\033[35m"
# define CYAN "\033[36m"
# define DEFAULT_CONFIG "./config/default.conf"

# define SUCCESS 0
# define ERROR -1

extern std::atomic<bool> g_serverPoolIsRunning;

enum e_status
{
	CONTINUE = 100,
	SWITCHING_PROTOCOLS = 101,
	OK = 200,
	CREATED = 201,
	ACCEPTED = 202,
	NON_AUTHORITATIVE_INFORMATION = 203,
	NO_CONTENT = 204,
	RESET_CONTENT = 205,
	PARTIAL_CONTENT = 206,
	MULTIPLE_CHOICES = 300,
	MOVED_PERMANENTLY = 301,
	FOUND = 302,
	SEE_OTHER = 303,
	NOT_MODIFIED = 304,
	USE_PROXY = 305,
	TEMPORARY_REDIRECT = 307,
	BAD_REQUEST = 400,
	UNAUTHORIZED = 401,
	PAYMENT_REQUIRED = 402,
	FORBIDDEN = 403,
	NOT_FOUND = 404,
	METHOD_NOT_ALLOWED = 405,
	NOT_ACCEPTABLE = 406,
	PROXY_AUTHENTICATION_REQUIRED = 407,
	REQUEST_TIMEOUT = 408,
	CONFLICT = 409,
	GONE = 410,
	LENGTH_REQUIRED = 411,
	PRECONDITION_FAILED = 412,
	CONTENT_TOO_LARGE = 413,
	URI_TOO_LARGE = 414,
	UNSUPPORTED_MEDIA_TYPE = 415,
	REQUESTED_RANGE_NOT_SATISFIABLE = 416,
	EXPECTATION_FAILED = 417,
	INTERNAL_SERVER_ERROR = 500,
	NOT_IMPLEMENTED = 501,
	BAD_GATEWAY = 502,
	SERVICE_UNAVAILABLE = 503,
	GATEWAY_TIMEOUT = 504,
	HTTP_VERSION_NOT_SUPPORTED = 505
};

enum e_methods
{
	GET,
	POST,
	DELETE
};

enum e_readyFor
{
	READ,
	WRITE
};

// generalUtils.cpp
# include <exception>
# include <iostream>
# include <string>
# include <fcntl.h>
# include <unistd.h>

class StatusCodeException : public std::exception
{
	public:
		StatusCodeException(const std::string& message, const e_status& status_code);
		const char* what() const noexcept override;
		const e_status& status() const noexcept;

	private:
		std::string _message;
		e_status _status_code;
};

class FatalException : public std::exception 
{
	public:
		explicit FatalException(const std::string& message);
		virtual const char* what() const noexcept override;

	private:
		std::string _message;
};

void error_exit(std::string message, int status_code);
void set_to_non_blocking(int fd);
void set_to_cloexec(int fd);
void set_fd_to_non_blocking_and_cloexec(int fd);

#endif