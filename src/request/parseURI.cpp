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
        throw StatusCodeException("trim_path() failed", BAD_REQUEST);
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
            throw StatusCodeException("trim_host() failed", BAD_REQUEST);
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
    if (request->getHost().empty())
        request->setHost(host);
    if (request->getPort() == -1)
        request->setPort(port);
    if (request->getPort() == -1)
        request->setPort(80); //default port for HTTP
    if (request->getHost().empty())
        throw StatusCodeException("Empty host", BAD_REQUEST);
}

std::vector<std::string> Request::splitQueryString(const std::string& queryString)
{
    std::vector<std::string> query;
    std::size_t pos = 0;
    
    while (pos != std::string::npos) {
        std::size_t pos_next = queryString.find('&', pos);
        std::string queryPair;
        if (pos_next != std::string::npos)
            queryPair = queryString.substr(pos, pos_next - pos);
        else
            queryPair = queryString.substr(pos);
        if (!queryPair.empty() && queryPair.find('=') == std::string::npos)
            throw StatusCodeException("Query string has bad format", BAD_REQUEST);
        query.push_back(decodePercentEncodedString(queryPair));
        if (pos_next != std::string::npos)
            pos = pos_next + 1;
        else
            break;
    }

    return query;
}

void Request::parseURI(std::string uri)
{
    _fragmentIdentifier = trim_fragment_identifier(uri);
    trim_scheme(uri);
    _query = splitQueryString(trim_query(uri));
    _path = trim_path(uri);
    _port = trim_port(uri);
    _host = trim_host(uri);
    get_host_and_port_from_header(this);
    _host = decodePercentEncodedString(_host);
    str_to_lower(_host);
    _path = decodePercentEncodedString(_path);
    _fragmentIdentifier = decodePercentEncodedString(_fragmentIdentifier);
    // for (std::string s : _query)
        // std::cout << GREEN << s << RESET << std::endl; 
}