/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: felicia <felicia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 17:54:08 by felicia           #+#    #+#             */
/*   Updated: 2024/04/17 12:27:55 by felicia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "webserv.hpp"
# include "Location.hpp"

class Server
{
	private:
		int										_port;
		std::string								_host;
		std::vector<std::string>				_serverNames;
		std::string								_rootFolder;
		std::vector<std::unique_ptr<Location>>	_locations;
		std::string								_defaultErrorPage;
		std::map<int, std::string>				_customErrorPages;
		int										_clientMaxBodySize;

	public:
		Server();
		~Server();

		void	setPort(int port);
		void	setHost(std::string host);
		void	addServerName(std::string serverName);
		void	setRootFolder(std::string rootFolder);
		void	addLocation(std::unique_ptr<Location> location);
		void	setDefaultErrorPage(std::string defaultErrorPage);
		void	addCustomErrorPage(int errorCode, std::string errorPage);
		void	setClientMaxBodySize(int clientMaxBodySize);
		
		int												getPort() const;
		std::string										getHost() const;
		std::vector<std::string>						getServerNames() const;
		std::string										getRootFolder() const;
		const std::vector<std::unique_ptr<Location>>&	getLocations() const;
		std::string										getDefaultErrorPage() const;
		std::map<int, std::string>						getCustomErrorPages() const;
		int												getClientMaxBodySize() const;
};

std::ostream& operator<<(std::ostream& out_stream, const Server& server);

#endif