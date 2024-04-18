/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   request.hpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: fhuisman <fhuisman@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/18 12:50:51 by fhuisman      #+#    #+#                 */
/*   Updated: 2024/04/18 17:15:46 by fhuisman      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <string>
#include <map>
#include <sstream>

class Request
{
private:
    std::stringstream _request_stream;
    std::string _method;
    std::string _uri;
    std::string _protocol;
    std::string _path;
    std::string _query;
    std::string _fragmentIdentifier;
    std::map<std::string, std::string> _headers;
    std::string _body;

public:
    Request() = delete;
    Request(std::string const request);
    Request(Request &copy) = delete;
    ~Request();
    Request &operator=(Request const &request) = delete;
    std::string getMethod();
    std::string getUri();
    std::string getProtocol();
    std::string getPath();
    std::string getQuery();
    std::string getFragmentIdentifier();
    std::map<std::string, std::string> getHeaders();
    std::string getBody();
    void setMethod(std::string method);
    void setUri(std::string uri);
    void setProtocol(std::string protocol);
    void setPath(std::string path);
    void setQuery(std::string query);
    void setFragmentIdentifier(std::string fragmentIdentifier);
    void setHeader(std::string headerName, std::string headerValue);
    void setBody(std::string body);
};

#endif