/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Epoll.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mverbrug <mverbrug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 18:07:06 by felicia           #+#    #+#             */
/*   Updated: 2024/05/06 15:14:49 by mverbrug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Epoll.hpp"

Epoll::Epoll()
{
	std::cout << "Epoll constructor called" << std::endl;
	this->_socketFD = epoll_create(1);
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

void Epoll::EpollCTL()
{
	// if (int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event) < 0)
		// handle error;
}

void Epoll::EpollWait()
{
	// if (int epoll_wait(int epfd, struct epoll_event *evlist, int maxevents, int timeout) < 0)
		// handle error;
}

std::ostream& operator<<(std::ostream& out_stream, const Epoll& Epoll)
{
	out_stream << CYAN BOLD "Epoll: \n" RESET << "_socketFD Epoll: " << Epoll.getSocketFD() << std::endl;
	return out_stream;
}
