/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Response.hpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: fkoolhov <fkoolhov@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/23 17:04:25 by fkoolhov      #+#    #+#                 */
/*   Updated: 2024/05/02 10:37:44 by fhuisman      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "webserv.hpp"
# include "Request.hpp"

class Response
{
	private:
		std::unique_ptr<Request>				_request;
		short									_statusCode;
		const std::string						_reasonPhrase;
		std::string								_statusLine; // http version, status code, reason phrase
		std::map<std::string, std::string>		_responseHeaders;
		std::string								_body; //
		std::string								_responseMessage; // status line , headers + CRLF + body + 2 * CRLF

	public:
		Response(short errorCode); // in case of error in request
		Response(std::unique_ptr<Request> request); // in case of succesful request
		~Response();

		void	setRequest(std::unique_ptr<Request> request);
		void	setStatusCode(short statusCode);
		void	setStatusLine(std::string statusLine);
		void	addResponseHeader(std::string name, std::string value);
		void	setBody(std::string body);
		void	setResponseMessage(std::string responseMessage);
		
		const std::unique_ptr<Request>&			getRequest() const;
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
		std::string							constructBody(std::string path);
		std::string							constructResponseMessage();
		std::string							constructErrorPage();
		std::string							constructBodyFromFile(std::string path);
};

std::ostream& operator<<(std::ostream& out_stream, const Response& response);

#endif