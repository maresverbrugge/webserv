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

# include "webserv.hpp"
# include "Server.hpp"

# include <vector>
# include <memory>

class ServerPool
{
	private:
		static std::unique_ptr<ServerPool>		_instance;
		std::vector<std::unique_ptr<Server>>	_servers;

		ServerPool();

	public:
		ServerPool(const ServerPool&) = delete;
        ServerPool& operator=(const ServerPool&) = delete;
		~ServerPool();
	
		void										addServer(std::unique_ptr<Server> server);

		static ServerPool& 							getInstance();
		const std::vector<std::unique_ptr<Server>>&	getServers() const;
};

std::ostream& operator<<(std::ostream& out_stream, const ServerPool& serverpool);

#endif