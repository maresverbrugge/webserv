
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

	_socketFD = epoll_create(1);
	if (_socketFD < 0)
	{
		throw FatalException("epoll_create() failed");
	}
	set_to_cloexec(_socketFD);
	// std::cout << "Signal fd = " << _signal.getSocketFD() << std::endl;
	if (addFDToEpoll(&_signal, EPOLLIN, _signal.getSocketFD()) < 0)
	{
		close(_socketFD);
		throw FatalException("Error adding signal FD to epoll");
	}
}

Epoll::~Epoll()
{
	std::cout << "Epoll destructor called" << std::endl;
	close(_socketFD);
}

int Epoll::addFDToEpoll(ASocket *ptr, int event_to_poll_for, int fdToAdd)
{
	struct epoll_event event{};

	event.events = event_to_poll_for;
	event.data.ptr = ptr;
	return (epoll_ctl(_socketFD, EPOLL_CTL_ADD, fdToAdd, &event));
}

void Epoll::runScript(CGI* cgi)
{
	// std::cout << "EPOLLOUT on a CGI Class" << std::endl;
    const char* python_path = "/usr/bin/python3";
	std::string script_string = cgi->getScriptString();
    const char* python_script = script_string.c_str();
	char *const argv[] = { const_cast<char *>(python_path), const_cast<char *>(python_script), NULL };
	char** envp = cgi->getEnvp();

	dup2(cgi->getSocketFD(), STDOUT_FILENO);
	cgi->getClient().deleteCGI();

  execve(python_path, argv, envp);
	perror("execve failed");
	exit(EXIT_FAILURE);
}

void Epoll::handleInEvents(ASocket* ptr)
{
	Server *server = dynamic_cast<Server *>(ptr);
	Client *client = dynamic_cast<Client *>(ptr);
	Signal *signal = dynamic_cast<Signal *>(ptr);
	CGI *cgi = dynamic_cast<CGI *>(ptr);
	
	if (server)
	{
		// std::cout << "EPOLLIN on a Server Class! We will now create a client class instance! on fd = " << server->getSocketFD() << std::endl;
		server->createNewClientConnection();
	}
	else if (client && client->getReadyForFlag() == READ)
	{
		// std::cout << "EPOLLIN on a Client Class with FLAG == READ! We will now start receiving and parse the request! on fd = " << client->getSocketFD() << std::endl;
		// std::cout << "Client Class fd = " << client->getSocketFD() << std::endl;
		if (client->receiveFromClient() != SUCCESS)
			client->getServer().removeClientConnection(client);
	}
	else if (signal)
	{
		// std::cout << PURPLE BOLD << "\nEPOLLIN op signal met fd = " << signal->getSocketFD() << RESET << std::endl;
		signal->readSignal();
	}
	else if (cgi)
	{
		// std::cout << "EPOLLIN on a CGI Class on fd = " << cgi->getSocketFD() << std::endl;
		cgi->readFromPipe();
		cgi->getClient().deleteCGI();
	}
}

void Epoll::handleOutEvents(ASocket* ptr)
{
	Client *client = dynamic_cast<Client *>(ptr);
	CGI *cgi = dynamic_cast<CGI *>(ptr);

	if (client && client->getReadyForFlag() == WRITE && !_isChildProcess)
	{
		// std::cout << "EPOLLOUT on a Client Class with FLAG == WRITE! on fd = " << client->getSocketFD() << std::endl;
		client->writeToClient();
		client->getServer().removeClientConnection(client);
	}
	else if (cgi && _isChildProcess)
	{
		// std::cout << YELLOW BOLD "\nWE'RE IN CHILD" << RESET << std::endl;
		// std::cout << "\nEPOLLIN op cgi met fd = " << cgi->getSocketFD() << RESET << std::endl;
		runScript(cgi);
	}
}

void Epoll::EpollWait()
{
	struct epoll_event event_list[MAX_EVENTS];
    ASocket *ready_listDataPtr{};

	while (g_serverIsRunning)
	{
		int epoll_return = epoll_wait(_socketFD, event_list, MAX_EVENTS, -1);
		if (epoll_return < 0)
			throw FatalException("epoll_wait() failed");
		for (int i = 0; i < epoll_return; i++)
		{
			ready_listDataPtr = static_cast<ASocket *>(event_list[i].data.ptr);
			if (event_list[i].events & EPOLLIN && !_isChildProcess)
				handleInEvents(ready_listDataPtr);
			else if (event_list[i].events & EPOLLOUT)
				handleOutEvents(ready_listDataPtr);
			// else if (event_list[i].events & (EPOLLHUP | EPOLLERR)) // EPOLLRDHUP niet denken wij...?
			// {
			// 	// should we remove the class on which was polled?	
			// 	Client *client = dynamic_cast<Client *>(ready_listDataPtr);
			// 	if (client)
			// 		client->getServer().removeClientConnection(client);
			// 	if (event_list[i].events & EPOLLRDHUP)
			// 		std::cout << RED BOLD << "EPOLLRDHUP on fd = " << ready_listDataPtr->getSocketFD() << RESET << std::endl;
			// 	if (event_list[i].events & EPOLLHUP)
			// 		std::cout << RED BOLD << "EPOLLHUP on fd = " << ready_listDataPtr->getSocketFD() << RESET << std::endl;
			// 	if (event_list[i].events & EPOLLERR)
			// 		std::cout << RED BOLD << "EPOLLERR on fd = " << ready_listDataPtr->getSocketFD() << RESET << std::endl;
			// 	// what if its error on server? remove this one and keep other servers running?
			// 	// epoll_ctl(_socketFD, EPOLL_CTL_DEL, ready_listDataPtr->getSocketFD(), &event_list[i]);
			// }
			// std::cout << "-------------------------" << std::endl;
		}
	}
	std::cout << "End of program" << std::endl;
}

void Epoll::setIsChildProcess(bool isChild)
{
	_isChildProcess = isChild;
}

std::ostream& operator<<(std::ostream& out_stream, const Epoll& Epoll)
{
	out_stream << CYAN BOLD "Epoll:" RESET << "\n_socketFD Epoll: " << Epoll.getSocketFD() << std::endl;
	return out_stream;
}
