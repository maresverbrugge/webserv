#include <iostream>
#include <string>
#include "Request.hpp"

static void trim_scheme(std::string& uri)
{
    size_t scheme_pos;

    scheme_pos = uri.find("//");
    if (scheme_pos != std::string::npos)
        uri = uri.substr(scheme_pos + 2);
}

static std::string trim_fragment_identifier(std::string& uri)
{
    std::string fragment_identifier;
    size_t hash_pos;
    
    hash_pos = uri.find('#');
    if (hash_pos != std::string::npos)
    {
        fragment_identifier = uri.substr(hash_pos + 1);
        uri = uri.substr(0, hash_pos);
    }
    return fragment_identifier;
}

static std::string trim_query(std::string& uri)
{
    std::string query_string;
    size_t question_pos;
    
    question_pos= uri.find('?');
    if (question_pos != std::string::npos)
    {
        query_string = uri.substr(question_pos + 1);
        uri = uri.substr(0, question_pos);
    }
    return (query_string);
}

static std::string trim_path(std::string& uri)
{
    std::string path;
    size_t slash_pos;
    
    slash_pos = uri.find('/');
    if (slash_pos == std::string::npos)
        throw (404);
    path = uri.substr(slash_pos);
    uri = uri.substr(0, slash_pos);
    return (path);
}

static int trim_port(std::string& uri)
{
    std::string port;
    size_t colon_pos;
    
    colon_pos = uri.find_last_of(':');
    if (colon_pos != std::string::npos)
    {
        port = uri.substr(colon_pos + 1);
        uri = uri.substr(0, colon_pos);
        return (std::atoi(port.c_str()));
    }
    return (-1);
}

static std::string trim_host(std::string& uri)
{
    size_t      bracket_pos = uri.find('[');
    size_t      closing_pos = uri.find(']');
    std::string host;

    host = uri;
    if (bracket_pos != std::string::npos) //trim [] brckets for when host is notated in IPv6 format ([::11])
    {
        if (closing_pos == std::string::npos)
            throw (404);
        host = uri.substr(bracket_pos + 1, closing_pos - (bracket_pos + 1));
    }
    return (host);
}

void get_host_and_port_from_header(Request *request)
{
    std::string host;
    int port = -1;
    std::map<std::string, std::string> headers = request->getHeaders();
    auto it = headers.find("host");

    if (it != headers.end())
    {
        host = (*it).second;
        port = trim_port(host);
    }
    if (request->getHost() == "")
        request->setHost(host);
    if (request->getPort() == -1)
        request->setPort(port);
    if (request->getPort() == -1)
        request->setPort(80); //default port for HTTP
    if (request->getHost() == "")
        throw (404);
}

void Request::parseURI(std::string uri)
{
    _fragmentIdentifier = trim_fragment_identifier(uri);
    trim_scheme(uri);
    _query = trim_query(uri);
    _path = trim_path(uri);
    _port = trim_port(uri);
    _host = trim_host(uri);
    get_host_and_port_from_header(this);
    _host = decodePercentEncodedString(_host);
    str_to_lower(_host);
    _path = decodePercentEncodedString(_path);
    _query = decodePercentEncodedString(_query);
    _fragmentIdentifier = decodePercentEncodedString(_fragmentIdentifier);
}