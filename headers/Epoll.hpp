/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Epoll.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mverbrug <mverbrug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 17:54:08 by felicia           #+#    #+#             */
/*   Updated: 2024/05/13 13:43:53 by mverbrug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EPOLL_HPP
# define EPOLL_HPP

# include "ASocket.hpp"
# include <sys/epoll.h> // for epoll
# include <unistd.h> // for close

// Define the maximum number of events to be returned from epoll_wait()
# define MAX_EVENTS 10

class Epoll : public ASocket
{
	private:
		// struct epoll_event _event{};
	public:
		Epoll();
		~Epoll();

		int addFDToEpoll(int event_to_poll_for, int fdToAdd);
		// ! OR:
		// int addFDToEpoll(ASocket *ptr, int event_to_poll_for, int fdToAdd);
		
		int delFDFromEpoll(int event_to_poll_for, int fdToDel);
		// ! OR:
		// int delFDFromEpoll(ASocket *ptr, int event_to_poll_for, int fdToAdd)

		int modFDInEpoll(int event_to_poll_for, int fdToMod);
		// ! OR:
		// int modFDInEpoll(ASocket *ptr, int event_to_poll_for, int fdToMod)

		void EpollWait();
};

std::ostream& operator<<(std::ostream& out_stream, const Epoll& Epoll);

#endif


/*
	
if (addFDToEpoll < 0)
	throw std::runtime_error("Error adding fd to epoll");

if (delFDFromEpoll < 0)
	throw std::runtime_error("Error deleting fd to epoll");

if (modFDInEpoll < 0)
	throw std::runtime_error("Error modifying fd in epoll");

*/