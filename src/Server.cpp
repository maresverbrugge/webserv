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
# include "ServerPool.hpp"

Server::Server(int port, std::string host, std::vector<std::string> serverNames, std::string rootFolder, std::map<short, std::string> customErrorPages, unsigned long long clientMaxBodySize, std::vector<std::unique_ptr<Location>> locations, std::unique_ptr<Location> defaultLocation, ServerPool& serverPool)
	: _port(port),
	  _host(host),
	  _serverNames(serverNames),
	  _rootFolder(rootFolder),
	  _customErrorPages(customErrorPages),
	  _clientMaxBodySize(clientMaxBodySize),
	  _locations(std::move(locations)),
	  _defaultLocation(std::move(defaultLocation)),
	  _epollReference(serverPool.getEpollInstance()) //   _serverPool(serverPool),
{
	std::cout << "Server constructor called" << std::endl;

    struct addrinfo hints{};
    // struct addrinfo *_serverInfo{};
    struct addrinfo *p{};
	int	yes = true;
    int status{};

    std::memset(&hints, 0, sizeof(hints));	// pad and set struct to zero so struct can be cast to another type
	hints.ai_family = AF_UNSPEC;			// AF_UNSPEC for IPv4 and IPv6
    hints.ai_socktype = SOCK_STREAM; 		// TCP stream sockets
    hints.ai_flags = AI_PASSIVE;			// AI_PASSIVE fill in my IP for me

	if ((status = getaddrinfo(NULL, (std::to_string(_port)).c_str(), &hints, &_serverInfo)) != 0)
    {
		std::cerr << "Getaddrinfo error on server: " << gai_strerror(status) << std::endl;
        throw std::runtime_error("Error setting options serverSocket getaddrinfo()");
    }
    for (p = _serverInfo; p != NULL; p = p->ai_next)
    {
		// create fdSocket for Server and set options
		if ((_socketFD = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0)
			continue; // skip over this node in linked list as it didn't create a socket
		if (setsockopt(_socketFD, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) < 0) // set socket option to make socket reusable immediately after closing
		{
			close(_socketFD); // close server socket
			continue ; // skip over this node in linked list as it didn't set sock options
		}
		if (bind(_socketFD, p->ai_addr, p->ai_addrlen) < 0)
		{
			close(_socketFD); // close server socket
			continue ; // skip over this node in linked list as it didn't bind
		}
		break; // if we get here, we successfully created a socket, set options and got it to bind
    }
	if (p == NULL) // if we get in this if, we failed to create any socket, set options and got it to bind
	{
		freeaddrinfo(_serverInfo);
		std::cout << "error server with server name[0]: " << this->getServerNames()[0] << std::endl; // ! for testing, remove later
		throw std::runtime_error("Error: failed to create socket, set sockopt and bind");
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

    // freeaddrinfo(_serverInfo);
	if (listen(_socketFD, BACKLOG) < 0)
	{
		close(_socketFD); // close server socket
		throw std::runtime_error("Error server socket listen to incoming requests with listen()");
	}

	// give reference of serverPool to constructor of Server so we can access EpollInstance
	// Epoll& EpollInstance = serverPool.getEpollInstance();
	// if (EpollInstance.addFDToEpoll(this, EPOLLIN, _socketFD) < 0)
	// {
	// 	close(_socketFD); // close server socket
	// 	throw std::runtime_error("Error adding fd to epoll");
	// }
	if (_epollReference.addFDToEpoll(this, EPOLLIN, _socketFD) < 0)
	{
		close(_socketFD); // close server socket
		throw std::runtime_error("Error adding fd to epoll");
	}

}

Epoll& Server::getEpollReference() const
{
	return this->_epollReference;
}

// ServerPool& Server::getServerPool() const
// {
// 	return this->_serverPool;
// }

struct addrinfo* Server::getServerInfo() const
{
	return this->_serverInfo;
}

void Server::createNewClientConnection()
{
	Client *newClient = new Client(*this);
	std::cout << "newClient _socketFD = " << newClient->getSocketFD() << std::endl; // for testing
}

Server::~Server()
{
	// close(_socketFD); // close server socket
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

void Server::setDefaultLocation(std::unique_ptr<Location> defaultLocation)
{
	if (_defaultLocation)
		throw std::runtime_error("Server can have only one default location.");
	this->_defaultLocation = std::move(defaultLocation);
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

Location& Server::getDefaultLocation() const
{
	if (!_defaultLocation)
		throw std::runtime_error("No default location present");
	return *this->_defaultLocation;
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
		Location& default_location = server.getDefaultLocation();
		out_stream << default_location << std::endl;
	}
	catch(const std::exception& e)
	{
		out_stream << "Server does not contain default location.\n";
	}

	out_stream << "----------------------------------------" << std::endl;
	out_stream << BOLD "_socketFD Server: " << server.getSocketFD() << std::endl;
	out_stream << RESET "versionIP Server: " << server.versionIP << std::endl;
	out_stream << "port of Server:   " << server.portOfASocket << std::endl;
	// out_stream << "addr of Server:   " << &server.addr << std::endl;
	out_stream << "stringIP Server:  " << server.strIP << std::endl;
	out_stream << "========================================" << std::endl;
	return out_stream;
}
