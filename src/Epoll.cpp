/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Epoll.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mverbrug <mverbrug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 18:07:06 by felicia           #+#    #+#             */
/*   Updated: 2024/04/25 14:32:24 by mverbrug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Epoll.hpp"

Epoll::Epoll()
{
	std::cout << "Epoll constructor called" << std::endl;
	this->_fdEpoll = epoll_create(0);
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

Epoll::~Epoll()
{
	std::cout << "Epoll destructor called" << std::endl;
}

int Epoll::getfdEpoll() const
{
	return this->_fdEpoll;
}

std::ostream& operator<<(std::ostream& out_stream, const Epoll& Epoll)
{
	out_stream << CYAN BOLD "\nEpoll: " RESET << Epoll.getfdEpoll() << std::endl;
	return out_stream;
}