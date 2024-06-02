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

Client::Client(Server& server) : _server(server), _readyFor(READ), _request(nullptr), _startTime(std::chrono::steady_clock::now())
{
	std::cout << "Client constructor called" << std::endl;
	if ((_socketFD = accept(server.getSocketFD(), server.getServerInfo()->ai_addr, &server.getServerInfo()->ai_addrlen)) < 0)
		std::cout << "Error: failed to accept new connection (Client class constructor) with accept()" << std::endl; // ! change into throw_error?
	// give reference of Server to constructor of Client so we access Epoll instance through reference
	if (server.getEpollReference().addFDToEpoll(this, EPOLLIN | EPOLLOUT, _socketFD) < 0)
	{
		close(_socketFD); // close server socket
		throw std::runtime_error("Error adding fd to epoll");
	}
}

Client::~Client()
{
	std::cout << "Client destructor called" << std::endl;
	close(_socketFD); // close client socket
}

void Client::setReadyForFlag(int readyFor)
{
	_readyFor = readyFor;
}

void Client::setResponse(char *response)
{
	_response = response;
}

int Client::getReadyForFlag() const
{
	return _readyFor;
}

Server& Client::getServer() const
{
	return _server;
}

std::string Client::getResponse() const
{
	return _response;
}

bool Client::headersComplete()
{
	return (_fullBuffer.find("\r\n\r\n") != std::string::npos);
}

bool Client::requestIsComplete()
{
	if(_request->getTransferEncoding() == CHUNKED) // check if this works? how?
	{
		if (_fullBuffer.find("\r\n0\r\n\r\n") != std::string::npos)
		{
			std::cout << RED BOLD "Request is complete\n" RESET;	
			return true;
		}
		std::cout << RED BOLD "Request is not complete\n" RESET;
		return false;
	}
	if (_fullBuffer.size() - (_fullBuffer.find("\r\n\r\n") + strlen("\r\n\r\n")) >= _request->getContentLength())
		return true;
	else
		return false;
}

/*
Recv() is use to receive data from a socket
*/

bool Client::requestHasTimedOut() // initialize starttime at first recv or when client is constructed?
{
	auto now = std::chrono::steady_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::seconds>(now - _startTime).count();

	if (duration > TIMEOUT)
		return (true);
	return (false);
}

int Client::clientReceives()
{

	try
	{
		if (requestHasTimedOut())
			throw_error("Request has timed out", REQUEST_TIMEOUT);
	
		char buffer[BUFSIZ]{};
		ssize_t bytes_received{};

		bytes_received = recv(_socketFD, buffer, BUFSIZ - 1, 0);
		// TO TEST:
		// std::cout << "Receiving data from client socket. Bytes received: " << bytes_received << std::endl;
		buffer[bytes_received] = '\0'; // good for safety
		// std::cout << "bytes_received = " << bytes_received << std::endl;
		// END OF TEST
	
		if (bytes_received < 0)
		{
			throw_error("Receiving data recv failure", INTERNAL_SERVER_ERROR);
			return (ERROR);
		}
		else 
		{
			_fullBuffer.append(buffer, bytes_received);
			// sleep(2); // for testing timeout
			if (bytes_received == 0 && _fullBuffer.size() == 0)
			{
				throw_error("Nothing received", BAD_REQUEST);
				return (ERROR);
			}
			if (_fullBuffer.size() > getServer().getClientMaxBodySize())
			{
				if (headersComplete())
					throw_error("Received buffer bigger than client max body size", REQUEST_TOO_LARGE);
				throw_error("Received headers bigger than client max body size", URI_TOO_LARGE);
			}
			if (headersComplete() && _request == nullptr)
				_request = std::make_unique<Request>(_fullBuffer);
			if (_request != nullptr && (bytes_received == 0 || requestIsComplete()))
			{
				_request->parseBody(_fullBuffer, _server.getClientMaxBodySize());
				// std::cout << *_request << std::endl;
				std::unique_ptr<RequestHandler> requestHandler = std::make_unique<RequestHandler>(*_request, *this);
				if (!requestHandler->isCGI())
				{
					std::unique_ptr <Response> response = std::make_unique<Response>(*requestHandler);
					_response = response->getResponseMessage();
					_readyFor = WRITE;
					// std::cout << "_readyFor flag == WRITE in request complete\n";
				}
			}
		}
	}
	catch (const e_status& statusCode)
	{
		std::unique_ptr<ErrorHandler> errorHandler = std::make_unique<ErrorHandler>(statusCode, _server);
		std::unique_ptr <Response> response = std::make_unique<Response>(*errorHandler);
		_response = response->getResponseMessage();
		_readyFor = WRITE;
		// std::cout << "_readyFor flag == WRITE in catch\n";
		std::cout << "REPSONSE = \n" << *response << std::endl;
	}
	return (SUCCESS);
}

void Client::clientWrites()
{
	ssize_t send_return{};
	send_return = send(_socketFD, _response.c_str(), _response.length(), 0);
	// TODO: add check for:
	// if (send_return <= 0)
	// remove client from epoll!

	// TO TEST:
    // std::cout << "WROTE TO CONNECTION!" << std::endl;
	// std::cout << "Send data to client socket. Bytes sent: " << send_return << std::endl;
}
