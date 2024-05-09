/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Epoll.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mverbrug <mverbrug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 18:07:06 by felicia           #+#    #+#             */
/*   Updated: 2024/05/09 13:55:48 by mverbrug         ###   ########.fr       */
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
void Epoll::addFDToEpoll(int event_to_poll_for, int fdToAdd)
{
	struct epoll_event event{};
	
	event.events = event_to_poll_for;
	// event.data.ptr = ptr; // ! gives extra info on custom data type, but do we really need this?
	if (epoll_ctl(_socketFD, EPOLL_CTL_ADD, fdToAdd, &event) < 0)
		throw std::runtime_error("Error adding fd to epoll");
}

// void Epoll::delFDFromEpoll(ASocket *ptr, int event_to_poll_for, int fdToAdd)
void Epoll::delFDFromEpoll(int event_to_poll_for, int fdToDel)
{
	struct epoll_event event{};

	event.events = event_to_poll_for;
	// event.data.ptr = ptr; // ! gives extra info on custom data type, but do we really need this?
	if (epoll_ctl(_socketFD, EPOLL_CTL_DEL, fdToDel, &event) < 0)
		throw std::runtime_error("Error deleting fd to epoll");
}

// void Epoll::modFDInEpoll(ASocket *ptr, int event_to_poll_for, int fdToMod)
void Epoll::modFDInEpoll(int event_to_poll_for, int fdToMod)
{
	struct epoll_event event{};

	event.events = event_to_poll_for;
	// event.data.ptr = ptr; // ! gives extra info on custom data type, but do we really need this?
	if (epoll_ctl(_socketFD, EPOLL_CTL_MOD, fdToMod, &event) < 0)
		throw std::runtime_error("Error modifying fd in epoll");
}

void Epoll::EpollWait()
{
	// if (int epoll_wait(_socketFD, struct epoll_event *evlist, int maxevents, int timeout) < 0)
		// handle error;


	// check for:
	//		-1 (error)
	//		0  (time out)
	//		> 0 (is the amount of file descriptors in the "ready list" in the "evlist")
}

std::ostream& operator<<(std::ostream& out_stream, const Epoll& Epoll)
{
	out_stream << CYAN BOLD "Epoll:" RESET << "\n_socketFD Epoll: " << Epoll.getSocketFD() << std::endl;
	return out_stream;
}
