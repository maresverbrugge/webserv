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
    struct addrinfo *p{};
	int	yes = true;
    int status{};

    std::memset(&hints, 0, sizeof(hints));	// pad and set struct to zero so struct can be cast to another type
	hints.ai_family = AF_UNSPEC;			// AF_UNSPEC for IPv4 and IPv6
    hints.ai_socktype = SOCK_STREAM; 		// TCP stream sockets
    hints.ai_flags = AI_PASSIVE;			// AI_PASSIVE fill in my IP for me

	if ((status = getaddrinfo(NULL, (std::to_string(_port)).c_str(), &hints, &_serverInfo)) != 0)
    {
		std::string error_message = gai_strerror(status);
        throw ServerConfigError("Getaddrinfo error on server: " + error_message);
    }
    for (p = _serverInfo; p != NULL; p = p->ai_next)
    {
		// create fdSocket for Server and set options
		if ((_socketFD = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0)
			continue; // skip over this node in linked list as it didn't create a socket
		set_fd_to_non_blocking_and_cloexec(_socketFD); // set socket to non-blocking
		if (setsockopt(_socketFD, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) < 0) // set socket option to make socket reusable immediately after closing
		{
			close(_socketFD);
			continue ; // skip over this node in linked list as it didn't set sock options
		}
		if (bind(_socketFD, p->ai_addr, p->ai_addrlen) < 0)
		{
			close(_socketFD);
			continue ; // skip over this node in linked list as it didn't bind
		}
		break; // if we get here, we successfully created a socket, set options and got it to bind
    }

	if (p == NULL)
	{
		freeaddrinfo(_serverInfo);
		throw ServerConfigError("Failed to configure server socket");
	}

	// * ONLY TO PRINT INFO ON SOCKET:
	// std::cout << "p->ai_family = " << p->ai_family << std::endl; // ! for testing, remove later
    if (p->ai_family == AF_INET) // if bound server socket is IPv4
    {
        struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr; // cast it to sockaddr_in
        addr = &(ipv4->sin_addr);
        versionIP = "IPv4";
        portOfASocket = ntohs(ipv4->sin_port);
    }
    else // if bound server socket is IPv6 (else if (p->ai_family == AF_INET6))
    {
        struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr; // cast it to sockaddr_in6
        addr = &(ipv6->sin6_addr);
        versionIP = "IPv6";
        portOfASocket = ntohs(ipv6->sin6_port);
    }
    inet_ntop(p->ai_family, addr, strIP, INET6_ADDRSTRLEN); // convert the IP to a string and print it
	// * END OF PRINT INFO

	if (listen(_socketFD, BACKLOG) < 0)
	{
		close(_socketFD);
		freeaddrinfo(_serverInfo);
		throw ServerConfigError("Server listen() failed");
	}

	if (Epoll::getInstance().addFDToEpoll(this, EPOLLIN, _socketFD) < 0)
	{
		close(_socketFD);
		freeaddrinfo(_serverInfo);
		throw ServerConfigError("Error adding FD to epoll");
	}
}

struct addrinfo* Server::getServerInfo() const
{
	return this->_serverInfo;
}

void Server::createNewClientConnection()
{
	try
	{
    	std::unique_ptr<Client> newClient = std::make_unique<Client>(*this);
		_connectedClients[newClient->getSocketFD()] = std::move(newClient);
	}
	catch (std::runtime_error& exception)
	{
		std::cout << RED BOLD "Error: " RESET << exception.what() << std::endl;
	}
}

void Server::removeClientConnection(Client* client)
{
	_connectedClients.erase(client->getSocketFD());
}

Server::~Server()
{
	freeaddrinfo(_serverInfo);
	std::cout << "Server destructor called" << std::endl;
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

const std::map<int, std::unique_ptr<Client>>& Server::getConnectedClients() const
{
	return this->_connectedClients;
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

	// ! outcommented in epoll branch
	// out_stream << BLUE BOLD "\n_locations: \n" RESET;
	// const std::vector<std::unique_ptr<Location>>& locations = server.getLocations();
	// for (size_t i = 0; i < locations.size(); ++i)
	// 	out_stream << *locations[i] << std::endl;
	// out_stream << BLUE BOLD "_defaultLocation: \n" RESET;
	// try
	// {
	// 	Location& default_location = server.getDefaultLocation();
	// 	out_stream << default_location << std::endl;
	// }
	// catch(const std::exception& e)
	// {
	// 	out_stream << "Server does not contain default location.\n";
	// }

	out_stream << "----------------------------------------" << std::endl;
	out_stream << BOLD "_socketFD Server: " << server.getSocketFD() << std::endl;
	out_stream << RESET "versionIP Server: " << server.versionIP << std::endl;
	out_stream << "port of Server:   " << server.portOfASocket << std::endl;
	// out_stream << "addr of Server:   " << &server.addr << std::endl;
	out_stream << "stringIP Server:  " << server.strIP << std::endl;
	out_stream << "========================================" << std::endl;
	return out_stream;
}
