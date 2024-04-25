/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkoolhov <fkoolhov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 14:08:27 by fkoolhov          #+#    #+#             */
/*   Updated: 2024/04/25 14:56:43 by fkoolhov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response(std::unique_ptr<Request> request)
{
    std::cout << "Response constructor called" << std::endl;
    this->_request = std::move(request);
    this->_statusCode = 200;
    this->_reasonPhrase = "OK";
    this->_header = "";
    this->_body = "";
    this->_responseMessage = "";
}

Response::Response(short errorCode)
{
    std::cout << "Response constructor called" << std::endl;
    this->_statusCode = errorCode;
    this->_reasonPhrase = "";
    this->_header = "";
    this->_body = ""; // get from server error page
    this->_responseMessage = "";
}

Response::~Response()
{
    std::cout << "Response destructor called" << std::endl;
}

void Response::setRequest(std::unique_ptr<Request> request)
{
    this->_request = std::move(request);
}

void Response::setStatusCode(short statusCode)
{
    this->_statusCode = statusCode;
}

void Response::setReasonPhrase(std::string reasonPhrase)
{
    this->_reasonPhrase = reasonPhrase;
}

void Response::addResponseHeader(std::string name, std::string value)
{
    this->_responseHeaders[name] = value;
}

void Response::setHeader(std::string header)
{
    this->_header = header;
}

void Response::setBody(std::string body)
{
   this->_body = body;
}

void Response::setResponseMessage(std::string responseMessage)
{
    this->_responseMessage = responseMessage;
}

const std::unique_ptr<Request>& Response::getRequest() const
{
    return this->_request;
}

short Response::getStatusCode() const
{
    return this->_statusCode;
}

std::string Response::getReasonPhrase() const
{
    return this->_reasonPhrase;
}

std::map<std::string, std::string> Response::getResponseHeaders() const
{
    return this->_responseHeaders;
}

std::string Response::getHeader() const
{
    return this->_header;
}

std::string Response::getBody() const
{
    return this->_body;
}

std::string Response::getResponseMessage() const
{
    return this->_responseMessage;
}

void Response::constructResponseMessage()
{
    // search path in server locations
    // read file (if exists)
    // set server body (html if file found)
    // set status code
    // set reason phrase
    // set header (http version + status code + reason phrase + optional response headers)
    // set response message (header + new line + body)
    // reponse is either requested file or error message

    // EXAMPLE:
    // header
    //	status line
    //		http version
    //		status code
    //		reason phrase
    //	optional response headers
    //		name:value
    //		name:value

    // blank line

    // body (optional)
    //	html
}

std::ostream& operator<<(std::ostream& out_stream, const Response& response)
{
    std::cout << GREEN BOLD "Response:\n" RESET;
    out_stream << "_statusCode: " << response.getStatusCode() << std::endl;
    out_stream << "_reasonPhrase: " << response.getReasonPhrase() << std::endl;
    
    out_stream << "_reasponseHeader: \n";
    std::map<std::string, std::string> response_headers = response.getResponseHeaders();
    for (auto it = response_headers.begin(); it != response_headers.end(); it++)
    {
        std::cout << "Name: " << it->first << " Value: " << it->second << std::endl;
    }
    
    out_stream << "full _header: " << response.getHeader() << std::endl;
    out_stream << "full _body: " << response.getBody() << std::endl;
    out_stream << "full _responseMessage: " << response.getResponseMessage() << std::endl;
    return out_stream;
}