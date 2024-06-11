/* ************************************************************************* */
/*      ##       ##      ## ##       ##      ## ##       ##      ##          */
/*       ##     ####    ##   ##     ####    ##   ##     ####    ##           */
/*        ##  ##   ##  ##     ##  ##   ##  ##     ##  ##   ##  ##            */
/*         ####     ####       ####     ####       ####     ####             */
/*          ##       ##         ##       ##         ##       ##              */
/*                                                                           */
/*           WONDERFUL            WEBSERV           WONDERTEAM               */
/*                                                                           */
/*      FELICIA KOOLHOVEN      FLEN HUISMAN       MARES VERBRUGGE            */
/*          fkoolhov             fhuisman             mverbrug               */
/*                                                                           */
/*          Codam Coding College        part of 42 network                   */
/*                            April - May 2024                               */
/* ************************************************************************* */

#ifndef SERVER_HPP
# define SERVER_HPP

# define BACKLOG 511

# include "webserv.hpp"
# include "AFileDescriptor.hpp"
# include "Location.hpp"
# include "Client.hpp"

# include <vector>
# include <map>
# include <memory>
# include <sys/socket.h>
# include <cstring>
# include <netdb.h>
# include <string>
# include <unistd.h>
# include <arpa/inet.h>
# include <sys/epoll.h>
# include <netinet/in.h>

class Server : public AFileDescriptor
{
	private:
		int										_port;
		std::string								_host;
		std::vector<std::string>				_serverNames;
		std::string								_rootFolder;
		std::map<short, std::string>			_customErrorPages;
		unsigned long long						_clientMaxBodySize;
		std::vector<std::unique_ptr<Location>>	_locations;
		const std::unique_ptr<Location>			_defaultLocation;
		struct addrinfo*						_serverInfo{};
		std::map<int, std::unique_ptr<Client>>	_connectedClients;

		void*		_addr{};
		std::string	_versionIP{};
		int			_portOfSocket{};
		char		_strIP[INET6_ADDRSTRLEN]{};

	public:
		Server(int port,
				std::string host,
				std::vector<std::string> serverNames,
				std::string rootFolder,
				std::map<short, std::string> customErrorPages,
				unsigned long long clientMaxBodySize,
				std::vector<std::unique_ptr<Location>> locations,
				std::unique_ptr<Location> defaultLocation);
		~Server();

		void	setPort(int port);
		void	setHost(std::string host);
		void	addServerName(std::string serverName);
		void	setRootFolder(std::string rootFolder);
		void	addCustomErrorPage(short errorCode, std::string errorPage);
		void	setClientMaxBodySize(unsigned long long clientMaxBodySize);
		void	addLocation(std::unique_ptr<Location> location);
		void	setServerInfo() = delete;
		void	createNewClientConnection();
		void	removeClientConnection(Client* client);

		int												getPort() const;
		std::string										getHost() const;
		std::vector<std::string>						getServerNames() const;
		std::string										getRootFolder() const;
		std::map<short, std::string>					getCustomErrorPages() const;
		unsigned long long								getClientMaxBodySize() const;
		const std::vector<std::unique_ptr<Location>>&	getLocations() const;
		const Location&									getDefaultLocation() const;
		struct addrinfo* 								getServerInfo() const;
		const std::map<int, std::unique_ptr<Client>>&	getConnectedClients() const;

		void		setAddr(void* addr);
		void		setVersionIP(std::string versionIP);
		void		setPortOfSocket(in_port_t portOfSocket);
		void 		setStrIP(struct addrinfo *ptr);
		void 		setAddrInfoSocket(struct addrinfo *ptr);

		void*		getAddr() const;
		std::string	getVersionIP() const;
		int			getPortOfSocket() const;
		const char*	getstrIP() const;

		class ServerConfigError : public std::exception 
		{
			public:
				explicit ServerConfigError(const std::string& message);
				virtual const char* what() const noexcept override;

			private:
				std::string message_;
		};
};

std::ostream& operator<<(std::ostream& out_stream, const Server& server);

#endif
