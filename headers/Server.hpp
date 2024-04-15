/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: felicia <felicia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 17:54:08 by felicia           #+#    #+#             */
/*   Updated: 2024/04/15 18:11:18 by felicia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "webserv.hpp"
# include "Location.hpp"

class Server
{
	private:
		int							_port;
		std::string					_host;
		std::vector<std::string>	_serverNames;
		std::vector<Location>		_locations;
		std::map<int, std::string>	_errorPages;

	public:
		Server();
		Server(const Server& src);
		~Server();
		Server& operator=(const Server& src);

		void	setPort(int port);
		void	setHost(std::string host);
		void	addServerName(std::string serverName);
		void	addLocation(Location location);
		void	addErrorPage(int errorCode, std::string errorPage);
		
		int							getPort();
		std::string					getHost();
		std::vector<std::string>	getServerNames();
		std::vector<Location>		getLocations();
		std::map<int, std::string>	getErrorPages();
};

#endif