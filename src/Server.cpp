/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Server.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: felicia <felicia@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/15 18:07:06 by felicia       #+#    #+#                 */
/*   Updated: 2024/05/07 15:02:39 by fhuisman      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

/*   By: fkoolhov <fkoolhov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 18:07:06 by felicia           #+#    #+#             */
/*   Updated: 2024/04/25 14:38:55 by fkoolhov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server()
{
	std::cout << "Server constructor called" << std::endl;
	this->_port = 8080;
	this->_host = "0.0.0.0";
	this->_rootFolder = "";
	this->_defaultErrorPage = "";
	this->_clientMaxBodySize = 1024 * 1024;
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
	
	out_stream << BLUE BOLD "\n_locations: \n" RESET;
	const std::vector<std::unique_ptr<Location>>& locations = server.getLocations();
	for (size_t i = 0; i < locations.size(); ++i)
		out_stream << *locations[i] << std::endl;
	
	out_stream << BLUE BOLD "_defaultLocation: \n" RESET;
	const std::unique_ptr<Location>& default_location = server.getDefaultLocation();
	if (default_location)
		out_stream << *default_location << std::endl;
	else
		out_stream << "Server does not contain default location.\n";
	return out_stream;
}