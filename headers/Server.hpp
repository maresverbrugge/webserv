/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkoolhov <fkoolhov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 17:54:08 by felicia           #+#    #+#             */
/*   Updated: 2024/04/25 14:35:55 by fkoolhov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "Location.hpp"

class Server
{
	private:
		int										_port;
		std::string								_host;
		std::vector<std::string>				_serverNames;
		std::string								_rootFolder;
		std::string								_defaultErrorPage;
		std::map<short, std::string>			_customErrorPages;
		unsigned long long						_clientMaxBodySize; // in bytes
		std::vector<std::unique_ptr<Location>>	_locations;
		std::unique_ptr<Location>				_defaultLocation;

	public:
		Server();
		~Server();

		void	setPort(int port);
		void	setHost(std::string host);
		void	addServerName(std::string serverName);
		void	setRootFolder(std::string rootFolder);
		void	setDefaultErrorPage(std::string defaultErrorPage);
		void	addCustomErrorPage(short errorCode, std::string errorPage);
		void	setClientMaxBodySize(unsigned long long clientMaxBodySize);
		void	addLocation(std::unique_ptr<Location> location);
		void	setDefaultLocation(std::unique_ptr<Location> location);
		
		int												getPort() const;
		std::string										getHost() const;
		std::vector<std::string>						getServerNames() const;
		std::string										getRootFolder() const;
		std::string										getDefaultErrorPage() const;
		std::map<short, std::string>					getCustomErrorPages() const;
		unsigned long long								getClientMaxBodySize() const;
		const std::vector<std::unique_ptr<Location>>&	getLocations() const;
		const std::unique_ptr<Location>&				getDefaultLocation() const;
};

std::ostream& operator<<(std::ostream& out_stream, const Server& server);

#endif