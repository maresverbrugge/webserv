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

# include "Epoll.hpp"
# include "ServerPool.hpp"

// std::unique_ptr<Epoll> Epoll::_instance = nullptr;

ServerPool::ServerPool() : _epoll(getEpollPtr())
{
	std::cout << "ServerPool constructor called" << std::endl;
}

ServerPool::~ServerPool()
{
	std::cout << "ServerPool destructor called" << std::endl;
}

Epoll* ServerPool::getEpollPtr()
{
    if (!_epoll)
	{
        _epoll = std::unique_ptr<Epoll>(new Epoll());
    }
    return _epoll.get();
}

Epoll& ServerPool::getEpollReference() const
{
	return *(this->_epoll);
}

void ServerPool::addServer(std::unique_ptr<Server> server)
{
	this->_servers.push_back(std::move(server));
}

const std::vector<std::unique_ptr<Server>>& ServerPool::getServers() const
{
	return this->_servers;
}

std::vector<std::unique_ptr<Server>>& ServerPool::getServers()
{
	return this->_servers;
}

// Epoll& ServerPool::getEpollReference() const
// {
// 	return *(this->_epoll);
// }

std::ostream& operator<<(std::ostream& out_stream, const ServerPool& server_pool)
{
	std::cout << std::endl;
	const std::vector<std::unique_ptr<Server>>& servers = server_pool.getServers();
	for (std::vector<std::unique_ptr<Server>>::const_iterator it = servers.begin(); it != servers.end(); ++it)
	{
		const std::unique_ptr<Server>& server = *it;
		out_stream << *server << std::endl;
	}
	return out_stream;
}