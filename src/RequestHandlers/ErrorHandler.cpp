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

#include "ErrorHandler.hpp"
#include "Server.hpp"

ErrorHandler::ErrorHandler(short statusCode, const Server& server) : _server(server),
                                                                        _statusCode(statusCode),
                                                                        _body(constructBody(_statusCode)) {}

ErrorHandler::~ErrorHandler() {}

const Server& ErrorHandler::getServer() const
{
    return _server;
}

short ErrorHandler::getStatusCode() const
{
    return _statusCode;
}

std::map<std::string, std::string> ErrorHandler::getHeaders() const
{
    return _headers;
}

std::string ErrorHandler::getBody() const
{
    return _body;
}

void ErrorHandler::addHeader(std::string name, std::string value)
{
    auto it = _headers.find(name);
    if (it != _headers.end())
        it->second += ", " + value;
    else
        _headers[name] = value;
}

void ErrorHandler::setBody(std::string body)
{
    _body = (body);
}

std::string ErrorHandler::constructErrorPage()
{
    std::string errorPage = "<!DOCTYPE html>\r\n<html lang=\"en\">\r\n<head>\r\n\t<meta charset=\"UTF-8\">\r\n\t<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\r\n\t<title>";
    errorPage += std::to_string(_statusCode) + " " + getReasonPhrase(_statusCode);
    errorPage += "</title>\r\n<link rel=\"icon\" type=\"image/x-icon\" href=\"../img/favicon.ico\">\r\n\t<link rel=\"stylesheet\" href=\"/styles/error.css\">\r\n</head>\r\n<body>\r\n\t<div class=\"container\">\r\n\t\t<h1>";
    errorPage += std::to_string(_statusCode) + " " + getReasonPhrase(_statusCode);
    errorPage += "</h1>\r\n\t\t<p>We apologize for the inconvenience. You should probably go back to the <a href=\"/pages/index.html\">homepage</a> and try again.</p>\r\n\t</div>\r\n</body>\r\n</html>";
    return errorPage;
}

std::string ErrorHandler::constructBody(short statusCode)
{
    std::string body;

    const auto& customErrorPages = _server.getCustomErrorPages();
    auto it = customErrorPages.find(statusCode);
    if (it == customErrorPages.end())
        body = constructErrorPage();
    else
    {
        try
        {
            body = constructBodyFromFile(it->second);
        }
        catch (const e_status& statusCode)
        {
            body = constructErrorPage();
        }
    }
    addHeader("Content-Type", "text/html");
    return body;
}
