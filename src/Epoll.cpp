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
// # include "Server.hpp"
// # include "Client.hpp"

Epoll::Epoll()
{
	std::cout << "Epoll constructor called" << std::endl;
	_socketFD = epoll_create(1);
	if (_socketFD < 0)
	{
		throw std::runtime_error("Error creating epoll with epoll_create()");
		// close server sockets; // ?
		// exit?
	}
}

Epoll::~Epoll()
{
	close(_socketFD); // close Epoll socket
	std::cout << "Epoll destructor called" << std::endl;
}

// int Epoll::addFDToEpoll(int event_to_poll_for, int fdToAdd)
int Epoll::addFDToEpoll(ASocket *ptr, int event_to_poll_for, int fdToAdd)
{
	struct epoll_event event{};

	std::cout << "We're in addFDToEpoll and event_to_poll_for = " << event_to_poll_for << std::endl;
	event.events = event_to_poll_for;
	event.data.fd = fdToAdd; // ! does not seem to work, see epoll_wait loop. used to print info later, don't think we need it?
	event.data.ptr = ptr;
	return (epoll_ctl(_socketFD, EPOLL_CTL_ADD, fdToAdd, &event));
}

// int Epoll::delFDFromEpoll(ASocket *ptr, int event_to_poll_for, int fdToAdd)
int Epoll::delFDFromEpoll(int event_to_poll_for, int fdToDel)
{
	struct epoll_event event{};

	event.events = event_to_poll_for;
	// event.data.ptr = ptr; // ! gives extra info on custom data type, but do we really need this?
	return (epoll_ctl(_socketFD, EPOLL_CTL_DEL, fdToDel, &event));
}

// int Epoll::modFDInEpoll(ASocket *ptr, int event_to_poll_for, int fdToMod)
int Epoll::modFDInEpoll(int event_to_poll_for, int fdToMod)
{
	struct epoll_event event{};

	event.events = event_to_poll_for;
	// event.data.ptr = ptr; // ! gives extra info on custom data type, but do we really need this?
	return (epoll_ctl(_socketFD, EPOLL_CTL_MOD, fdToMod, &event));
}

void Epoll::EpollWait()
{
	// array of epoll_event structs
	struct epoll_event event_list[MAX_EVENTS];
    ASocket *ready_listDataPtr{};

	int epoll_return = epoll_wait(_socketFD, event_list, MAX_EVENTS, -1);
	if (epoll_return < 0)
	{
		throw std::runtime_error("Error epoll wait");
		// delete sockets?
	}
	// ? epoll_return = 0 -> indicates timeout but i think we never timeout because it is put to -1 in epoll_wait()
	// in case epoll_return > 0 (is the amount of file descriptors in the "ready list" in the "evlist")
	// FLAG = ready_to_read
	for (int i = 0; i < epoll_return; i++)
	{
		// TO TEST:
		std::cout << "epoll_return = " << epoll_return << std::endl;
		std::cout << "i = " << i << std::endl;
		// std::cout << "event_list[i].data.fd: " << event_list[i].data.fd << std::endl;
		// std::cout << "event_list[i].events: " << event_list[i].events << std::endl;
		ready_listDataPtr = static_cast<ASocket *>(event_list[i].data.ptr);
		std::cout << "event_list[i].data.ptr->_socketFD: " << ready_listDataPtr->getSocketFD() << std::endl;
		Server *server = dynamic_cast<Server *>(ready_listDataPtr);
		Client *client = dynamic_cast<Client *>(ready_listDataPtr);
		std::cout << "we're in epoll loop and event_list[i].events = " << event_list[i].events << std::endl;
		if (event_list[i].events == EPOLLIN && server != NULL)
		{
			std::cout << "this is a Server Class! We will now create a client class instance!" << std::endl;
			server->createNewClientConnection();
		}
		if (event_list[i].events == (EPOLLIN | EPOLLOUT) && client != NULL && (client->getReadyForFlag() == READ))
		{
			std::cout << "this is a Client Class with FLAG == READ! We will now start reading and parse the request!" << std::endl;
			client->clientReceives();
		}
		if (event_list[i].events == (EPOLLIN | EPOLLOUT) && client != NULL && (client->getReadyForFlag() == WRITE))
		{
			std::cout << "this is a Client Class with FLAG == WRITE! We will now start writing!" << std::endl;
			client->clientWrites();
		}
		std::cout << "-------------------------" << std::endl;



		// try to cast events[i].data.ptr to Server class or Client class
		// to find out on what kind of socket the EPOLLIN-event is happening.

		// if EPOLLIN on server socket:
			// accept connection with accept(), create client class
			// add new connection to epoll
		// if EPOLLIN on connection socket (client class):
			// use read() to read data
			// parse request
			// proces request, make response?
			// set FLAG = ready_to_write
		// if EPOLLOUT on connection socket && FLAG == ready_to_write:
			// use write() to write response back to client
			// after response, remove connection (client class) from epoll
			// after response, close connection (client class)
		// if EPOLLHUP on connection socket (client class):
			// remove connection (client class) from epoll
			// close connection (client class)
	}
}

std::ostream& operator<<(std::ostream& out_stream, const Epoll& Epoll)
{
	out_stream << CYAN BOLD "Epoll:" RESET << "\n_socketFD Epoll: " << Epoll.getSocketFD() << std::endl;
	return out_stream;
}
