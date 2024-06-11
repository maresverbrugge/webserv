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