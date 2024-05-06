/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mverbrug <mverbrug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 18:07:06 by felicia           #+#    #+#             */
/*   Updated: 2024/05/06 15:29:30 by mverbrug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server() :	_port(-1),
					_host(""),
					_rootFolder(""),
					_defaultErrorPage(""),
					_clientMaxBodySize(1)
{
	std::cout << "Server constructor called" << std::endl;
	// create fdSocket for Server and set options
	if ((this->_socketFD = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		throw std::runtime_error("Error creating _socketFD Server with socket()");
	initServerSocket();
}

void Server::initServerSocket()
{
	serverSocketOptions();
	serverSocketAddress();
	serverSocketBind();
	serverSocketListen();
}

// Set socket option to make socket reusable immediately after closing
// Handle signal ctrl-C
void Server::serverSocketOptions()
{
    int opt = 1;
    if (setsockopt(getSocketFD(), SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
		throw std::runtime_error("Error setting options serverSocket");
		// close(this->_socketFD); // ? close server socket
}

// Fill in sockaddr struct
void Server::serverSocketAddress()
{
    std::memset(&_serverSockAddress, 0, sizeof(_serverSockAddress));	// pad and set struct to zero so struct can be cast to another type      (sockaddr instead of sockaddr_in) for bind() and accept()
	_serverSockAddress.sin_family = AF_INET;							// family, AF_INET when using IP networking
    _serverSockAddress.sin_addr.s_addr = INADDR_ANY;					// address for this socket (usually machine's IP address), or INADDR_ANY if OS can decide
    _serverSockAddress.sin_port = htons(getPort());			 			// converts short integer (port number) to network representation byte order
}

// binding server socket to port
void Server::serverSocketBind()
{
	socklen_t addr_len = sizeof(_serverSockAddress);
	if (bind(getSocketFD(), (struct sockaddr *)&_serverSockAddress, addr_len) < 0) // ! change to getaddrinfo?
		throw std::runtime_error("Error binding server socket to port with bind()");
			// close(this->_socketFD); // ? close server socket
}

void Server::serverSocketListen()
{
    // let server socket listen to incoming requests
    if (listen(getSocketFD(), BACKLOG) < 0)
		throw std::runtime_error("Error server socket listen to incoming requests with listen()");
			// close(this->_socketFD); // ? close server socket
}

Server::~Server()
{
	// close(this->_socketFD); // ? close server socket
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

void Server::setDefaultErrorPage(std::string defaultErrorPage)
{
	this->_defaultErrorPage = defaultErrorPage;
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

std::string Server::getDefaultErrorPage() const
{
	return this->_defaultErrorPage;
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
	out_stream << "_defaultErrorPage: " << server.getDefaultErrorPage() << std::endl;
	out_stream << "_customErrorPages: " << std::endl;
	const std::map<short, std::string>& customErrorPages = server.getCustomErrorPages();
	for (const std::pair<const short, std::string>& error : customErrorPages)
    	out_stream << "Code " << error.first << ", Page " << error.second << std::endl;
	out_stream << "_clientMaxBodySize: " << server.getClientMaxBodySize() << " bytes\n";
	
	// ! outcommented for testing sockets and epoll:
	// out_stream << BLUE BOLD "\n_locations: \n" RESET;
	// const std::vector<std::unique_ptr<Location>>& locations = server.getLocations();
	// for (size_t i = 0; i < locations.size(); ++i)
	// 	out_stream << *locations[i] << std::endl;
	
	// out_stream << BLUE BOLD "_defaultLocation: \n" RESET;
	// const std::unique_ptr<Location>& default_location = server.getDefaultLocation();
	// if (default_location)
	// 	out_stream << *default_location << std::endl;
	// else
	// 	out_stream << "Server does not contain default location.\n";
	out_stream << "_socketFD Server: " << server.getSocketFD() << std::endl;

	return out_stream;
}