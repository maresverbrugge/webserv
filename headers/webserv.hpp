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
# include <cstdlib>
# include <vector>
# include <map>
# include <string>
# include <array>
# include <stack>
# include <unordered_set>
# include <fstream>
# include <sstream>
# include <memory>
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

extern std::atomic<bool> g_serverIsRunning;

enum e_status
{
	OK = 200,
	CREATED = 201,
	NO_CONTENT = 204,
	FOUND = 302,
	BAD_REQUEST = 400,
	UNAUTHORIZED = 401,
	FORBIDDEN = 403,
	NOT_FOUND = 404,
	METHOD_NOT_ALLOWED = 405,
	REQUEST_TIMEOUT = 408,
	CONFLICT = 409,
	LENGTH_REQUIRED = 411,
	PAYLOAD_TOO_LARGE = 413, // ! kiezen!
	REQUEST_TOO_LARGE = 413,
	URI_TOO_LARGE = 414,
	INTERNAL_SERVER_ERROR = 500,
	NOT_IMPLEMENTED = 501,
	HTTP_VERSION_NOT_SUPPORTED = 505
};

enum e_methods
{
	GET,
	POST,
	DELETE
};

void throw_error(std::string message, const e_status& status_code);
void error_exit(std::string message, int status_code);
void sigIntHandler(int signum);

#endif