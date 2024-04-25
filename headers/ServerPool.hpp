/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerPool.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mverbrug <mverbrug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 12:49:12 by fkoolhov          #+#    #+#             */
/*   Updated: 2024/04/25 14:25:42 by mverbrug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERPOOL_HPP
# define SERVERPOOL_HPP

# include "Server.hpp"
# include "Epoll.hpp"

class ServerPool
{
	private:
		std::vector<std::unique_ptr<Server>>	_servers;
		Epoll									_epoll;
		
	public:
		ServerPool();
		~ServerPool();

		void	addServer(std::unique_ptr<Server> server);
		
		const std::vector<std::unique_ptr<Server>>&	getServers() const;
		Epoll getEpoll() const;

};

std::ostream& operator<<(std::ostream& out_stream, const ServerPool& serverpool);

#endif