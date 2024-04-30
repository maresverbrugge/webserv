/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mverbrug <mverbrug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 17:54:08 by felicia           #+#    #+#             */
/*   Updated: 2024/04/30 16:52:24 by mverbrug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "Location.hpp"
# include <sys/socket.h> // for socket(), bind(), listen()
# include <netinet/in.h> // for struct sockaddr_in
# include <cstring> // for memset

# define BACKLOG 5

class Server
{
	private:
		int										_port;
		std::string								_host;
		std::vector<std::string>				_serverNames;
		std::string								_rootFolder;
		std::string								_defaultErrorPage;
		std::map<int, std::string>				_customErrorPages;
		unsigned long long						_clientMaxBodySize; // in bytes
		std::vector<std::unique_ptr<Location>>	_locations;
		int										_serverSocket;
		struct sockaddr_in						_serverSockAddress;

	public:
		Server();
		~Server();

		void	setPort(int port);
		void	setHost(std::string host);
		void	addServerName(std::string serverName);
		void	setRootFolder(std::string rootFolder);
		void	setDefaultErrorPage(std::string defaultErrorPage);
		void	addCustomErrorPage(int errorCode, std::string errorPage);
		void	setClientMaxBodySize(unsigned long long clientMaxBodySize);
		void	addLocation(std::unique_ptr<Location> location);
		void	setServerSocket(int serverSocket); // ? will we be using this?

		int												getPort() const;
		std::string										getHost() const;
		std::vector<std::string>						getServerNames() const;
		std::string										getRootFolder() const;
		std::string										getDefaultErrorPage() const;
		std::map<int, std::string>						getCustomErrorPages() const;
		unsigned long long								getClientMaxBodySize() const;
		const std::vector<std::unique_ptr<Location>>&	getLocations() const;
		int												getServerSocket() const;

		void	initServerSocket();
		void	serverSocketOptions();
		void	serverSocketAddress();
		void	serverSocketBind();
		void	serverSocketListen();
		// void	serverAccept();
};

std::ostream& operator<<(std::ostream& out_stream, const Server& server);

#endif