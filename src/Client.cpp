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


static bool request_is_complete(std::string fullBuffer)
{
	if (fullBuffer.find("Transfer-Encoding: chunked")) // if chunked request
	{
		if (fullBuffer.find("\r\n0\r\n\r\n")) // if end of chunked request
			return true;
		return false;
	}
	// if (content length header)
	// body size from header end "\r\n\r\n"
	// check if content length == body size
	int pos = fullBuffer.find("\r\n\r\n");
	if (pos == std::string::npos)
		return false;
	int pos2 = fullBuffer.find("Content-Length");
	std::string content_length_string = fullBuffer.substr(pos2 + 16);
	unsigned long long content_length = std::stoull(content_length_string);
	if (sizeof(fullBuffer) - (pos + 4) >= content_length)
		return true;
	return false;
}

/*
Recv() is use to receive data from a socket
*/

void Client::clientReceives()
{
	char buffer[BUFSIZ]{}; // buffer to hold client data, BUFSIZ = 8192?
	ssize_t bytes_received{};

	bytes_received = recv(_socketFD, buffer, BUFSIZ - 1, 0);
	// TO TEST:
	std::cout << "Receiving data from client socket. Bytes received: " << bytes_received << std::endl;
    buffer[bytes_received] = '\0'; // it this necessary to do ourselves?
	std::cout << "bytes_received = " << bytes_received << std::endl;
	// END OF TEST

	try
	{
		if (bytes_received < 0)
			throw_error("Receiving data recv failure", INTERNAL_SERVER_ERROR); // ! remove client from epoll
		else if (bytes_received == 0)
			_readyFor = WRITE;
		else
		{
			_fullBuffer.append(buffer, bytes_received);
			if (request_is_complete(_fullBuffer)) // check later for body bytes read == content_length
			{
				std::unique_ptr<Request> request = std::make_unique<Request>(_fullBuffer);
				std::cout << *request << std::endl; // for for debugging purposes
				std::unique_ptr<RequestHandler> requestHandler = std::make_unique<RequestHandler>(*request, _server);
				if (!requestHandler->isCGI())
				{
					_response = std::make_unique<Response>(*requestHandler);
					// std::cout << *_response << std::endl; // for for debugging purposes
					_readyFor = WRITE;
					std::cout << "_readyFor flag == WRITE in request complete\n";
				}
			}
		}
	}
	catch (const e_status& statusCode)
	{
		std::unique_ptr<ErrorHandler> errorHandler = std::make_unique<ErrorHandler>(statusCode, _server);
		_response = std::make_unique<Response>(*errorHandler);
		// std::cout << "statusCode: " << statusCode << std::endl; //for debugging purposes
		// std::cout << *_response << std::endl; // for for debugging purposes
		_readyFor = WRITE;
		std::cout << "_readyFor flag == WRITE in catch\n";
	}
}

void Client::clientWrites()
{
	ssize_t send_return{};
	send_return = send(_socketFD, _response->getResponseMessage().c_str(), _response->getResponseMessage().length(), 0);
	// TODO: add check for:
	// if (send_return <= 0)
	// remove client from epoll!

	// TO TEST:
    std::cout << "WROTE TO CONNECTION!" << std::endl;
	std::cout << "Send data to client socket. Bytes sent: " << send_return << std::endl;

}
