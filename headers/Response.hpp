/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Response.hpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: fkoolhov <fkoolhov@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/23 17:04:25 by fkoolhov      #+#    #+#                 */
/*   Updated: 2024/05/08 18:00:24 by fhuisman      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "webserv.hpp"
# include "Request.hpp"
# include "Server.hpp"

class Response
{
	private:
		Server&								_server;
		short								_statusCode;
		std::string							_reasonPhrase;
		std::string							_statusLine; // http version, status code, reason phrase
		std::map<std::string, std::string>	_responseHeaders;
		std::string							_body;
		std::string							_responseMessage; // status line , headers + CRLF + body + 2 * CRLF

	public:
		Response(Request& request, Server& server); // in case of succesful request
		Response(short statusCode, Server& server); // in case of error in request
		~Response();

		void	setStatusCode(short statusCode);
		void	setReasonPhrase(short statusCode);
		void	setStatusLine(std::string statusLine);
		void	addResponseHeader(std::string name, std::string value);
		void	setBody(std::string body);
		void	setResponseMessage(std::string responseMessage);
		
		Server&									getServer() const;
		short									getStatusCode() const;
		std::string								getReasonPhrase() const;
		std::string								getReasonPhrase(short statusCode) const;
		std::string								getStatusLine() const;
		std::map<std::string, std::string>		getResponseHeaders() const;
		std::string								getBody() const;
		std::string								getResponseMessage() const;

		std::string							constructStatusLine();
		std::map<std::string, std::string>	constructHeaders();
		std::string							constructBody(short statusCode);
		std::string							constructBody(Request& request);
		std::string							constructResponseMessage();
		std::string							constructErrorPage();
		std::string							constructBodyFromFile(std::ifstream& file);

		Location&	matchLocation(std::string path);
		std::string	redirect(Location& location);

};

std::ostream& operator<<(std::ostream& out_stream, const Response& response);

#endif