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

ErrorHandler::ErrorHandler(short statusCode, const Server& server) : _server(server),
                                                                        _statusCode(statusCode),
                                                                        _body(constructBody(_statusCode))
{
    std::cout << "ErrorHandler constructor called" << std::endl;
}


ErrorHandler::~ErrorHandler()
{
    std::cout << "ErrorHandler destructor called" << std::endl;
}

const Server& ErrorHandler::getServer() const
{
    return (_server);
}

short ErrorHandler::getStatusCode() const
{
    return (_statusCode);
}

std::map<std::string, std::string> ErrorHandler::getHeaders() const
{
    return (_headers);
}

std::string ErrorHandler::getBody() const
{
    return (_body);
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
    errorPage += "</title>\r\n\t<style>\r\n\t\tbody {\r\n\t\t\tfont-family: Arial, sans-serif;\r\n\t\t\tmargin: 0;\r\n\t\t\tpadding: 0;\r\n\t\t\tbackground-color: #f4f4f4;\r\n\t\t}\r\n\t\t.container {\r\n\t\t\twidth: 80%;\r\n\t\t\tmargin: 50px auto;\r\n\t\t\ttext-align: center;\r\n\t\t}\r\n\t\th1 {\r\n\t\t\tcolor: #dc3545;\r\n\t\t}\r\n\t\tp {\r\n\t\t\tcolor: #6c757d;\r\n\t\t}\r\n\t</style>\r\n</head>\r\n<body>\r\n\t<div class=\"container\">\r\n\t\t<h1>";
    errorPage += std::to_string(_statusCode) + " " + getReasonPhrase(_statusCode);
    errorPage += "</h1>\r\n\t\t<p>We apologize for the inconvenience. Please try again later.</p>\r\n\t</div>\r\n</body>\r\n</html>";
    addHeader("Content-Type", "text/html");
    addHeader("Content-Length", std::to_string(errorPage.size()));
    return (errorPage);
}

std::string ErrorHandler::constructBody(short statusCode)
{
    std::string body;

    const auto& customErrorPages = _server.getCustomErrorPages();
    auto it = customErrorPages.find(statusCode);
    if (it == customErrorPages.end())
        return (constructErrorPage());
    try
    {
        return (constructBodyFromFile(it->second));
    }
    catch(const e_status& statusCode)
    {
        return (constructErrorPage());
    }
}
