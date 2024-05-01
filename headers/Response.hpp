/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkoolhov <fkoolhov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 17:04:25 by fkoolhov          #+#    #+#             */
/*   Updated: 2024/04/25 14:47:54 by fkoolhov         ###   ########.fr       */
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
		std::string								_reasonPhrase;
		std::map<std::string, std::string>		_responseHeaders;
		std::string								_header; // only the response header (http version, status code, reason phrase, response headers)
		std::string								_body; // only the response header
		std::string								_responseMessage; // header + blank line + body

	public:
		Response(short errorCode); // in case of error in request
		Response(std::unique_ptr<Request> request); // in case of succesful request
		~Response();

		void	setRequest(std::unique_ptr<Request> request);
		void	setStatusCode(short statusCode);
		void	setReasonPhrase(std::string reasonPhrase);
		void	addResponseHeader(std::string name, std::string value);
		void	setHeader(std::string header);
		void	setBody(std::string body);
		void	setResponseMessage(std::string responseMessage);
		
		const std::unique_ptr<Request>&			getRequest() const;
		short									getStatusCode() const;
		std::string								getReasonPhrase() const;
		std::map<std::string, std::string>		getResponseHeaders() const;
		std::string								getHeader() const;
		std::string								getBody() const;
		std::string								getResponseMessage() const;

		void constructResponseMessage();
};

std::ostream& operator<<(std::ostream& out_stream, const Response& response);

#endif