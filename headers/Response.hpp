/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkoolhov <fkoolhov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 17:04:25 by fkoolhov          #+#    #+#             */
/*   Updated: 2024/04/23 17:33:32 by fkoolhov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "webserv.hpp"

class Response
{
	private:
		std::vector<std::unique_ptr<Request>>	_request;
		short									_statusCode;
		std::string								_reasonPhrase;
		std::map<std::string, std::string>		_responseHeaders;
		std::string								_body;
		std::string								_responseMessage;

	public:
		Response();
		~Response();

		void	setRequest(std::unique_ptr<Request> request);
		void	setStatusCode(short);
		void	setReasonPhrase(std::string);
		void	addResponseHeader(std::string name, std::string value);
		void	setBody(std::string);
		void	setResponseMessage(std::string responseMessage);
		
		std::vector<std::unique_ptr<Request>>&	getRequest() const;
		short									getStatusCode() const;
		std::string								getReasonPhrase() const;
		std::map<std::string, std::string>		getResponseHeaders() const;
		std::string								getBody() const;
		std::string								getResponseMessage() const;
};

#endif