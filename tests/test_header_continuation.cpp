#include <string>
#include <iostream>
#include <map>

void trim_lws(std::string &s)
{
    auto it = s.begin();
    while (*it == ' ' || *it == '\t')
        s.erase(it);
    it = s.end();
    while (*--it == ' ' || *it == '\t')
        s.erase(it);
}

void header_continuation(std::string &header_line)
{
    std::string next_line;
    next_line = "      ...valueContinues   \t   ";
    auto it = next_line.begin();
    if (*it == ' ' || *it == '\t')
    {
        trim_lws(next_line);
        header_line += next_line;
    }
}

void add_header(std::string header_line, std::map<std::string, std::string> &map)
{
    header_continuation(header_line);
    size_t semicolon = header_line.find_first_of(':');
    if (semicolon == std::string::npos)
        std::cerr << "400 Bad Request" << std::endl;
    std::string headerName = header_line.substr(0, semicolon);
    std::string headerValue = header_line.substr(semicolon + 1, header_line.size() - semicolon);
    trim_lws(headerName);
    trim_lws(headerValue);
    map[headerName] = headerValue;
}

int main(void)
{
    std::string s;
    std::map<std::string, std::string> map;
    s = "headerName : headerValue..";
    add_header(s, map);
    for (auto it : map)
        std::cout << it.first << " = " << it.second << "!" << std::endl;
}
