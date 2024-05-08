/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mverbrug <mverbrug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 17:54:08 by felicia           #+#    #+#             */
/*   Updated: 2024/05/08 11:38:45 by mverbrug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*   By: fkoolhov <fkoolhov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 17:54:08 by felicia           #+#    #+#             */
/*   Updated: 2024/05/07 15:26:54 by fkoolhov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "ASocket.hpp"
# include "Location.hpp"
# include <sys/socket.h> // for socket(), bind(), listen()
# include <cstring> // for memset
# include <netdb.h> // getaddrinfo()
# include <string> // for to_string
# include <unistd.h> // for close
# include <arpa/inet.h> // * for inet_ntop() only to print info - might remove?

# define BACKLOG 5

class Server : public ASocket
{
	private:
		int										_port;
		std::string								_host;
		std::vector<std::string>				_serverNames;
		std::string								_rootFolder;
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
		void	addCustomErrorPage(short errorCode, std::string errorPage);
		void	setClientMaxBodySize(unsigned long long clientMaxBodySize);
		void	addLocation(std::unique_ptr<Location> location);
		void	setDefaultLocation(std::unique_ptr<Location> location);
		
		int												getPort() const;
		std::string										getHost() const;
		std::vector<std::string>						getServerNames() const;
		std::string										getRootFolder() const;
		std::map<short, std::string>					getCustomErrorPages() const;
		unsigned long long								getClientMaxBodySize() const;
		const std::vector<std::unique_ptr<Location>>&	getLocations() const;
		const std::unique_ptr<Location>&				getDefaultLocation() const;
};

std::ostream& operator<<(std::ostream& out_stream, const Server& server);

#endif