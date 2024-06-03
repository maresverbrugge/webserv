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

#ifndef SERVERPOOL_HPP
# define SERVERPOOL_HPP

# include "Epoll.hpp"
# include "Server.hpp"

class Epoll;
class Server;

class ServerPool
{
	private:
		std::vector<std::unique_ptr<Server>>	_servers;
		std::unique_ptr<Epoll> 					_epoll = nullptr;

	public:
		ServerPool();
		~ServerPool();

		void	addServer(std::unique_ptr<Server> server);
		
		const std::vector<std::unique_ptr<Server>>&	getServers() const;
		std::vector<std::unique_ptr<Server>>&		getServers() ;
		Epoll& getEpollReference() const;
		Epoll* getEpollPtr();

};

std::ostream& operator<<(std::ostream& out_stream, const ServerPool& serverpool);

#endif