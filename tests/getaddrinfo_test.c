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

#define MAX_EVENTS 10

void sigHandler(int sig)
{
    if (sig == SIGINT)
    {
        printf("SIGINT\n");
        close(3); // assuming server_socket = 3 (Need to fix that later)
        exit(0);
    }
}

void getservinfo(struct addrinfo *servinfo)
{
    struct addrinfo hints;
    struct addrinfo *res;

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
    servinfo->ai_flags = res->ai_flags;
    servinfo->ai_family = res->ai_family;
    servinfo->ai_socktype = res->ai_socktype;
    servinfo->ai_protocol = res->ai_protocol;
    servinfo->ai_addrlen = res->ai_addrlen;
    servinfo->ai_canonname = res->ai_canonname;
    servinfo->ai_addr = res->ai_addr; // ai_addr is een structure. moeten de onderdelen daarvan ook worden gekopieerd?
    servinfo->ai_next = NULL;
    freeaddrinfo(res); // is nu niets dat in servinfo zit gefreed? (Is servinfo een DEEP copy van res?)
}

void add_fd_to_epoll_interest_list(int epoll_fd, int fd)
{
    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = fd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &event))
    {
        perror("Error adding fd to epoll interest list with epoll_ctl()");
        close(epoll_fd); // en word zo ook de epoll interest list (met de client sockets) geclosed?
        close(fd);
        // close(server_socket)
        exit(EXIT_FAILURE);
    }
}

int main(void)
{
    int server_socket;
    int request_socket = -1;
    const char *server_message = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 124\n\n<html>\n <head>\n </head>\n <body>\nHey Wonderfull webserv wonderteam <3\n </body>\n</html>";

    if ((server_socket = socket(PF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Error creating server_socket with socket()");
        exit(EXIT_FAILURE);
    }
    int opt = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    struct addrinfo servinfo;
    getservinfo(&servinfo);
    if (bind(server_socket, servinfo.ai_addr, servinfo.ai_addrlen) || listen(server_socket, 4) < 0)
    {
        perror("Error staring server");
        exit(EXIT_FAILURE);
    }
    signal(SIGINT, sigHandler);
    int epoll_fd = epoll_create1(0);
    if (epoll_fd == -1)
    {
        perror("Error creating epoll with epoll_create1()");
        return (EXIT_FAILURE);
    }
    add_fd_to_epoll_interest_list(epoll_fd, server_socket);
    struct epoll_event events[MAX_EVENTS];
    int number_of_events;
    while ((number_of_events = epoll_wait(epoll_fd, events, MAX_EVENTS, -1)) > 0)
    {
        // Loop over all triggered events
        for (int i = 0; i < number_of_events; i++)
        {
            // If the event was triggered on the server socket, a new connection is available.
            if (events[i].data.fd == server_socket)
            {
                int new_client = accept(server_socket, servinfo.ai_addr, &servinfo.ai_addrlen);
                add_fd_to_epoll_interest_list(epoll_fd, new_client);
                printf("ACCEPTED NEW CONNECTION!\n");
            }
            else
            {
                char buffer[1024];
                int end = read(events[i].data.fd, buffer, 1024);
                buffer[end] = '\0';
                printf("%s\n", buffer);
                write(events[i].data.fd, server_message, strlen(server_message));
                printf("WROTE TO CONNECTION!\n");
            }
        }
    }
    close(server_socket);
    close(epoll_fd); // dit moet ook gebeuren als je met CTRL-C exit. en word zo ook de epoll interest list (met de client sockets) geclosed?
    return (EXIT_SUCCESS);
}