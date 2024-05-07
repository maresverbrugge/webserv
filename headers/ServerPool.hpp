/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ServerPool.hpp                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: fkoolhov <fkoolhov@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/16 12:49:12 by fkoolhov      #+#    #+#                 */
/*   Updated: 2024/05/07 15:13:35 by fhuisman      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERPOOL_HPP
# define SERVERPOOL_HPP

# include "Server.hpp"

class ServerPool
{
	private:
		std::vector<std::unique_ptr<Server>>	_servers;
		
	public:
		ServerPool();
		~ServerPool();

		void	addServer(std::unique_ptr<Server> server);
		
		const std::vector<std::unique_ptr<Server>>&	getServers() const;
		std::vector<std::unique_ptr<Server>>&		getServers() ;
};

std::ostream& operator<<(std::ostream& out_stream, const ServerPool& serverpool);

#endif