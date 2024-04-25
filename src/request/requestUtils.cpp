#include "Request.hpp"

bool is_valid_method(std::string &method)
{
    return (method == "GET" || method == "DELETE" || method == "POST");
}

bool is_http_protocol(std::string &protocol)
{
    if (protocol.compare(0, 5, "HTTP/") == 0)
        return (true);
    return (false);
}

bool is_http1_1_protocol(std::string &protocol)
{
    return (protocol == "HTTP/1.1");
}

void trim_lws(std::string &s)
{
    auto it = s.begin();
    while (*it == ' ' || *it == '\t')
        s.erase(it);
    it = s.end();
    while (*--it == ' ' || *it == '\t')
        s.erase(it);
}

void trim_cr(std::string &s)
{
    auto it = s.end();
    if (*--it == '\r')
        s.erase(it);
}

void str_to_lower(std::string &s)
{
    for (char& c : s)
    {
        c = tolower(c);
    }
}