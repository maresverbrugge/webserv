/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mverbrug <mverbrug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 18:07:06 by felicia           #+#    #+#             */
/*   Updated: 2024/05/08 11:41:58 by mverbrug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*   By: fkoolhov <fkoolhov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 18:07:06 by felicia           #+#    #+#             */
/*   Updated: 2024/05/07 15:27:24 by fkoolhov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server() :	_port(8080),
					_host("0.0.0.0"),
					_rootFolder(""),
					_clientMaxBodySize(1024 * 1024)
{
	std::cout << "Server constructor called" << std::endl;

    struct addrinfo hints{};
    struct addrinfo *res{};
    struct addrinfo *p{};
	int	yes = true;
    int status{};

    std::memset(&hints, 0, sizeof(hints));	// pad and set struct to zero so struct can be cast to another type
	hints.ai_family = AF_UNSPEC;			// AF_UNSPEC for IPv4 and IPv6
    hints.ai_socktype = SOCK_STREAM; 		// TCP stream sockets
    hints.ai_flags = AI_PASSIVE;			// AI_PASSIVE fill in my IP for me
	
	// std::string test = std::to_string(this->getPort()).c_str();
	// int test_port = this->getPort();
	// std::cout << "test port = " << test_port << " and test string = " << test << std::endl; // ! testing
	// if ((status = getaddrinfo(NULL, std::to_string(this->getPort()).c_str(), &hints, &res)) != 0)
	if ((status = getaddrinfo(NULL, "8080", &hints, &res)) != 0)
    {
		std::cerr << "Getaddrinfo error on server: " << gai_strerror(status) << std::endl;
        throw std::runtime_error("Error setting options serverSocket getaddrinfo()");
    }
    for (p = res; p != NULL; p = p->ai_next)
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
		freeaddrinfo(res);
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
        port = ntohs(ipv4->sin_port);
    }
    else // if bound server socket is IPv6 (else if (p->ai_family == AF_INET6))
    {
        struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr; // cast it to sockaddr_in6
        addr = &(ipv6->sin6_addr);
        versionIP = "IPv6";
        port = ntohs(ipv6->sin6_port);
    }
    inet_ntop(p->ai_family, addr, strIP, INET6_ADDRSTRLEN); // convert the IP to a string and print it
	// * END OF PRINT INFO
	
    freeaddrinfo(res);
	if (listen(_socketFD, BACKLOG) < 0)
	{
		close(_socketFD); // close server socket
		throw std::runtime_error("Error server socket listen to incoming requests with listen()");
	}
	this->_port = 8080;
	this->_host = "0.0.0.0";
	this->_rootFolder = "";
	this->_clientMaxBodySize = 1024 * 1024;
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

const std::unique_ptr<Location>& Server::getDefaultLocation() const
{
	return this->_defaultLocation;
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
	
	// ! outcommented for testing sockets and epoll:
	// out_stream << YELLOW BOLD "\n_locations: \n" RESET;
	// const std::vector<std::unique_ptr<Location>>& locations = server.getLocations();
	// for (size_t i = 0; i < locations.size(); ++i)
	// 	out_stream << *locations[i] << std::endl;
	
	// out_stream << YELLOW BOLD "_defaultLocation: \n" RESET;
	// const std::unique_ptr<Location>& default_location = server.getDefaultLocation();
	// if (default_location)
	// 	out_stream << *default_location << std::endl;
	// else
	// 	out_stream << "Server does not contain default location.\n";
	out_stream << "----------------------------------------" << std::endl;
	out_stream << BOLD "_socketFD Server: " << server.getSocketFD() << std::endl;
	out_stream << RESET "versionIP Server: " << server.versionIP << std::endl;
	out_stream << "port of Server:   " << server.port << std::endl;
	// out_stream << "addr of Server:   " << &server.addr << std::endl;
	out_stream << "stringIP Server:  " << server.strIP << std::endl;
	out_stream << "========================================" << std::endl;
	return out_stream;
}