/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   RequestHandler.hpp                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: fhuisman <fhuisman@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/09 16:19:30 by fhuisman      #+#    #+#                 */
/*   Updated: 2024/05/10 22:13:51 by fhuisman      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUESTHANDLER_HPP
# define REQUESTHANDLER_HPP

# include "webserv.hpp"
# include "Request.hpp"
# include "Server.hpp"

class RequestHandler
{
	private:
        Request&                            _request;
		Server&		                        _server;
        Location&                           _location;
        short                               _statusCode;
        std::map<std::string, std::string>  _headers;
        std::string                         _body;
		
	public:
		RequestHandler(Request& request, Server& server);
        RequestHandler() = delete;
		~RequestHandler();

		Request&                            getRequest() const;
		Server&                             getServer() const;
		Location&                           getLocation() const;
		short                               getStatusCode() const;
        std::map<std::string, std::string>  getHeaders() const;
		std::string                         getBody() const;

        void    setStatusCode(short statusCode);
        void    addHeader(std::string name, std::string value);
        void    setBody(std::string body);

        Location&   matchLocation(std::string path);
        bool        methodIsAllowedOnLocation();
        void        handleGetRequest();
        void        redirect();
        std::string constructBody(std::string path);
        std::string constructBodyFromDirectory(std::string path);
        void        handlePostRequest();
        void        handleDeleteRequest();
};

std::string constructBodyFromFile(std::string pathToFile);
std::string getReasonPhrase(short statusCode);
std::string getContentType(std::string filename);


#endif