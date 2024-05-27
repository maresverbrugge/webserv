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


static bool request_is_complete(std::string fullBuffer) // get requests?
{
	std::cout << "CHECKING IF REQUEST IS COMPLETE\n";
	if (fullBuffer.find("Transfer-Encoding: chunked") != std::string::npos) // if chunked request
	{
		if (fullBuffer.find("\r\n0\r\n\r\n") != std::string::npos) // if end of chunked request
		{
			std::cout << "CHUNKED REQUEST IS COMPLETE\n";
			return true;
		}
		std::cout << "CHUNKED REQUEST IS NOT COMPLETE\n";
		return false;
	}
	else
	{
		std::size_t pos = fullBuffer.find("\r\n\r\n");
		if (pos == std::string::npos)
		{
			std::cout << "Header not found\n";
			return false;
		}
		std::size_t pos2 = fullBuffer.find("Content-Length");
		if (pos2 == std::string::npos)
		{
			return true;
		}
		try
		{
			std::string content_length_string = fullBuffer.substr(pos2 + 16);
			unsigned long long content_length = std::stoull(content_length_string);
			std::cout << "content length " << content_length << std::endl;
			std::cout << "size of fullBuffer " << fullBuffer.size() << std::endl;
			if (fullBuffer.size() - (pos + 4) >= content_length)
				return true;
			return false;
		}
		catch(const std::exception& e)
		{
			throw_error(e.what(), BAD_REQUEST);
			return false;
		}
	}
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
		else
		{
			_fullBuffer.append(buffer, bytes_received);
			if (bytes_received == 0 || request_is_complete(_fullBuffer)) // check later for body bytes read == content_length
			{
				std::unique_ptr<Request> request = std::make_unique<Request>(_fullBuffer);
				// std::cout << *request << std::endl; // for for debugging purposes
				std::cout << "GONNA HANDLE DIS BITCH\n";
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
