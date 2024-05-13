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

void Response::addResponseHeader(std::string name, std::string value)
{
    this->_responseHeaders[name] = value;
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

std::map<std::string, std::string> Response::constructHeaders()
{
    std::map<std::string, std::string> headers;
    // welke headers moeten er in komen? Server, Accept-Ranges, Age, ETag, Location, Proxy-Authenticate, Retry-After, Vary, WWW-Authenticate?
    // bijv: headers["server"] =  getServer());
    return (headers);
}

std::string Response::constructErrorPage()
{
    std::string errorPage = "<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n\t<meta charset=\"UTF-8\">\n\t<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n\t<title>";
    errorPage += std::to_string(_statusCode) + " " + getReasonPhrase();
    errorPage +="</title>\n\t<style>\n\t\tbody {\n\t\t\tfont-family: Arial, sans-serif;\n\t\t\tmargin: 0;\n\t\t\tpadding: 0;\n\t\t\tbackground-color: #f4f4f4;\n\t\t}\n\t\t.container {\n\t\t\twidth: 80%;\n\t\t\tmargin: 50px auto;\n\t\t\ttext-align: center;\n\t\t}\n\t\th1 {\n\t\t\tcolor: #dc3545;\n\t\t}\n\t\tp {\n\t\t\tcolor: #6c757d;\n\t\t}\n\t</style>\n</head>\n<body>\n\t<div class=\"container\">\n\t\t<h1>Oops! Something went wrong.</h1>\n\t\t<p>We apologize for the inconvenience. Please try again later.</p>\n\t</div>\n</body>\n</html>";
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

std::string Response::constructBodyFromFile(std::string path)
{
    // if path is a directory: check config for default of directory listing or throw correct error
    std::string body;
    std::ifstream file;
    std::string line;
    file.open(path);
    if (!file.is_open())
        throw (404); // is dit altijd page not found? of kan bijv geen read rights andere error geven?
    while (std::getline(file, line))
    {
        body += line + "\n";
    }
    file.close();
    return (body);
}


std::string Response::constructBody(Request& request)
{
    std::string body;
    std::string path = request.getPath();

    if (path.front() == '/') //hier haal ik even de slash er af omdat t in de config zonder slash is opgeslagen
        path.erase(path.begin());
    
    auto& locations = _server.getLocations();
    for (auto it = locations.begin(); it < locations.end(); it++)
    {
        Location& location = **it;
        if (location.getLocationName() == path)
        {
            auto allowedMethods = location.getAllowedMethods();
            if (allowedMethods[request.getMethod()] == false)
                throw (405);
            path = location.getPath() + "/" + path; //hier moet de slash er weer bij.
            body = constructBodyFromFile(path);
            return (body);
        }
    }
    // location is not found, check for default or 404 Page not Found
    throw (404);
    return (body);
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