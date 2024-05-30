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

Client::Client(const Server& server) : _server(server), _readyFor(READ), _request(nullptr), _response(nullptr), _fullBuffer("")
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

Response& Client::getResponse() const
{
	return *_response;
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
			return true;
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

void Client::clientReceives()
{
	char buffer[BUFSIZ]{};
	ssize_t bytes_received{};

	bytes_received = recv(_socketFD, buffer, BUFSIZ - 1, 0);
	// TO TEST:
	// std::cout << "Receiving data from client socket. Bytes received: " << bytes_received << std::endl;
    buffer[bytes_received] = '\0'; // good for safety
	// std::cout << "bytes_received = " << bytes_received << std::endl;
	// END OF TEST

	try
	{
		if (bytes_received < 0)
			throw_error("Receiving data recv failure", INTERNAL_SERVER_ERROR); // ! remove client from epoll
		else 
		{
			_fullBuffer.append(buffer, bytes_received);
			if (headersComplete() && _request == nullptr)
				_request = std::make_unique<Request>(_fullBuffer);
			if (_request != nullptr && (bytes_received == 0 || requestIsComplete()))
			{
				_request->parseBody(_fullBuffer);
				std::cout << *_request << std::endl;
				std::unique_ptr<RequestHandler> requestHandler = std::make_unique<RequestHandler>(*_request, _server);
				if (!requestHandler->isCGI())
				{
					_response = std::make_unique<Response>(*requestHandler);
					_readyFor = WRITE;
					std::cout << *_response << std::endl;
					// std::cout << "_readyFor flag == WRITE in request complete\n";
				}
			}
		}
	}
	catch (const e_status& statusCode)
	{
		std::unique_ptr<ErrorHandler> errorHandler = std::make_unique<ErrorHandler>(statusCode, _server);
		_response = std::make_unique<Response>(*errorHandler);
		_readyFor = WRITE;
		// std::cout << "_readyFor flag == WRITE in catch\n";
		std::cout << "REPSONSE = \n" << *_response << std::endl; 
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
    // std::cout << "WROTE TO CONNECTION!" << std::endl;
	// std::cout << "Send data to client socket. Bytes sent: " << send_return << std::endl;
}
