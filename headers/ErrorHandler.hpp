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

#ifndef ERRORHANDLER_HPP
# define ERRORHANDLER_HPP

# include "Server.hpp"

class ErrorHandler
{
	private:
        Server&                             _server;
        const short                         _statusCode;
        std::map<std::string, std::string>  _headers;
        std::string                         _body;
		
	public:
        ErrorHandler(short statusCode, Server& server);
        ErrorHandler() = delete;
		~ErrorHandler();

        Server&                             getServer() const;
		short                               getStatusCode() const;
        std::map<std::string, std::string>  getHeaders() const;
		std::string                         getBody() const;

        void    addHeader(std::string name, std::string value);
        void    setBody(std::string body);

        std::string    constructErrorPage();
        std::string    constructBody(short statusCode);
};

std::string getReasonPhrase(short statusCode);
std::string constructBodyFromFile(std::string pathToFile);

#endif