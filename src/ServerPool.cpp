/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ServerPool.cpp                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: felicia <felicia@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/17 11:03:06 by felicia       #+#    #+#                 */
/*   Updated: 2024/05/02 12:27:57 by fhuisman      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ServerPool.hpp"

ServerPool::ServerPool()
{
	std::cout << "ServerPool constructor called" << std::endl;
}

ServerPool::~ServerPool()
{
	std::cout << "ServerPool destructor called" << std::endl;
}

void ServerPool::addServer(std::shared_ptr<Server> server)
{
	this->_servers.push_back(server);
}

const std::vector<std::shared_ptr<Server>>& ServerPool::getServers() const
{
	return this->_servers;
}

std::vector<std::shared_ptr<Server>>& ServerPool::getServers() 
{
	return this->_servers;
}

std::ostream& operator<<(std::ostream& out_stream, const ServerPool& server_pool)
{
	std::cout << std::endl;
	const std::vector<std::shared_ptr<Server>>& servers = server_pool.getServers();
	for (std::vector<std::shared_ptr<Server>>::const_iterator it = servers.begin(); it != servers.end(); ++it)
	{
		const std::shared_ptr<Server>& server = *it;
		out_stream << *server << std::endl;
	}
	return out_stream;
}