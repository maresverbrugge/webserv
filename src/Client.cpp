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

# include "Epoll.hpp"
# include "Client.hpp"
# include "ServerPool.hpp"

Client::Client(const Server& server) : _server(server), _readyFor(READ), _response(nullptr)
{
	std::cout << "Client constructor called" << std::endl;
	if ((_socketFD = accept(server.getSocketFD(), server.getServerInfo()->ai_addr, &server.getServerInfo()->ai_addrlen)) < 0)
		std::cout << "Error: failed to accept new connection (Client class constructor) with accept()" << std::endl;
	// give reference of Server to constructor of Client so we access Epoll instance through reference
	if (server.getEpollReference().addFDToEpoll(this, EPOLLIN | EPOLLOUT, _socketFD) < 0)
	{
		close(_socketFD); // close server socket
		throw std::runtime_error("Error adding fd to epoll");
	}
}

Client::~Client()
{
	close(_socketFD); // close client socket
	std::cout << "Client destructor called" << std::endl;
}

void Client::setReadyForFlag(int readyFor)
{
	_readyFor = readyFor;
}

int Client::getReadyForFlag() const
{
	return _readyFor;
}

/*
Recv() is use to receive data from a socket
*/
void Client::clientReceives()
{
	char buffer[BUFSIZ]{}; // buffer to hold client data, BUFSIZ = 8192?
	ssize_t recv_return{};

	recv_return = recv(_socketFD, buffer, BUFSIZ - 1, 0);

	// TO TEST:
	std::cout << "Receiving data from client socket. Bytes received: " << recv_return << std::endl;
    buffer[recv_return] = '\0'; // it this necessary to do ourselves?
	std::cout << "recv_return = " << recv_return << std::endl;
	// END OF TEST

	try
	{
		std::unique_ptr<Request> request = std::make_unique<Request>(buffer, recv_return);
		std::cout << *request << std::endl; // for for debugging purposes
		std::unique_ptr<RequestHandler> requestHandler = std::make_unique<RequestHandler>(*request, _server);
		if (!requestHandler->isCGI())
		{
			_response = std::make_unique<Response>(*requestHandler);
			// std::cout << *_response << std::endl; // for for debugging purposes
		}
	}
	catch (const e_status& statusCode)
	{
		std::unique_ptr<ErrorHandler> errorHandler = std::make_unique<ErrorHandler>(statusCode, _server);
		_response = std::make_unique<Response>(*errorHandler);
		// std::cout << "statusCode: " << statusCode << std::endl; //for debugging purposes
		// std::cout << *_response << std::endl; // for for debugging purposes
	}
	// TODO:
	// clear buffer before recv?

	// TODO:
	// add check for:
	// if (recv_return <= 0)
	// remove client from epoll!

	// TODO:
	// call parse request
	// call process request
	// if no errors: change flag to WRITE
	_readyFor = WRITE;
	std::cout << "_readyFor flag == WRITE\n";
}


void Client::clientWrites()
{
	// TO TEST:
	// std::string message = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 124\n\n<html>\n <head>\n </head>\n <body>\nHey Wonderfull webserv wonderteam <3 \n _socketFD van deze client = " + std::to_string(_socketFD) + " \n </body>\n</html>\n";
	// const char* message_ready = message.c_str();
	// std::cout << "Message_ready in clientWrites = " << message_ready << std::endl;
	//  "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 124\n\n<html>\n <head>\n </head>\n <body>\nHey Wonderfull webserv wonderteam <3\n </body>\n</html>\n";

	// write(_socketFD, message_ready, strlen(message_ready));
	ssize_t send_return{};
	send_return = send(_socketFD, _response->getResponseMessage().c_str(), _response->getResponseMessage().length(), 0);
    std::cout << "WROTE TO CONNECTION!" << std::endl;
	// TO TEST:
	std::cout << "Send data to client socket. Bytes sent: " << send_return << std::endl;

	// TODO: add check for:
	// if (send_return < 0)
	// remove client from epoll!
}
