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

#include "RequestHandler.hpp"

RequestHandler::RequestHandler(Request& request, const Server& server) : _request(request),
                                                                            _server(server),
                                                                            _location(matchLocation(_request.getPath())),
                                                                            _statusCode(OK),
                                                                            _body(""),
                                                                            _absPath(findAbsolutePath()),
                                                                            _extension(extractExtension()),
                                                                            _CGI(false)
{
    std::cout << "RequestHandler constructor called" << std::endl;

    if (_location.getRedirectLink() != "")
    {
        redirect();
        return;
    }
    if(!methodIsAllowedOnLocation())
        throw (METHOD_NOT_ALLOWED);
    if (_extension.size() != 0 && _extension == _location.getCgiExtension())
    {
        _CGI = true;
        handleCGI();
        return ;
    }
    if (_request.getMethod() == GET)
        handleGetRequest();
    else if (_request.getMethod() == POST)
        handlePostRequest();
    else if(_request.getMethod() == DELETE)
        handleDeleteRequest();
}

RequestHandler::~RequestHandler()
{
    std::cout << "RequestHandler destructor called" << std::endl;
}

Request& RequestHandler::getRequest() const
{
    return (_request);
}

const Server& RequestHandler::getServer() const
{
    return (_server);
}

Location& RequestHandler::getLocation() const
{
    return (_location);
}

short RequestHandler::getStatusCode() const
{
    return (_statusCode);
}

std::map<std::string, std::string> RequestHandler::getHeaders() const
{
    return (_headers);
}

std::string RequestHandler::getBody() const
{
    return (_body);
}

bool RequestHandler::isCGI() const
{
    return (_CGI);
}

void RequestHandler::setStatusCode(short statusCode)
{
    _statusCode = statusCode;
}

void RequestHandler::addHeader(std::string name, std::string value)
{
    auto it = _headers.find(name);
    if (it != _headers.end())
        it->second += ", " + value;
    else
        _headers[name] = value;
}

void RequestHandler::setBody(std::string body)
{
    _body = body;
}

void RequestHandler::setCGI(bool cgi)
{
    _CGI = cgi;
}

Location& RequestHandler::matchLocation(std::string path)
{
    if (path.empty() || path == "/")
    {
        try
        {
            return (_server.getDefaultLocation());
        }
        catch(const std::exception& e)
        {
            throw (NOT_FOUND);
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

bool RequestHandler::methodIsAllowedOnLocation()
{
    auto allowedMethods = _location.getAllowedMethods();
    return (allowedMethods[_request.getMethod()]);
}

std::string RequestHandler::findAbsolutePath()
{
    std::string path = _request.getPath();
    return (_location.getPath() + path.substr(path.find(_location.getLocationName()) + _location.getLocationName().size()));
}

std::string RequestHandler::extractExtension()
{
    return (_absPath.substr(_absPath.find_last_of('.')));
}

void RequestHandler::redirect()
{
    setStatusCode(FOUND);
    addHeader("Location", _location.getRedirectLink());
    addHeader("Content-Length", "0");
}
