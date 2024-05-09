/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Response.cpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: fkoolhov <fkoolhov@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/25 14:08:27 by fkoolhov      #+#    #+#                 */
/*   Updated: 2024/05/09 15:04:30 by fhuisman      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"
#include "Request.hpp"
#include "Location.hpp"
#include <stdlib.h>
#include <iomanip> //put_time(), gmtime()
#include <dirent.h> //opendir()
#include <filesystem> //is_directory()

Response::Response(Request& request, Server& server) :  _server(server),
                                                        _statusCode(200),
                                                        _reasonPhrase(getReasonPhrase(200)),
                                                        _statusLine(constructStatusLine()),
                                                        _responseHeaders(constructHeaders()),
                                                        _body(constructBody(request)),
                                                        _responseMessage(constructResponseMessage())
{
    std::cout << "Response request constructor called" << std::endl;
}

Response::Response(short statusCode, Server& server) :  _server(server),
                                                        _statusCode(statusCode),
                                                        _reasonPhrase(getReasonPhrase(statusCode)),
                                                        _statusLine(constructStatusLine()),
                                                        _responseHeaders(constructHeaders()),
                                                        _body(constructBody(statusCode)),
                                                        _responseMessage(constructResponseMessage())
{
    std::cout << "Response statusCode constructor called" << std::endl;
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

void Response::addResponseHeader(std::string name, std::string value)
{
    auto it = _responseHeaders.find(name);
    if (it != _responseHeaders.end())
        it->second += ", " + value;
    else
        _responseHeaders[name] = value;
}

void Response::setStatusLine(std::string statusLine)
{
    this->_statusLine = statusLine;
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

std::string Response::getReasonPhrase() const
{
    return this->_reasonPhrase;
}

std::string Response::getReasonPhrase(short statusCode) const
{
    std::map<short, std::string> reasonPhrases = {{100, "Continue"}, {101, "Switching Protocols"}, {200, "OK"}, {201, "Created"}, {202, "Accepted"}, {203, "Non-Authoritative Information"}, {204, "No Content"}, {205, "Reset Content"}, {206, "Partial Content"}, {300, "Multiple Choices"}, {301, "Moved Permanently"}, {302, "Found"}, {303, "See Other"}, {304, "Not Modified"}, {305, "Use Proxy"}, {307, "Temporary Redirect"}, {400, "Bad Request"}, {401, "Unauthorized"}, {402, "Payment Required"}, {403, "Forbidden"}, {404, "Not Found"}, {405, "Method Not Allowed"}, {406, "Not Acceptable"}, {407, "Proxy Authentication Required"}, {408, "Request Time-out"}, {409, "Conflict"}, {410, "Gone"}, {411, "Length Required"}, {412, "Precondition Failed"}, {413, "Request Entity Too Large"}, {414, "Request-URI Too Large"}, {415, "Unsupported Media Type"}, {416, "Requested range not satisfiable"}, {417, "Expectation Failed"}, {500, "Internal Server Error"}, {501, "Not Implemented"}, {502, "Bad Gateway"}, {503, "Service Unavailable"}, {504, "Gateway Time-out"}, {505, "HTTP Version not supported"}};
    auto it = reasonPhrases.find(statusCode);
    if (it == reasonPhrases.end())
        ;//ik denk dat dit nooit voorkomt, want dan hebben we zelf een statuscode gethrowt die niet bestaat. toch implementeren?
    return (*it).second;
}

std::map<std::string, std::string> Response::getResponseHeaders() const
{
    return this->_responseHeaders;
}

std::string Response::getStatusLine() const
{
    return this->_statusLine;
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

static std::string generateDateHeader() {
    std::time_t currentTime = std::time(nullptr);
    std::tm* gmTime = std::gmtime(&currentTime);
    std::stringstream ss;
    ss << std::put_time(gmTime, "%a, %d %b %Y %H:%M:%S GMT");
    return ss.str();
}

std::map<std::string, std::string> Response::constructHeaders()
{
    std::map<std::string, std::string> headers;
    addResponseHeader("Date", generateDateHeader());
    for (auto serverName : _server.getServerNames())
    {
        addResponseHeader("Server", serverName);
    }
    addResponseHeader("Connection", "close");
    return (headers);
}

std::string Response::constructErrorPage()
{
    std::string errorPage = "<!DOCTYPE html>\r\n<html lang=\"en\">\r\n<head>\r\n\t<meta charset=\"UTF-8\">\r\n\t<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\r\n\t<title>";
    errorPage += std::to_string(_statusCode) + " " + getReasonPhrase();
    errorPage +="</title>\r\n\t<style>\r\n\t\tbody {\r\n\t\t\tfont-family: Arial, sans-serif;\r\n\t\t\tmargin: 0;\r\n\t\t\tpadding: 0;\r\n\t\t\tbackground-color: #f4f4f4;\r\n\t\t}\r\n\t\t.container {\r\n\t\t\twidth: 80%;\r\n\t\t\tmargin: 50px auto;\r\n\t\t\ttext-align: center;\r\n\t\t}\r\n\t\th1 {\r\n\t\t\tcolor: #dc3545;\r\n\t\t}\r\n\t\tp {\r\n\t\t\tcolor: #6c757d;\r\n\t\t}\r\n\t</style>\r\n</head>\r\n<body>\r\n\t<div class=\"container\">\r\n\t\t<h1>Oops! Something went wrong.</h1>\r\n\t\t<p>We apologize for the inconvenience. Please try again later.</p>\r\n\t</div>\r\n</body>\r\n</html>";
    addResponseHeader("Content-Type", "text/html");
    addResponseHeader("Content-Length", std::to_string(errorPage.size()));
    return (errorPage);
}

std::string Response::constructBody(short statusCode)
{
    std::string body;

    (void) statusCode;
    std::map<short, std::string> customErrorPages = _server.getCustomErrorPages();
    auto it = customErrorPages.find(statusCode);
    if (it == customErrorPages.end())
        body = constructErrorPage();
    else
        body = constructBodyFromFile((*it).second);
    return (body);
}

std::string Response::constructBodyFromFile(std::string pathToFile)
{
    std::string body;
    std::string line;
    std::ifstream file;
    file.open(pathToFile);
    if (!file.is_open())
        throw (404);
    if (std::getline(file, line))
        body += line;
    while (std::getline(file, line))
    {
        body += "\r\n" + line;
    }
    file.close();
    addResponseHeader("Content-Type", "text/html");
    addResponseHeader("Content-Length", std::to_string(body.size()));
    return (body);
}

std::string Response::redirect(Location& location)
{
    setStatusCode(302);
    setReasonPhrase(302);
    addResponseHeader("Location", location.getRedirectLink());
    setStatusLine(constructStatusLine());
    return ("");
}

static std::string getAbsolutePath(Location& location, std::string path)
{
    return (location.getPath() + path.substr(path.find(location.getLocationName()) + location.getLocationName().size()));
}

std::string Response::constructBodyFromDirectory(Location& location, std::string path)
{
    std::string body;

    if (!location.getDirectoryListing())
    {
        std::string defaultPage = location.getDefaultPage();
        if (defaultPage == "")
            throw (403);
        return (constructBodyFromFile(defaultPage));
    }
    DIR* dir_stream = opendir(path.c_str());
    if (!dir_stream)
        throw (404);
    struct dirent *dirent;
    body = "<!DOCTYPE html>\r\n<html lang=\"en\">\r\n<head>\r\n\t<meta charset=\"UTF-8\">\r\n\t<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\r\n\t<title>Directory Listing</title>\r\n</head>\r\n<body>\r\n\t<h1>Directory Listing</h1>\r\n\t<p>The page you requested is a directory</p>\r\n\t<ul>\r\n";
    while ((dirent = readdir(dir_stream)))
    {
        if (dirent->d_name[0] != '.')
            body += "\t\t<li><a href=\"" + path + dirent->d_name + "\">" + dirent->d_name + "</a></li>\r\n";
    }
    body += "\t</ul>\r\n</body>\r\n</html>";
    closedir(dir_stream);
    return (body);
}

std::string Response::constructBody(Request& request)
{
    std::string body;
    std::string path;
    std::ifstream file;
    
    path = request.getPath();
    Location& location = matchLocation(path);
    if (location.getRedirectLink() != "")
        return (redirect(location));
    auto allowedMethods = location.getAllowedMethods();
    if (allowedMethods[request.getMethod()] == false)
        throw (405);
    path = getAbsolutePath(location, path);
    if (std::filesystem::is_directory(path))
        return (constructBodyFromDirectory(location, path));
    return (constructBodyFromFile(path));
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

Location& Response::matchLocation(std::string path)
{
    if (path == "" || path == "/")
    {
        try
        {
            return (_server.getDefaultLocation());
        }
        catch(const std::exception& e)
        {
            throw (404);
        }
    }
    auto& locations = _server.getLocations();
    for (auto it = locations.begin(); it < locations.end(); it++)
    {
        Location& location = **it;
        if (location.getLocationName() == path)
            return (location);
    }
    return (matchLocation(path.substr(0, path.find_last_of('/'))));
}

std::ostream& operator<<(std::ostream& out_stream, const Response& response)
{
    std::cout << GREEN BOLD "Response:\n" RESET;
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