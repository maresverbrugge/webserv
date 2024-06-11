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
# include "Server.hpp"

Server::Server(int port, std::string host, std::vector<std::string> serverNames, std::string rootFolder, std::map<short, std::string> customErrorPages, unsigned long long clientMaxBodySize, std::vector<std::unique_ptr<Location>> locations, std::unique_ptr<Location> defaultLocation)
	: _port(port),
	  _host(host),
	  _serverNames(serverNames),
	  _rootFolder(rootFolder),
	  _customErrorPages(customErrorPages),
	  _clientMaxBodySize(clientMaxBodySize),
	  _locations(std::move(locations)),
	  _defaultLocation(std::move(defaultLocation))
{
	std::cout << "Server constructor called" << std::endl;

	struct addrinfo hints{};
	struct addrinfo *ptr{};
	int	yes = true;
	int status{};

	std::memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	if ((status = getaddrinfo(NULL, (std::to_string(_port)).c_str(), &hints, &_serverInfo)) != 0)
	{
		std::string error_message = gai_strerror(status);
		throw ServerConfigError("Getaddrinfo error on server: " + error_message);
	}
	for (ptr = _serverInfo; ptr != NULL; ptr = ptr->ai_next)
	{
		if ((_FD = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol)) < 0)
			continue;
		set_fd_to_non_blocking_and_cloexec(_FD);
		if (setsockopt(_FD, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) < 0)
		{
			close(_FD);
			continue;
		}
		if (bind(_FD, ptr->ai_addr, ptr->ai_addrlen) < 0)
		{
			close(_FD);
			continue;
		}
		break;
	}
	if (ptr == NULL)
	{
		freeaddrinfo(_serverInfo);
		throw ServerConfigError("Failed to configure server socket");
	}
	setAddrInfoSocket(ptr);

	if (listen(_FD, BACKLOG) < 0)
	{
		close(_FD);
		freeaddrinfo(_serverInfo);
		throw ServerConfigError("Server listen() failed");
	}

	if (Epoll::getInstance().addFDToEpoll(this, EPOLLIN, _FD) < 0)
	{
		close(_FD);
		freeaddrinfo(_serverInfo);
		throw ServerConfigError("Error adding FD to epoll");
	}
}

Server::~Server()
{
	std::cout << "Server destructor called" << std::endl;
	freeaddrinfo(_serverInfo);
}

void Server::setPort(int port)
{
	this->_port = port;
}

void Server::setHost(std::string host)
{
	this->_host = host;
}

void Server::addServerName(std::string serverName)
{
	this->_serverNames.push_back(serverName);
}

void Server::setRootFolder(std::string rootFolder)
{
	this->_rootFolder = rootFolder;
}

void Server::addCustomErrorPage(short errorCode, std::string errorPage)
{
	this->_customErrorPages[errorCode] = errorPage;
}

void Server::setClientMaxBodySize(unsigned long long clientMaxBodySize)
{
	this->_clientMaxBodySize = clientMaxBodySize;
}

void Server::addLocation(std::unique_ptr<Location> location)
{
	this->_locations.push_back(std::move(location));
}

void Server::createNewClientConnection()
{
	try
	{
		std::unique_ptr<Client> newClient = std::make_unique<Client>(*this);
		_connectedClients[newClient->getFD()] = std::move(newClient);
	}
	catch (std::runtime_error& exception)
	{
		std::cerr << RED BOLD "Error: " RESET << exception.what() << std::endl;
	}
}

void Server::removeClientConnection(Client* client)
{
	_connectedClients.erase(client->getFD());
}

int Server::getPort() const
{
	return this->_port;
}

std::string Server::getHost() const
{
	return this->_host;
}

std::vector<std::string> Server::getServerNames() const
{
	return this->_serverNames;
}

std::string Server::getRootFolder() const
{
	return this->_rootFolder;
}

std::map<short, std::string> Server::getCustomErrorPages() const
{
	return this->_customErrorPages;
}

