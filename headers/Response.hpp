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

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "webserv.hpp"

# include <map>
# include <stdlib.h>
# include <iomanip>

class ErrorHandler;
class RequestHandler;
class Server;

class Response
{
	private:
		const Server&						_server;
		short								_statusCode;
		std::string							_reasonPhrase;
		std::string							_statusLine;
		std::map<std::string, std::string>	_responseHeaders;
		std::string							_body;
		std::string							_responseMessage;

	public:
		Response(RequestHandler& requestHandler);
		Response(ErrorHandler& errorHandler);
		~Response();

		void	setStatusCode(short statusCode);
		void	setReasonPhrase(short statusCode);
		void	setStatusLine(std::string statusLine);
		void	addResponseHeader(std::string name, std::string value);
		void	setBody(std::string body);
		void	setResponseMessage(std::string responseMessage);
		
		const Server&							getServer() const;
		short									getStatusCode() const;
		std::string								getStatusLine() const;
		std::map<std::string, std::string>		getResponseHeaders() const;
		std::string								getBody() const;
		std::string								getResponseMessage() const;

		std::string							constructStatusLine();
		std::string							constructResponseMessage();
		void								addHeaders();
		std::string							PercentEncoding(std::string string);
};

std::ostream&	operator<<(std::ostream& out_stream, const Response& response);
std::string 	getContentType(std::string filename);

#endif