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
}

Server::~Server()
{
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
	return out_stream;
}