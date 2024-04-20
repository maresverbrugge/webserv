/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configErrors.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkoolhov <fkoolhov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 13:32:00 by fkoolhov          #+#    #+#             */
/*   Updated: 2024/04/20 16:24:01 by fkoolhov         ###   ########.fr       */
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
		for (auto it = servers.begin(); it != servers.end(); it++)
		{
			int port = (*it)->getPort();
			occupied_ports.insert(port);
			if (occupied_ports.count(port) > 0)
                throw std::runtime_error("Duplicate port was found.");
            else
                occupied_ports.insert(port);
		}
	}
}

void check_server_config_errors(std::unique_ptr<Server>& server)
{
	if (server->getPort() < 1)
		throw std::runtime_error("Server must have port number.");
	else if (server->getHost().length() == 0)
		throw std::runtime_error("Server must have host.");
	else if (server->getRootFolder().length() == 0)
		throw std::runtime_error("Server must have root folder.");
	else if (server->getDefaultErrorPage().length() == 0)
		throw std::runtime_error("Server must have default error page.");
}

void check_location_config_errors(std::unique_ptr<Location>& location)
{
	if (location->getLocationName().length() == 0)
		throw std::runtime_error("Server location must have location name.");
	else if (location->getPath().length() == 0)
		throw std::runtime_error("Server location must have path.");
	
}