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

# include "ASocket.hpp"
# include "Location.hpp"
# include <sys/socket.h> // for socket(), bind(), listen()
# include <cstring> // for memset
# include <netdb.h> // getaddrinfo()
# include <string> // for to_string
# include <unistd.h> // for close
# include <arpa/inet.h> // * for inet_ntop() only to print info - might remove?
# include <sys/epoll.h> // for EPOLLIN

# define BACKLOG 5

class ServerPool;

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

		struct addrinfo*						_serverInfo{};
		ServerPool& 							_serverPool;

	public:
		Server(int port,
				std::string host,
				std::vector<std::string> serverNames, 
				std::string rootFolder, 
				std::map<short, std::string> customErrorPages, 
				unsigned long long clientMaxBodySize, 
				std::vector<std::unique_ptr<Location>> locations, 
				std::unique_ptr<Location> defaultLocation,
				ServerPool& serverPool);
		~Server();

		void	setPort(int port);
		void	setHost(std::string host);
		void	addServerName(std::string serverName);
		void	setRootFolder(std::string rootFolder);
		void	addCustomErrorPage(short errorCode, std::string errorPage);
		void	setClientMaxBodySize(unsigned long long clientMaxBodySize);
		void	addLocation(std::unique_ptr<Location> location);
		void	setDefaultLocation(std::unique_ptr<Location> location);

		void	createNewClientConnection();
		
		int												getPort() const;
		std::string										getHost() const;
		std::vector<std::string>						getServerNames() const;
		std::string										getRootFolder() const;
		std::map<short, std::string>					getCustomErrorPages() const;
		unsigned long long								getClientMaxBodySize() const;
		const std::vector<std::unique_ptr<Location>>&	getLocations() const;
		const std::unique_ptr<Location>&				getDefaultLocation() const;

		struct addrinfo* getServerInfo() const;
		ServerPool& getServerPool() const;

		void	configSocket();
};

std::ostream& operator<<(std::ostream& out_stream, const Server& server);

#endif