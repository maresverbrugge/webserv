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

#ifndef REQUESTHANDLER_HPP
# define REQUESTHANDLER_HPP

# include "webserv.hpp"
# include "Location.hpp"
# include "Request.hpp"

# include <vector>
# include <map>
# include <filesystem>
# include <unistd.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <cstring>
# include <string>
# include <fstream>

# define CHILD_PID 0

class Client;

class RequestHandler
{
	private:
        Request&                            _request;
		Client&		                        _client;
        const Location&                     _location;
        short                               _statusCode;
        std::map<std::string, std::string>  _headers;
        std::string                         _body;
        const std::string                   _absPath;
        const std::string                   _extension;
        bool                                _CGI;
		
	public:
		RequestHandler(Request& request, Client& client);
        RequestHandler() = delete;
		~RequestHandler();

		Request&                            getRequest() const;
		Client&                             getClient() const;
		const Location&                     getLocation() const;
		short                               getStatusCode() const;
        std::map<std::string, std::string>  getHeaders() const;
		std::string                         getBody() const;
        const std::string                   getAbsPath() const;
        const std::string                   getExtension() const;
        bool                                isCGI() const;

        void    setStatusCode(short statusCode);
        void    addHeader(std::string name, std::string value);
        void    setBody(std::string body);
        void    setCGI(bool cgi);

        const Location& matchLocation(std::string path);
        bool            methodIsAllowedOnLocation();
        std::string     findAbsolutePath();
        std::string     extractExtension();
        void            redirect();
        void            handleGetRequest();
        std::string     constructBody();
        std::string     constructBodyFromDirectory();
        void            handlePostRequest();
        void            handleDeleteRequest();
        void            handleCGI();
        void            fork_process();
};

std::string constructBodyFromFile(std::string pathToFile);
std::string getReasonPhrase(short statusCode);
std::string getContentType(std::string filename);

#endif