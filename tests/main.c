#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/epoll.h>

#define MAX_EVENTS 10

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
	int server_socket = socket(AF_INET, SOCK_STREAM, 0);

	// HTML website that we want to deliver
	const char *html_content = "<html>\n <head>\n </head>\n <body>\nHey Wonderfull Webserv Wonderteam <3\n </body>\n</html>";
	char message[1024];

	// Combining the response header and html website into one var message
	sprintf(message, "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: %ld\n\n%s", strlen(html_content), html_content);

	// Setting options for server socket
	int opt = 1;
	setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	struct sockaddr_in address;
	socklen_t addr_len = sizeof(address);
	address.sin_family = AF_INET;
	address.sin_port = htons(8080);
	address.sin_addr.s_addr = INADDR_ANY;

	// Start up server and signal handler
	bind(server_socket, (struct sockaddr *)&address, addr_len);
	listen(server_socket, 4);
	signal(SIGINT, sigHandler);

	// Creating a new epoll file descriptor (0 means no flags set)
	int epoll_fd = epoll_create1(0);
	if(epoll_fd == -1)
	{
		// Handle error here
		return (EXIT_FAILURE);
	}

	// Add the file descriptor server_socket to the epoll instance epoll_fd
	struct epoll_event event;
	event.events = EPOLLIN;
	event.data.fd = server_socket;
	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_socket, &event))
	{
		// Handle error here
		return (EXIT_FAILURE);
	}

	// Create an array of epoll_event structures to hold events from epoll_wait
	struct epoll_event events[MAX_EVENTS];

	// Main server loop
	while (true)
	{
		// Wait for events on the epoll file descriptor. This will block until an event is available.
		int number_of_events = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);

		// Loop over all triggered events
		for (int i = 0; i < number_of_events; i++)
		{
			// If the event was triggered on the server socket, a new connection is available.
			if (events[i].data.fd == server_socket)
			{
				// Accept the new connection
				int new_client = accept(server_socket, (struct sockaddr *)&address, &addr_len);

				// Add the new connection to the epoll file descriptor
				event.events = EPOLLIN;
				event.data.fd = new_client;
				if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, new_client, &event))
				{
					// Handle error here
					return (EXIT_FAILURE);
				}

				// For debugging
				printf("ACCEPTED NEW CONNECTION!\n");
			}
			else // If the event was not triggered on the server socket, it was triggered on a connection
			{
				// Read data from the connection
				char buffer[1024];
				int end = read(events[i].data.fd, buffer, 1024);

				// Otherwise, print the received data and send the HTTP response
				buffer[end] = '\0';
				printf("%s\n", buffer);
				write(events[i].data.fd, message, strlen(message));

				// For debugging
				printf("WROTE TO CONNECTION!\n");
			}
		}
	}

	// Close the server socket and the epoll file descriptor when the server is shutting down.
	close(server_socket);
	close(epoll_fd);
	return (EXIT_SUCCESS);
}