#include <sys/types.h>  // sys/socket.h depends on sys/types
#include <sys/socket.h> // socket(), bind(), listen(), accept()
#include <netinet/in.h> // sockaddr_in struct
#include <unistd.h>     // read()
#include <stdio.h>      // printf
#include <string.h>     // strlen()
#include <signal.h>     // signalhandler
#include <netdb.h>      // getaddrinfo()
#include <sys/epoll.h>  // epoll
#include <stdlib.h>     // printf()
#include <iostream>
#include <vector>

#define MAX_EVENTS 10

int g_state = 1;

void getservinfo(struct addrinfo *servinfo)
{
    struct addrinfo hints;
    struct addrinfo *res;
    struct addrinfo *p;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;     // AF_UNSPEC for IPv4 and IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
    hints.ai_flags = AI_PASSIVE;     // AI_PASSIVE fill in my IP for me

    int status;
    if ((status = getaddrinfo(NULL, "8080", &hints, &res)) != 0)
    {
        perror(gai_strerror(status));
        exit(EXIT_FAILURE);
    }
    for (p = res; p != NULL; p = p->ai_next)
    {
        if (p->ai_family == AF_INET || p->ai_family == AF_INET6)
            break;
    }
    memcpy(servinfo, p, sizeof(struct addrinfo));                // making copy of the result
    memcpy(servinfo->ai_addr, p->ai_addr, servinfo->ai_addrlen); // making sure its a DEEP copy
    freeaddrinfo(res);
}

void epoll_modify_interest_list(int epoll_fd, int fd, int action)
{
    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = fd;
    if (epoll_ctl(epoll_fd, action, fd, &event) == -1)
    {
        g_state = 0;
        perror("Error modifying epoll interest list with epoll_ctl()");
    }
}

void siginthandler(int signum)
{
    if (signum == SIGINT)
    {
        g_state = 0;
    }
}

int start_server(struct addrinfo *servinfo)
{
    int request_socket = -1;
    int server_socket;

    signal(SIGINT, siginthandler);
    if ((server_socket = socket(PF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Error creating server_socket with socket()");
        exit(EXIT_FAILURE);
    }
    int opt = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    if (bind(server_socket, servinfo->ai_addr, servinfo->ai_addrlen) || listen(server_socket, 4) < 0)
    {
        close(server_socket);
        perror("Error staring server");
        exit(EXIT_FAILURE);
    }
    return (server_socket);
}

int start_epoll(int server_socket)
{
    int epoll_fd = epoll_create1(0);
    if (epoll_fd == -1)
    {
        close(server_socket);
        perror("Error creating epoll with epoll_create1()");
        return (EXIT_FAILURE);
    }
    return (epoll_fd);
}

void write_to_connection(int fd)
{
    const char *server_message = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 124\n\n<html>\n <head>\n </head>\n <body>\nHey Wonderfull webserv wonderteam <3\n </body>\n</html>\n";

    char buffer[1024];
    int end = read(fd, buffer, 1024);
    std::cout << "reading from socket. bytes read: " << end << std::endl;
    buffer[end] = '\0';
    std::cout << buffer << std::endl;
    write(fd, server_message, strlen(server_message));
    std::cout << "WROTE TO CONNECTION!" << std::endl;
}

void accept_new_connection(int server_socket, int epoll_fd, struct addrinfo *servinfo)
{
    int new_client = accept(server_socket, servinfo->ai_addr, &servinfo->ai_addrlen);
    if (new_client == -1)
    {
        perror("Error acepting incomming connection");
        g_state = 0;
        return;
    }
    epoll_modify_interest_list(epoll_fd, new_client, EPOLL_CTL_ADD);
    std::cout << "ACCEPTED NEW CONNECTION!" << std::endl;
}

int main(void)
{
    struct addrinfo servinfo;
    getservinfo(&servinfo);

    int server_socket = start_server(&servinfo);
    int epoll_fd = start_epoll(epoll_fd);
    epoll_modify_interest_list(epoll_fd, server_socket, EPOLL_CTL_ADD);
    struct epoll_event events[MAX_EVENTS];
    int number_of_events;
    while (g_state && (number_of_events = epoll_wait(epoll_fd, events, MAX_EVENTS, -1)) >= 0)
    {
        for (int i = 0; i < number_of_events; i++)
        {
            if (events[i].data.fd == server_socket)
                accept_new_connection(server_socket, epoll_fd, &servinfo);
            else
            {
                write_to_connection(events[i].data.fd);
                epoll_modify_interest_list(epoll_fd, events[i].data.fd, EPOLL_CTL_DEL);
                close(events[i].data.fd);
            }
        }
    }
    close(epoll_fd);
    close(server_socket);
    std::cout << "Thank you for choosing Wonderfull Webserv Wonderteam" << std::endl;
    return (EXIT_SUCCESS);
}
