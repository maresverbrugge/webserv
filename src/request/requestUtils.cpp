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

static int hex_to_int(char hex)
{
    int x;
    if (hex >= '0' && hex <= '9')
        x = hex - '0';
    else if (hex >= 'A' && hex <= 'F')
        x = hex - 'A' + 10;
    else if (hex >= 'a' && hex <= 'f')
        x = hex - 'a' + 10;
    else
        throw (BAD_REQUEST);
    return (x);
}

std::string decodePercentEncodedString(std::string& s)
{
    std::string result;
    for (size_t i = 0; i < s.length(); i++)
    {
        if (s[i] == '%')
        {
            if (i + 2 < s.length())
            {
                result += (16 * hex_to_int(s[i + 1])) + hex_to_int(s[i + 2]);
                i += 2;
            }
            else
                throw (BAD_REQUEST);
        }
        else
            result += s[i];
    }
    return (result);
}