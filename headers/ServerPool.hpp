/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ServerPool.hpp                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: fkoolhov <fkoolhov@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/16 12:49:12 by fkoolhov      #+#    #+#                 */
/*   Updated: 2024/05/02 12:27:04 by fhuisman      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERPOOL_HPP
# define SERVERPOOL_HPP

# include "Server.hpp"

class ServerPool
{
	private:
		std::vector<std::shared_ptr<Server>>	_servers;
		
	public:
		ServerPool();
		~ServerPool();

		void	addServer(std::shared_ptr<Server> server);
		
		const std::vector<std::shared_ptr<Server>>&	getServers() const;
		std::vector<std::shared_ptr<Server>>&		getServers() ;
};

std::ostream& operator<<(std::ostream& out_stream, const ServerPool& serverpool);

#endif