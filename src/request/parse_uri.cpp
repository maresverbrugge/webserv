#include <iostream>
#include <string>
#include "../../headers/request.hpp"

static void trim_scheme(std::string& uri)
{
    size_t scheme_pos = uri.find("//");
    if (scheme_pos != std::string::npos)
        uri = uri.substr(scheme_pos + 2);
}

static std::string get_fragment_identifier(std::string& uri)
{
    std::string fragment_identifier;
    size_t hash_pos = uri.find('#');
    if (hash_pos != std::string::npos)
    {
        fragment_identifier = uri.substr(hash_pos + 1);
        uri = uri.substr(0, hash_pos);
    }
    return fragment_identifier;
}

static std::string get_query(std::string& uri)
{
    std::string query_string;
    size_t question_pos = uri.find('?');
    if (question_pos != std::string::npos)
    {
        query_string = uri.substr(question_pos + 1);
        uri = uri.substr(0, question_pos);
    }
    return (query_string);
}

static std::string get_path(std::string& uri)
{
    std::string path;
    size_t slash_pos = uri.find('/');
    if (slash_pos == std::string::npos)
        std::cout << "404 bad request" << std::endl;
    path = uri.substr(slash_pos);
    uri = uri.substr(0, slash_pos);
    return (path);
}

static int get_port(std::string& uri)
{
    std::string port_string;
    size_t colon_pos = uri.find_last_of(':');
    if (colon_pos != std::string::npos)
    {
        port_string = uri.substr(colon_pos + 1);
        uri = uri.substr(0, colon_pos);
        return (std::atoi(port_string.c_str()));
    }
    return (80); // default http port
}

static std::string get_host(std::string& uri, Request *request)
{
    size_t      bracket_pos = uri.find('[');
    size_t      closing_pos = uri.find(']');
    std::string host;
    std::string port;

    host = uri;
    if (bracket_pos != std::string::npos)
    {
        if (closing_pos == std::string::npos)
            std::cerr << "404 bad request";
        host = uri.substr(bracket_pos + 1, closing_pos - (bracket_pos + 1));
    }
    if (host == "")
    {
        std::map<std::string, std::string> headers = request->getHeaders();
        auto it = headers.find("Host"); //case insensitive maken
        if (it == headers.end())
            std::cerr << "404 bad request";
        else
        {
            host = (*it).second;
            size_t colon_pos = host.find(':');
            if (colon_pos != std::string::npos)
            {
                port = host.substr(colon_pos + 1);
                request->setPort(atoi(port.c_str()));
                host = host.substr(0, colon_pos);
            }
        }
    }
    return (host);
}

void Request::parse_uri(std::string uri)
{
    trim_scheme(uri);
    _fragmentIdentifier = get_fragment_identifier(uri);
    _query = get_query(uri);
    _path = get_path(uri);
    _port = get_port(uri);
    _host = get_host(uri, this);
}