/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   request.cpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: fhuisman <fhuisman@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/18 13:12:07 by fhuisman      #+#    #+#                 */
/*   Updated: 2024/04/19 15:23:32 by fhuisman      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include <fstream>

static void parse_request_line(std::stringstream& ss, Request *request)
{
    std::string request_line;
    std::string method;
    std::string uri;
    std::string protocol;

    std::getline(ss, request_line);
    trim_cr(request_line);
    size_t sp1 = request_line.find(' ');
    size_t sp2 = request_line.find(' ', sp1 + 1);
    method = request_line.substr(0, sp1);
    protocol = request_line.substr(sp2 + 1, request_line.size() - sp2 - 1);
    if (sp1 == std::string::npos || sp2 == std::string::npos || !is_valid_method(method) || !is_http_protocol(protocol))
        throw (404);
    if (!is_http1_1_protocol(protocol))
        throw (505);
    uri = request_line.substr(sp1 + 1, sp2 - sp1 - 1);
    request->setMethod(method);
    request->setUri(uri);
}

static std::string look_for_header_continuation(std::stringstream &ss, std::string &header_line)
{
    std::string next_line;
    
    std::getline(ss, next_line);
    auto it = next_line.begin();
    while (*it == ' ' || *it == '\t')
    {
        trim_cr(next_line);
        trim_lws(next_line);
        header_line += next_line;
        std::getline(ss, next_line);
        it = next_line.begin();
    }
    return (next_line);
}

static void add_headers(Request *request, std::stringstream &ss)
{
    std::string header_line;
    std::string next_line;

    std::getline(ss, header_line);
    while (header_line != "" && header_line != "\r")
    {
        std::string header_name;
        std::string header_value;
        size_t semicolon;

        trim_cr(header_line);
        next_line = look_for_header_continuation(ss, header_line);
        semicolon = header_line.find_first_of(':');
        if (semicolon == std::string::npos)
            throw (404);
        header_name = header_line.substr(0, semicolon);
        header_value = header_line.substr(semicolon + 1, header_line.size() - semicolon);
        trim_lws(header_value);
        header_name = decodePercentEncodedString(header_name);
        header_value = decodePercentEncodedString(header_value);
        str_to_lower(header_name);
        str_to_lower(header_value);
        request->setHeader(header_name, header_value);
        header_line = next_line;
    }
}

Request::Request(std::string const request) : _port(-1), _contentLength(0)
{
    std::stringstream ss(request);

    if (request.find("\r\n\r\n") == std::string::npos)
        std::cerr << "Request is not complete" << std::endl; // is dit altijd bad request of kan t nog niet helemaal ontvangen zijn?
    std::cout << "Request constructor called" << std::endl;
    parse_request_line(ss, this);
    add_headers(this, ss);
    parseURI(_uri);
    if (_method == POST)
        parsePostRequest(ss);
}

Request::~Request()
{
    std::cout << "Request destructor called" << std::endl;
}

int Request::getMethod() const
{
    return (_method);
}

std::string Request::getUri() const
{
    return (_uri);
}

std::string Request::getHost() const
{
    return (_host);
}

std::string Request::getPath() const
{
    return (_path);
}

std::string Request::getQuery() const
{
    return (_query);
}

std::string Request::getFragmentIdentifier() const
{
    return (_fragmentIdentifier);
}

std::map<std::string, std::string> Request::getHeaders() const
{
    return (_headers);
}

std::string Request::getBody() const
{
    return (_body);
}

int Request::getPort() const
{
    return (_port);
}

int Request::getCntentLength() const
{
    return (_contentLength);
}

void Request::setMethod(std::string method)
{
    if (method == "GET")
        _method = GET;
    else if (method == "DELETE")
        _method = DELETE;
    else if (method == "POST")
        _method = POST;
}

void Request::setUri(std::string uri)
{
    _uri = uri;
}

void Request::setHost(std::string host)
{
    _host = host;
}

void Request::setPath(std::string path)
{
    _path = path;
}

void Request::setQuery(std::string query)
{
    _query = query;
}

void Request::setFragmentIdentifier(std::string fragmentIdentifier)
{
    _fragmentIdentifier = fragmentIdentifier;
}

void Request::setHeader(std::string headerName, std::string headerValue)
{
    auto it = _headers.find(headerName);
    if (it != _headers.end())
        it->second += ", " + headerValue;
    else
        _headers[headerName] = headerValue;
}

void Request::setBody(std::string body)
{
    _body = body;
}

void Request::setPort(int port)
{
    _port = port;
}

void Request::setContentLength(int contentLength)
{
    _contentLength = contentLength;
}

std::ostream &operator<<(std::ostream &os, const Request &request)
{
    std::cout << GREEN BOLD "\nRequest:\n" RESET;
    int method = request.getMethod();
    if (method == GET)
        os << "GET";
    else if (method == DELETE)
        os << "DELETE";
    else if (method == POST)
        os << "POST";
    os << " " << request.getHost() << ":" << request.getPort() << request.getPath();
    if (request.getQuery() != "")
        os << "?" << request.getQuery();
    if (request.getFragmentIdentifier() != "")
        os << "#" << request.getFragmentIdentifier();
    os << " HTTP/1.1" << std::endl;
    for (auto it : request.getHeaders())
        os << it.first << ": " << it.second << std::endl;
    os << std::endl;
    os << request.getBody() << std::endl;
    if (request.getCntentLength() == -1)
        os << "This request is encoded" << std::endl;
    else
        os << "Content-Length: " << request.getCntentLength() << std::endl;
    return (os);
}