
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

int Epoll::addFDToEpoll(ASocket *ptr, int event_to_poll_for, int fdToAdd)
{
	struct epoll_event event{};

	event.events = event_to_poll_for;
	event.data.ptr = ptr;
	return (epoll_ctl(_socketFD, EPOLL_CTL_ADD, fdToAdd, &event));
}

// ! we probably won't need this:
int Epoll::delFDFromEpoll(int fdToDel)
{
	struct epoll_event event{};
	return (epoll_ctl(_socketFD, EPOLL_CTL_DEL, fdToDel, &event));
}

// ! we probably won't need this:
int Epoll::modFDInEpoll(ASocket *ptr, int event_to_poll_for, int fdToMod)
{
	struct epoll_event event{};

	event.events = event_to_poll_for;
	event.data.ptr = ptr;
	return (epoll_ctl(_socketFD, EPOLL_CTL_MOD, fdToMod, &event));
}

void Epoll::EpollWait()
{
	// array of epoll_event structs
	struct epoll_event event_list[MAX_EVENTS];
    ASocket *ready_listDataPtr{};

	while (1)
	{
		// std::cout << "we're passing epoll_wait again!\n";
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
			ready_listDataPtr = static_cast<ASocket *>(event_list[i].data.ptr);
			Server *server = dynamic_cast<Server *>(ready_listDataPtr);
			Client *client = dynamic_cast<Client *>(ready_listDataPtr);

			// TO TEST:
			// std::cout << "epoll_return = " << epoll_return << std::endl;
			// std::cout << "i = " << i << std::endl;
			// std::cout << "event_list[i].data.ptr->_socketFD: " << ready_listDataPtr->getSocketFD() << std::endl;
			// if (client != NULL)
			// {
				// std::cout << "This fd is a client\n";
				// if (client->getReadyForFlag() == READ)
					// std::cout << "Client->getReadyForFlag() == READ\n";
				// else
					// std::cout << "Client->getReadyForFlag() == WRITE\n";
			// }
			// std::cout << "event_list[i].events = " << event_list[i].events << std::endl;
			// if (event_list[i].events == 1)
				// std::cout << "This means EPOLLIN\n";
			// else if (event_list[i].events == 4)
			// 	std::cout << "This means EPOLLOUT\n";
			// else if (event_list[i].events == 5)
			// 	std::cout << "This means EPOLLIN | EPOLLOUT\n";
			// else
			// 	std::cout << RED BOLD "attention for this epoll event mrazzle \n";
			// END OF TEST

			if (event_list[i].events & EPOLLIN && server != NULL)
			{
				// std::cout << "EPOLLIN on a Server Class! We will now create a client class instance!" << std::endl;
				server->createNewClientConnection();
			}
			else if (client != NULL)
			{
				if ((event_list[i].events & EPOLLIN) && (client->getReadyForFlag() == READ))
				{
					// std::cout << "EPOLLIN on a Client Class with FLAG == READ! We will now start receiving and parse the request!" << std::endl;
					client->clientReceives();
				}
				else if ((event_list[i].events & EPOLLOUT) && (client->getReadyForFlag() == WRITE))
				{
					// std::cout << "EPOLLOUT on a Client Class with FLAG == WRITE! We will now start writing!" << std::endl;
					client->clientWrites();
					// if whole response is send, remove client from epoll
					epoll_ctl(_socketFD, EPOLL_CTL_DEL, client->getSocketFD(), &event_list[i]);
					delete client;
				}
			}
			// std::cout << "-------------------------" << std::endl;
		}
	}
	// std::cout << "End of program" << std::endl;
		// try to cast events[i].data.ptr to Server class or Client class
		// to find out on what kind of socket the EPOLLIN-event is happening.

		// if EPOLLIN on server socket:
			// accept connection with accept(), create client class
			// add new connection to epoll
		// if EPOLLIN on connection socket (client class):
			// use read() to read data
			// parse request
			// proces request: make response or error
			// set FLAG = ready_to_write
		// if EPOLLOUT on connection socket && FLAG == ready_to_write:
			// use send() to write response back to client
			// after response, remove connection (client class) from epoll
			// ! after response, close connection (client class) -> ADD signals and check if client destructor is called and connection is closed
		// if EPOLLHUP on connection socket (client class):
			// remove connection (client class) from epoll
			// close connection (client class)
		
		// end of program:
		// remove all servers from epoll:
		// epoll_ctl(_socketFD, EPOLL_CTL_DEL, server->getSocketFD(), &event_list[i]);
}

std::ostream& operator<<(std::ostream& out_stream, const Epoll& Epoll)
{
	out_stream << CYAN BOLD "Epoll:" RESET << "\n_socketFD Epoll: " << Epoll.getSocketFD() << std::endl;
	return out_stream;
}
