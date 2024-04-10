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
int main(void)
{
    int     server_socket;
    int     request_socket = -1;
    const char  *server_message = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 130\n\n<html>\n <head>\n </head>\n <body>\nHey Wonderfull webserv wonderteam <3\n </body>\n</html>";
    
    // creating server socket fd
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Error creating server_socket with socket()");
        exit(EXIT_FAILURE);
    }

    printf("server_socket = %i\n", server_socket);
    // set socket option to make socket reusable immediately after closing
    int opt = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // fill in sockaddr struct
    struct sockaddr_in address;
    socklen_t addr_len = sizeof(address);
    address.sin_family = AF_INET; // address family, AF_INET when using IP networking
    address.sin_port = htons(8080); // port number (transport address), htons converts short integer (Port) to network representation
    address.sin_addr.s_addr = INADDR_ANY; // address for this socket (usually machine's IP address), or INADDR_ANY if OS can decide

    // set address to zero so struct can be cast to another type (sockaddr instead of sockaddr_in) for bind() and accept()
    memset(address.sin_zero, '\0', sizeof address.sin_zero);

    // binding server socket to port
    // printf("bind = %i\n", bind(server_socket, (struct sockaddr *)&address, addr_len));
    if (bind(server_socket, (struct sockaddr *)&address, addr_len)) // ! change to getaddrinfo?
    {
        perror("Error binding server socket to port with bind()");
        exit(EXIT_FAILURE);
    }

    // let server socket listen to incoming requests
    if (listen(server_socket, 4) < 0)
    {
        perror("Error in letting server socket listen to incoming requests with listen()");
        exit(EXIT_FAILURE);
    }

    signal(SIGINT, sigHandler);
    while (1)
    {
        printf("\n------------ Waiting for new connection ------------\n\n");
        // create new socket for the first connection request on the queue of pending connections (set up in listen)
        request_socket = accept(server_socket, (struct sockaddr *)&address, &addr_len);
        if (request_socket < 0)
        {
            perror("Error in accepting incoming requests with accept()");
            exit(EXIT_FAILURE);
        }
    
        // read incoming request and store it in buffer
        char buffer[1024] = {0}; // ! other way to clear the buffer?
        long end = read(request_socket, buffer, 1024);
        if (end < 0)
        {
            perror("Error reading from request socket with read()");
            exit(EXIT_FAILURE);
        }
        printf("%s\n", buffer);

        // write server_message response to the request_socket fd
        if (write(request_socket, server_message, strlen(server_message)) < 0)
        {
            perror("Error write server response to request socket with write()");
            exit(EXIT_FAILURE);
        }
        printf("\n------------- Server response sent -------------\n\n");
    
        // close request_socket after communication
        close(request_socket);
    }
    return (0);
}