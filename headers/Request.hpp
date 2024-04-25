/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Request.hpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: fhuisman <fhuisman@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/18 12:50:51 by fhuisman      #+#    #+#                 */
/*   Updated: 2024/04/24 14:18:01 by fhuisman      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <string>
#include <map>
#include <sstream>
#include "webserv.hpp"

class Request
{
private:
    e_methods                           _method;
    std::string                         _uri;
    std::string                         _host;
    std::string                         _path;
    std::string                         _query;
    std::string                         _fragmentIdentifier;
    std::map<std::string, std::string>  _headers;
    std::string                         _body;
    int                                 _port;
    int                                 _contentLength;

public:
    Request() = delete;
    Request(const std::string request);
    ~Request();

    int                                 getMethod() const;
    std::string                         getUri() const;
    std::string                         getHost() const;
    std::string                         getPath() const;
    std::string                         getQuery() const;
    std::string                         getFragmentIdentifier() const;
    std::map<std::string, std::string>  getHeaders() const;
    std::string                         getBody() const;
    int                                 getPort() const;

    void    setMethod(std::string method);
    void    setUri(std::string uri);
    void    setHost(std::string host);
    void    setPath(std::string path);
    void    setQuery(std::string query);
    void    setFragmentIdentifier(std::string fragmentIdentifier);
    void    setHeader(std::string headerName, std::string headerValue);
    void    setBody(std::string body);
    void    setPort(int port);

    void    parse_uri(std::string uri);
};

std::ostream&   operator<<(std::ostream& os, const Request& request);

bool is_valid_method(std::string &method);
bool is_http_protocol(std::string &protocol);
bool is_http1_1_protocol(std::string &protocol);
void trim_lws(std::string &s);
void trim_cr(std::string &s);
void str_to_lower(std::string &s);

#endif