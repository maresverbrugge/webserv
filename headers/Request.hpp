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

#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <string>
#include <map>
#include <sstream>
#include <ctime> 
#include <iomanip> 
#include <algorithm>
#include "webserv.hpp"

class Request
{
    private:
        e_methods                           _method;
        std::string                         _uri;
        std::string                         _host;
        std::string                         _path;
        std::vector<std::string>            _query;
        std::string                         _fragmentIdentifier;
        std::map<std::string, std::string>  _headers;
        std::vector<char>                   _body;
        int                                 _port;
        unsigned long long                  _contentLength;

    public:
        Request() = delete;
        Request(std::string buffer, ssize_t recv_return);
        ~Request();

    int                                 getMethod() const;
    std::string                         getUri() const;
    std::string                         getHost() const;
    std::string                         getPath() const;
    std::vector<std::string>            getQuery() const;
    std::string                         getFragmentIdentifier() const;
    std::map<std::string, std::string>  getHeaders() const;
    std::vector<char>                   getBody() const;
    int                                 getPort() const;
    unsigned long long                  getContentLength() const;

    void    setMethod(std::string method);
    void    setUri(std::string uri);
    void    setHost(std::string host);
    void    setPath(std::string path);
    void    setQuery(std::vector<std::string> query);
    void    setFragmentIdentifier(std::string fragmentIdentifier);
    void    setHeader(std::string headerName, std::string headerValue);
    void    setBody(std::vector<char>);
    void    setPort(int port);
    void    setContentLength(unsigned long long contentLength);

    void                        parseURI(std::string uri);
    std::vector<std::string>    splitQueryString(const std::string& queryString);
    void                        parsePostRequest(std::stringstream& ss, std::string request, ssize_t recv_return);

};

std::ostream&   operator<<(std::ostream& os, const Request& request);

bool        is_valid_method(std::string &method);
bool        is_http_protocol(std::string &protocol);
bool        is_http1_1_protocol(std::string &protocol);
void        trim_lws(std::string &s);
void        trim_cr(std::string &s);
void        str_to_lower(std::string &s);
std::string decodePercentEncodedString(std::string &s);

#endif