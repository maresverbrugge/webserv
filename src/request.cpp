/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   request.cpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: fhuisman <fhuisman@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/18 13:12:07 by fhuisman      #+#    #+#                 */
/*   Updated: 2024/04/18 17:36:27 by fhuisman      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/request.hpp"
#include <iostream> //maybe remove this later

#include <sys/socket.h> // socket(), bind(), listen(), accept()
#include <netinet/in.h> // sockaddr_in struct
#include <unistd.h>     // read()
#include <stdio.h>      // printf
#include <string.h>     // strlen()
#include <signal.h>     // signalhandler
#include <stdlib.h>

void sigHandler(int sig)
{
    if (sig == SIGINT)
    {
        printf("SIGINT\n");
        close(3);
        exit(0);
    }
}

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

void parse_request_line(std::string &request_line, Request *request)
{
    std::string method;
    std::string uri;
    std::string protocol;

    size_t sp1 = request_line.find(' ');
    size_t sp2 = request_line.find(' ', sp1 + 1);
    method = request_line.substr(0, sp1);
    uri = request_line.substr(sp1 + 1, sp2 - sp1 - 1);
    protocol = request_line.substr(sp2 + 1, (request_line.size() - 1) - (sp2 + 1));
    if (sp1 == std::string::npos || sp2 == std::string::npos || !is_valid_method(method) || !is_http_protocol(protocol))
        std::cerr << "400 Bad Request" << std::endl;
    if (!is_http1_1_protocol(protocol))
        std::cerr << "505 HTTP Version Not Supported";
    request->setMethod(method);
    request->setUri(uri);
    request->setProtocol(protocol);
}

Request::Request(std::string const request)
{
    std::string line;

    std::cout << "Request constructor called" << std::endl;
    _request_stream << request;
    std::getline(_request_stream, line);
    parse_request_line(line, this);
}

Request::~Request()
{
    std::cout << "Request destructor called" << std::endl;
}

std::string Request::getMethod()
{
    return (_method);
}

std::string Request::getUri()
{
    return (_uri);
}

std::string Request::getProtocol()
{
    return (_protocol);
}

std::string Request::getPath()
{
    return (_path);
}

std::string Request::getQuery()
{
    return (_query);
}

std::string Request::getFragmentIdentifier()
{
    return (_fragmentIdentifier);
}

std::map<std::string, std::string> Request::getHeaders()
{
    return (_headers);
}

std::string Request::getBody()
{
    return (_body);
}

void Request::setMethod(std::string method)
{
    _method = method;
}
void Request::setUri(std::string uri)
{
    _uri = uri;
}

void Request::setProtocol(std::string protocol)
{
    _protocol = protocol;
}

void Request::setPath(std::string path)
{
    _path = path;
}

void Request::setQuery(std::string query)
{
    _query = query;
}

void Request::setFragmentIdentifier(std::string fragmentIdentifier)
{
    _fragmentIdentifier = fragmentIdentifier;
}

void Request::setHeader(std::string headerName, std::string headerValue)
{
    auto it = _headers.find(headerName);
    if (it != _headers.end())
        it->second += ", " + headerValue;
    else
        _headers[headerName] = headerValue;
}

void Request::setBody(std::string body)
{
    _body = body;
}

int main(void)
{
    int server_socket;
    int request_socket = -1;
    const char *server_message = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 130\n\n<html>\n <head>\n </head>\n <body>\nHey Wonderfull webserv wonderteam <3\n </body>\n</html>";

    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Error creating server_socket with socket()");
        exit(EXIT_FAILURE);
    }

    printf("server_socket = %i\n", server_socket);
    int opt = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    struct sockaddr_in address;
    socklen_t addr_len = sizeof(address);
    address.sin_family = AF_INET;
    address.sin_port = htons(8080);
    address.sin_addr.s_addr = INADDR_ANY;
    memset(address.sin_zero, '\0', sizeof address.sin_zero);
    if (bind(server_socket, (struct sockaddr *)&address, addr_len)) // ! change to getaddrinfo?
    {
        perror("Error binding server socket to port with bind()");
        exit(EXIT_FAILURE);
    }
    if (listen(server_socket, 4) < 0)
    {
        perror("Error in letting server socket listen to incoming requests with listen()");
        exit(EXIT_FAILURE);
    }
    signal(SIGINT, sigHandler);
    while (1)
    {
        printf("\n------------ Waiting for new connection ------------\n\n");
        request_socket = accept(server_socket, (struct sockaddr *)&address, &addr_len);
        if (request_socket < 0)
        {
            perror("Error in accepting incoming requests with accept()");
            exit(EXIT_FAILURE);
        }
        char buffer[1024] = {0};
        long end = read(request_socket, buffer, 1024);
        if (end < 0)
        {
            perror("Error reading from request socket with read()");
            exit(EXIT_FAILURE);
        }
        Request request(buffer);
        std::cout << "Method: \"" << request.getMethod() << "\"" << std::endl;
        std::cout << "URI: \"" << request.getUri() << "\"" << std::endl;
        std::cout << "Protocol: \"" << request.getProtocol() << "\"" << std::endl;
        if (write(request_socket, server_message, strlen(server_message)) < 0)
        {
            perror("Error write server response to request socket with write()");
            exit(EXIT_FAILURE);
        }
        printf("\n------------- Server response sent -------------\n\n");
        close(request_socket);
    }
    return (0);
}