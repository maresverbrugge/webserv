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

#include "Response.hpp"
#include "Request.hpp"
#include "Location.hpp"
#include <stdlib.h>
#include <iomanip> //put_time(), gmtime()

Response::Response(RequestHandler& requestHandler) :    _server(requestHandler.getServer()),
                                                        _statusCode(requestHandler.getStatusCode()),
                                                        _reasonPhrase(getReasonPhrase(_statusCode)),
                                                        _statusLine(constructStatusLine()),
                                                        _responseHeaders(requestHandler.getHeaders()),
                                                        _body(requestHandler.getBody()),
                                                        _responseMessage("")
{
    std::cout << "Response RequestHandler constructor called" << std::endl;
    addHeaders();
    setResponseMessage(constructResponseMessage());
}

Response::Response(ErrorHandler& errorHandler) :    _server(errorHandler.getServer()),
                                                    _statusCode(errorHandler.getStatusCode()),
                                                    _reasonPhrase(getReasonPhrase(_statusCode)),
                                                    _statusLine(constructStatusLine()),
                                                    _responseHeaders(errorHandler.getHeaders()),
                                                    _body(errorHandler.getBody()),
                                                    _responseMessage("")
{
    std::cout << "Response errorHandler constructor called" << std::endl;
    addHeaders();
    setResponseMessage(constructResponseMessage());
}

Response::~Response()
{
    std::cout << "Response destructor called" << std::endl;
}

void Response::setStatusCode(short statusCode)
{
    this->_statusCode = statusCode;
}

void Response::setReasonPhrase(short statusCode)
{
    _reasonPhrase = getReasonPhrase(statusCode);
}

void Response::setStatusLine(std::string statusLine)
{
    this->_statusLine = statusLine;
}

void Response::addResponseHeader(std::string name, std::string value)
{
    auto it = _responseHeaders.find(name);
    if (it != _responseHeaders.end())
        it->second += ", " + value;
    else
        _responseHeaders[name] = value;
}

void Response::setBody(std::string body)
{
   this->_body = body;
}

void Response::setResponseMessage(std::string responseMessage)
{
    this->_responseMessage = responseMessage;
}

Server& Response::getServer() const
{
    return this->_server;
}

short Response::getStatusCode() const
{
    return this->_statusCode;
}

std::string Response::getStatusLine() const
{
    return this->_statusLine;
}

std::map<std::string, std::string> Response::getResponseHeaders() const
{
    return this->_responseHeaders;
}

std::string Response::getBody() const
{
    return this->_body;
}

std::string Response::getResponseMessage() const
{
    return this->_responseMessage;
}

std::string Response::constructStatusLine()
{
    std::string statusLine;

    statusLine = "HTTP/1.1 ";
    statusLine += std::to_string(_statusCode) + ' ';
    statusLine += _reasonPhrase;
    return (statusLine);
}

std::string Response::constructResponseMessage()
{
    std::string responseMessage;
    responseMessage = _statusLine + "\r\n";
    for (auto pair : _responseHeaders)
    {
        responseMessage += pair.first + ": " + pair.second + "\r\n";
    }
    if (_body != "")
        responseMessage += "\r\n" + _body + "\r\n";
    responseMessage += "\r\n";
    return (responseMessage);
}

static std::string generateDateHeader() {
    std::time_t currentTime = std::time(nullptr);
    std::tm* gmTime = std::gmtime(&currentTime);
    std::stringstream ss;
    ss << std::put_time(gmTime, "%a, %d %b %Y %H:%M:%S GMT");
    return ss.str();
}

std::string Response::PercentEncoding(std::string string)
{
    std::ostringstream encodedString;

    for (char c : string)
    {
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~')
            encodedString << c;
        else
            encodedString << '%' << std::uppercase << std::hex << ((c >> 4) & 0x0F) << (c & 0x0F);
    }

    return encodedString.str();
}

void Response::addHeaders()
{
    addResponseHeader("Date", generateDateHeader());
    for (auto serverName : _server.getServerNames())
    {
        addResponseHeader("Server", PercentEncoding(serverName));
    }
    addResponseHeader("Connection", "close");
}

std::ostream& operator<<(std::ostream& out_stream, const Response& response)
{
    std::cout << PURPLE BOLD "Response:\n" RESET;
    out_stream << "_statusCode: " << response.getStatusCode() << std::endl;
    out_stream << "_statusLine: " << response.getStatusLine() << std::endl;
    
    out_stream << "_responseHeaders:\n";
    std::map<std::string, std::string> response_headers = response.getResponseHeaders();
    for (auto it = response_headers.begin(); it != response_headers.end(); it++)
    {
        std::cout << "Name: " << it->first << " Value: " << it->second << std::endl;
    }
    
    out_stream << "\nfull _body:\n" << response.getBody() << std::endl;
    out_stream << "full _responseMessage:\n" << response.getResponseMessage() << std::endl;
    return out_stream;
}