unsigned long long Server::getClientMaxBodySize() const
{
	return this->_clientMaxBodySize;
}

const std::vector<std::unique_ptr<Location>>& Server::getLocations() const
{
	return this->_locations;
}

const Location& Server::getDefaultLocation() const
{
	return *this->_defaultLocation;
}

struct addrinfo* Server::getServerInfo() const
{
	return this->_serverInfo;
}

const std::map<int, std::unique_ptr<Client>>& Server::getConnectedClients() const
{
	return this->_connectedClients;
}

void Server::setAddr(void* addr)
{
	this->_addr = addr;
}

void Server::setVersionIP(std::string versionIP)
{
	this->_versionIP = versionIP;
}

void Server::setPortOfSocket(in_port_t portOfSocket)
{
	this->_portOfSocket = ntohs(portOfSocket);
}

void Server::setStrIP(struct addrinfo *ptr)
{
	inet_ntop(ptr->ai_family, _addr, _strIP, INET6_ADDRSTRLEN);
}

void Server::setAddrInfoSocket(struct addrinfo *ptr)
{
	if (ptr->ai_family == AF_INET)
	{
		struct sockaddr_in *ipv4 = (struct sockaddr_in *)ptr->ai_addr;
		setAddr(&ipv4->sin_addr);
		setVersionIP("IPv4");
		setPortOfSocket(ipv4->sin_port);
	}
	else
	{
		struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)ptr->ai_addr;
		setAddr(&ipv6->sin6_addr);
		setVersionIP("IPv6");
		setPortOfSocket(ipv6->sin6_port);
	}
	setStrIP(ptr);
}

void *Server::getAddr() const
{
	return this->_addr;
}

std::string Server::getVersionIP() const
{
	return this->_versionIP;
}

int Server::getPortOfSocket() const
{
	return this->_portOfSocket;
}

const char* Server::getstrIP() const
{
	return this->_strIP;
}

Server::ServerConfigError::ServerConfigError(const std::string& message) 
	: message_(RED BOLD "Server config error: " RESET + message) {}

const char* Server::ServerConfigError::what() const noexcept 
{
	return message_.c_str();
}

std::ostream& operator<<(std::ostream& out_stream, const Server& server)
{
	out_stream << GREEN BOLD "Server: " RESET << server.getHost() << ":" << server.getPort() << std::endl;
	out_stream << "_serverNames: ";
	for (std::string name : server.getServerNames())
		out_stream << name << " ";
	out_stream << std::endl;
	out_stream << "_rootFolder: " << server.getRootFolder() << std::endl;
	out_stream << "_customErrorPages: " << std::endl;
	const std::map<short, std::string>& customErrorPages = server.getCustomErrorPages();
	for (const std::pair<const short, std::string>& error : customErrorPages)
		out_stream << "Code " << error.first << ", Page " << error.second << std::endl;
	out_stream << "_clientMaxBodySize: " << server.getClientMaxBodySize() << " bytes\n";

	out_stream << BLUE BOLD "\n_locations: \n" RESET;
	const std::vector<std::unique_ptr<Location>>& locations = server.getLocations();
	for (size_t i = 0; i < locations.size(); ++i)
		out_stream << *locations[i] << std::endl;
	out_stream << BLUE BOLD "_defaultLocation: \n" RESET;
	try
	{
		const Location& default_location = server.getDefaultLocation();
		out_stream << default_location << std::endl;
	}
	catch (const std::exception& e)
	{
		out_stream << "Server does not contain default location.\n";
	}

	out_stream << "----------------------------------------" << std::endl;
	out_stream << GREEN BOLD "_FD Server:       " RESET << server.getFD() << std::endl;
	out_stream << RESET "addr Server:      " << server.getAddr() << std::endl;
	out_stream << "port of Server:   " << server.getPortOfSocket() << std::endl;
	out_stream << "versionIP Server: " << server.getVersionIP() << std::endl;
	out_stream << "stringIP Server:  " << server.getstrIP() << std::endl;
	out_stream << "========================================" << std::endl;
	return out_stream;
}
