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

#include "Server.hpp"
#include "Epoll.hpp"
#include "RequestHandler.hpp"
#include "ErrorHandler.hpp"
#include "Response.hpp"

Client::Client(Server& server) : _server(server), _readyFor(READ), _request(nullptr), _cgi(nullptr), _timerStarted(false)
{
	std::cout << "Client constructor called" << std::endl;
	if ((_socketFD = accept(server.getSocketFD(), server.getServerInfo()->ai_addr, &server.getServerInfo()->ai_addrlen)) < 0)
		std::cout << "Error: failed to accept new connection (Client class constructor) with accept()" << std::endl;
	set_fd_to_non_blocking_and_cloexec(_socketFD);
	if (Epoll::getInstance().addFDToEpoll(this, EPOLLIN | EPOLLOUT | EPOLLRDHUP, _socketFD) < 0)
	{
		close(_socketFD);
		throw std::runtime_error("Error adding client FD to epoll");
	}
}

Client::~Client()
{
	std::cout << "Client destructor called" << std::endl;
}

void Client::setReadyForFlag(int readyFor)
{
	_readyFor = readyFor;
}

void Client::setResponse(char *response)
{
	_response = response;
}

void Client::newReadCGI(int read_end)
{
	_cgi = std::make_unique<CGI>(read_end, *this);
}

void Client::newWriteCGI(int write_end, char** envp, std::string script_string)
{
	_cgi = std::make_unique<CGI>(write_end, *this, envp, script_string);
}

void Client::deleteCGI()
{
	_cgi = nullptr;
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
	if(_request->getTransferEncoding() == CHUNKED)
	{
		if (_fullBuffer.find("\r\n0\r\n\r\n") != std::string::npos)
			return true;
		return false;
	}
	else if (_fullBuffer.size() - (_fullBuffer.find("\r\n\r\n") + strlen("\r\n\r\n")) >= _request->getContentLength())
		return true;
	else
		return false;
}

bool Client::requestHasTimedOut()
{
	if (!_timerStarted)
	{
		_startTime = std::chrono::steady_clock::now();
		_timerStarted = true;
		return (false);
	}
	else
	{
		auto now = std::chrono::steady_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::seconds>(now - _startTime).count();

		if (duration > TIMEOUT)
		{
			_timerStarted = false;
			return (true);
		}
		return (false);
	}
}

int Client::receiveFromClient() // ! need to write this back to void?
{

	try
	{
		if (requestHasTimedOut())
			throw StatusCodeException("Request has timed out", REQUEST_TIMEOUT);
	
		char buffer[BUFSIZ]{};
		ssize_t bytes_received{};

		bytes_received = recv(_socketFD, buffer, BUFSIZ - 1, 0);
		// TO TEST:
		// std::cout << "Receiving data from client socket. Bytes received: " << bytes_received << std::endl;
		buffer[bytes_received] = '\0';
		// std::cout << "bytes_received = " << bytes_received << std::endl;
		// END OF TEST
	
		if (bytes_received < 0)
			throw StatusCodeException("Receiving data recv failure", INTERNAL_SERVER_ERROR);
		else 
		{
			_fullBuffer.append(buffer, bytes_received);
			// if (bytes_received == 0 && _fullBuffer.size() == 0)
			// {
			// 	throw StatusCodeException("Nothing received", BAD_REQUEST);
			// 	return (ERROR);
			// }
			if (_fullBuffer.size() > getServer().getClientMaxBodySize())
			{
				if (headersComplete())
					throw StatusCodeException("Received buffer bigger than client max body size", CONTENT_TOO_LARGE);
				throw StatusCodeException("Received headers bigger than client max body size", URI_TOO_LARGE);
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
	catch (const StatusCodeException& exception)
	{
		std::cout << RED BOLD "Error: " RESET << exception.what() << std::endl;
		std::unique_ptr<ErrorHandler> errorHandler = std::make_unique<ErrorHandler>(exception.status(), _server);
		std::unique_ptr <Response> response = std::make_unique<Response>(*errorHandler);
		_response = response->getResponseMessage();
		_readyFor = WRITE;
		// std::cout << _response << std::endl;
		// std::cout << "_readyFor flag == WRITE in catch\n";
		// std::cout << "REPSONSE = \n" << *response << std::endl;
	}
	return (SUCCESS);
}

void Client::writeToClient()
{
	ssize_t send_return{};
	send_return = send(_socketFD, _response.c_str(), _response.length(), 0);
	// TODO: add check for:
	if (send_return < 0)
		std::cerr << "send() in writeToClient failed" << std::endl;

	// TO TEST:
    // std::cout << "WROTE TO CONNECTION!" << std::endl;
	// std::cout << "Send data to client socket. Bytes sent: " << send_return << std::endl;
}
