#include <iostream>
#include <string>

void trim_scheme(std::string &uri)
{
    size_t scheme_pos = uri.find("//");
    if (scheme_pos != std::string::npos)
        uri = uri.substr(scheme_pos + 2);
}

std::string trim_fragment_identifier(std::string &uri)
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

std::string trim_query(std::string &uri)
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

std::string trim_path(std::string &uri)
{
    std::string path;
    size_t slash_pos = uri.find('/');
    if (slash_pos == std::string::npos)
        std::cout << "404 bad request" << std::endl;
    path = uri.substr(slash_pos);
    uri = uri.substr(0, slash_pos);
    return (path);
}

int trim_port(std::string &uri)
{
    std::string port_string;
    size_t colon_pos = uri.find_last_of(':');
    if (colon_pos != std::string::npos)
    {
        port_string = uri.substr(colon_pos + 1);
        uri = uri.substr(0, colon_pos);
    http: //[::1]:8080/path/to/resource
        return (std::atoi(port_string.c_str()));
    }
    return (80); // default http port
}

std::string trim_host(std::string &uri)
{
    size_t bracket_pos = uri.find('[');
    size_t closing_pos = uri.find(']');
    if (bracket_pos != std::string::npos)
    {
        if (closing_pos == std::string::npos)
            std::cerr << "404 bad request";
        return (uri.substr(bracket_pos + 1, closing_pos - (bracket_pos + 1)));
    }
    return (uri);
}

void parse_uri(std::string uri)
{
    std::cout << "uri: " << uri << std::endl;
    std::string host;
    std::string path;
    std::string query_string;
    std::string fragment_identifier;
    int port;

    trim_scheme(uri);
    fragment_identifier = trim_fragment_identifier(uri);
    query_string = trim_query(uri);
    path = trim_path(uri);
    port = trim_port(uri);
    host = trim_host(uri);

    std::cout << "host: " << host << std::endl;
    std::cout << "port: " << port << std::endl;
    std::cout << "path: " << path << std::endl;
    std::cout << "query: " << query_string << std::endl;
    std::cout << "fragment identifier: " << fragment_identifier << std::endl
              << std::endl;
}

int main()
{
    std::string uri1 = "example.com:8080/path/to/resource?param1=value1&param2=value2";
    parse_uri(uri1);
    std::string uri2 = "http://example.com/path/to/resource?param1=value1&param2=value2";
    parse_uri(uri2);
    std::string uri3 = "/path/to/resource";
    parse_uri(uri3);
    std::string uri4 = "/";
    parse_uri(uri4);
    std::string uri5 = "http://example.com/path/to/resource?param1=value1&param2=value2#section1";
    parse_uri(uri5);

    std::string IPv6 = "http://[::1]:8080/path/to/resource";
    parse_uri(IPv6);
    return 0;
}