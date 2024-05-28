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

Client::Client(const Server& server) : _server(server), _readyFor(READ), _response(nullptr), _fullBuffer(""), _contentLength(NOT_INITIALIZED)
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

static long long get_content_length(const std::string& buffer)
{
	std::size_t content_length_pos = buffer.find("Content-Length");
	if (content_length_pos == std::string::npos) // not a POST request
		return 0;

	try
	{
		std::string content_length = buffer.substr(content_length_pos + strlen("Content-Length:"));
		return std::stoll(content_length);
	}
	catch (const std::exception& exception)
	{
		throw_error(exception.what(), BAD_REQUEST);
		return NOT_INITIALIZED;
	}
}

bool Client::requestIsComplete()
{
	if (_fullBuffer.find("Transfer-Encoding: chunked") != std::string::npos) // check if this works? how?
	{
		if (_fullBuffer.find("\r\n0\r\n\r\n") != std::string::npos)
			return true;
		return false;
	}

	std::size_t header_end = _fullBuffer.find("\r\n\r\n");

	if (header_end == std::string::npos)
		return false;
	else if (_contentLength == NOT_INITIALIZED)
		_contentLength = get_content_length(_fullBuffer);
	
	if (_contentLength == 0)
		return true;
	else if (_fullBuffer.size() - (header_end + strlen("\r\n\r\n")) >= (unsigned long)_contentLength)
		return true;
	else
		return false;
}

/*
Recv() is use to receive data from a socket
*/

void Client::clientReceives()
{
	char buffer[BUFSIZ]{};
	ssize_t bytes_received{};

	bytes_received = recv(_socketFD, buffer, BUFSIZ - 1, 0);
	// TO TEST:
	std::cout << "Receiving data from client socket. Bytes received: " << bytes_received << std::endl;
    buffer[bytes_received] = '\0'; // good for safety
	std::cout << "bytes_received = " << bytes_received << std::endl;
	// END OF TEST

	try
	{
		if (bytes_received < 0)
			throw_error("Receiving data recv failure", INTERNAL_SERVER_ERROR); // ! remove client from epoll
		else 
		{
			_fullBuffer.append(buffer, bytes_received);
			if (bytes_received == 0 || requestIsComplete())
			{
				std::unique_ptr<Request> request = std::make_unique<Request>(_fullBuffer);
				std::cout << *request << std::endl;
				std::unique_ptr<RequestHandler> requestHandler = std::make_unique<RequestHandler>(*request, _server);
				if (!requestHandler->isCGI())
				{
					_response = std::make_unique<Response>(*requestHandler);
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
