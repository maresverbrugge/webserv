
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

#include "Epoll.hpp"
#include "Server.hpp"

std::unique_ptr<Epoll> Epoll::_instance = nullptr;

Epoll& Epoll::getInstance()
{
    if (!_instance)
        _instance = std::unique_ptr<Epoll>(new Epoll());
    return *_instance;
}

Epoll::Epoll() : _isChildProcess(false)
{
	std::cout << "Epoll constructor called" << std::endl;

	_FD = epoll_create(1);
	if (_FD < 0)
	{
		throw FatalException("epoll_create() failed");
	}
	set_to_cloexec(_FD);
	if (addFDToEpoll(&_signal, EPOLLIN, _signal.getFD()) < 0)
	{
		close(_FD);
		throw FatalException("Error adding signal FD to epoll");
	}
}

Epoll::~Epoll()
{
	std::cout << "Epoll destructor called" << std::endl;
}

bool Epoll::isChild() const
{
	return _isChildProcess;
}

const Signal& Epoll::getSignal() const
{
	return _signal;
}

int Epoll::addFDToEpoll(AFileDescriptor *ptr, int event_to_poll_for, int fdToAdd)
{
	struct epoll_event event{};

	event.events = event_to_poll_for;
	event.data.ptr = ptr;
	return (epoll_ctl(_FD, EPOLL_CTL_ADD, fdToAdd, &event));
}

void Epoll::runScript(CGI* cgi)
{
    const char* python_path = "/usr/bin/python3";
	std::string script_string = cgi->getScriptString();
    const char* python_script = script_string.c_str();
	char *const argv[] = { const_cast<char *>(python_path), const_cast<char *>(python_script), NULL };
	char** envp = cgi->getEnvp();

	dup2(cgi->getFD(), STDOUT_FILENO);
	cgi->getClient().deleteCGI();
	execve(python_path, argv, envp);
	perror("execve failed");
	exit(EXIT_FAILURE);
}

int Epoll::handleInEvents(AFileDescriptor* ptr)
{
	Server *server = dynamic_cast<Server *>(ptr);
	Client *client = dynamic_cast<Client *>(ptr);
	Signal *signal = dynamic_cast<Signal *>(ptr);
	CGI *cgi = dynamic_cast<CGI *>(ptr);

	if (server)
		server->createNewClientConnection();
	else if (client && client->getReadyForFlag() == READ)
	{
		if (client->receiveFromClient() != SUCCESS)
		{
			client->getServer().removeClientConnection(client);
			return (ERROR);
		}
	}
	else if (client && client->getReadyForFlag() == WRITE) // TALK TO MARES AND FLEN, do this or read all requests first? some other solution???
	{
		if (!client->getResponse().empty())
		{
			client->writeToClient();
			client->getServer().removeClientConnection(client);
		}
	}
	else if (signal)
		signal->readSignal();
	else if (cgi)
	{
		if (cgi->readFromPipe() != SUCCESS)
			cgi->getClient().getServer().removeClientConnection(&cgi->getClient());
		else
			cgi->getClient().deleteCGI();
	}
	return (SUCCESS);
}

void Epoll::handleOutEvents(AFileDescriptor* ptr)
{
	Client *client = dynamic_cast<Client *>(ptr);
	CGI *cgi = dynamic_cast<CGI *>(ptr);

	if (client && client->getReadyForFlag() == WRITE && !_isChildProcess)
	{
		client->writeToClient();
		client->getServer().removeClientConnection(client);
	}
	else if (cgi && _isChildProcess)
		runScript(cgi);
}

void Epoll::EpollLoop()
{
	struct epoll_event event_list[MAX_EVENTS];
    AFileDescriptor *ready_listDataPtr{};

	while (g_serverPoolIsRunning)
	{
		int epoll_return = epoll_wait(_FD, event_list, MAX_EVENTS, -1);
		if (epoll_return < 0)
			throw FatalException("epoll_wait() failed");
		for (int i = 0; i < epoll_return; i++)
		{
			ready_listDataPtr = static_cast<AFileDescriptor *>(event_list[i].data.ptr);
			if (event_list[i].events & EPOLLIN && !_isChildProcess)
			{
				if (handleInEvents(ready_listDataPtr) != SUCCESS)
					break; // testen met meerdere clients
			}
			else if (event_list[i].events & EPOLLOUT)
				handleOutEvents(ready_listDataPtr);
		}
	}
}

void Epoll::setIsChildProcess(bool isChild)
{
	_isChildProcess = isChild;
}

std::ostream& operator<<(std::ostream& out_stream, const Epoll& Epoll)
{
	out_stream << CYAN BOLD "Epoll:" RESET << "\n_FD Epoll: " << Epoll.getFD() << std::endl;
	return out_stream;
}
