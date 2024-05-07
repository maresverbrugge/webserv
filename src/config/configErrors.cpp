/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   configErrors.cpp                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: fkoolhov <fkoolhov@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/20 13:32:00 by fkoolhov      #+#    #+#                 */
/*   Updated: 2024/05/07 15:13:35 by fhuisman      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "configuration.hpp"

void check_serverpool_config_errors(std::unique_ptr<ServerPool>& serverpool)
{
	if (serverpool->getServers().size() == 0)
		throw std::runtime_error("ServerPool must contain at least one server.");
	else
	{
		const std::vector<std::unique_ptr<Server>>& servers = serverpool->getServers();
		std::unordered_set<int> occupied_ports;
		auto it = servers.begin();
		while (it != servers.end()) 
		{
		    int port = (*it)->getPort();
		    if (occupied_ports.count(port) > 0)
		    {
				config_error_message("Duplicate port number in serverpool configuration");
		        it = serverpool->getServers().erase(it);
		    }
		    else 
			{
		        occupied_ports.insert(port);
		        it++;
		    }
		}
	}
}

int check_server_config_errors(std::unique_ptr<Server>& server)
{
	if (server->getPort() < 1 || server->getPort() > 65535)
    	config_error_message("Server port number must be between 1 and 65535.");
	else if (server->getHost().length() == 0)
		config_error_message("Server must have host.");
	else if (server->getRootFolder().length() == 0)
		config_error_message("Server must have root folder.");
	else
		return EXIT_SUCCESS;
	return EXIT_FAILURE;
}

int check_location_config_errors(std::unique_ptr<Location>& location, bool is_default_location)
{
	if (location->getLocationName().length() == 0 && !is_default_location)
		config_error_message("Server location must have location name.");
	else if (location->getPath().length() == 0)
		config_error_message("Server location must have path.");
	else
		return EXIT_SUCCESS;
	return EXIT_FAILURE;
}