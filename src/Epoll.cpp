/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Epoll.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mverbrug <mverbrug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 18:07:06 by felicia           #+#    #+#             */
/*   Updated: 2024/05/09 15:32:40 by mverbrug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Epoll.hpp"

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

// void Epoll::addFDToEpoll(ASocket *ptr, int event_to_poll_for, int fdToAdd)
int Epoll::addFDToEpoll(int event_to_poll_for, int fdToAdd)
{
	struct epoll_event event{};
	
	event.events = event_to_poll_for;
	// event.data.ptr = ptr; // ! gives extra info on custom data type, but do we really need this?
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
	struct epoll_event events[MAX_EVENTS];

	int epoll_return = epoll_wait(_socketFD, events, MAX_EVENTS, -1);
	if (epoll_return < 0) // -1 (error)
	{
		throw std::runtime_error("Error epoll wait");
		// delete sockets?
	}
	// ? epoll_return = 0 -> indicates timeout but i think we never timeout because it is put to -1 in epoll_wait()
	// in case epoll_return > 0 (is the amount of file descriptors in the "ready list" in the "evlist")
	// FLAG = ready_to_read
	for (int i = 0; i < epoll_return; i++)
	{
		// try to cast events[i].data.ptr to Server class or Client class
		// to find out on what kind of socket is ready to reathe EPOLLIN-event is happening.
		
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
		// if EPOLLHUP of connection socket (client class):
			// remove connection (client class) from epoll
			// close connection (client class)
	}
}

std::ostream& operator<<(std::ostream& out_stream, const Epoll& Epoll)
{
	out_stream << CYAN BOLD "Epoll:" RESET << "\n_socketFD Epoll: " << Epoll.getSocketFD() << std::endl;
	return out_stream;
}
