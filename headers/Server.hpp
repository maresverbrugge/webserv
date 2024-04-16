/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkoolhov <fkoolhov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 17:54:08 by felicia           #+#    #+#             */
/*   Updated: 2024/04/16 15:13:25 by fkoolhov         ###   ########.fr       */
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
		std::string					_rootFolder;
		std::vector<Location>		_locations;
		std::string					_defaultErrorPage;
		std::map<int, std::string>	_customErrorPages;

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