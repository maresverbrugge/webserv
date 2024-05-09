/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Epoll.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mverbrug <mverbrug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 17:54:08 by felicia           #+#    #+#             */
/*   Updated: 2024/05/09 13:39:17 by mverbrug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EPOLL_HPP
# define EPOLL_HPP

# include "ASocket.hpp"
# include <sys/epoll.h> // for epoll
# include <unistd.h> // for close

class Epoll : public ASocket
{
	private:
		// struct epoll_event _event{};
	public:
		Epoll();
		~Epoll();

		void addFDToEpoll(int event_to_poll_for, int fdToAdd);
		// ! OR:
		// void addFDToEpoll(ASocket *ptr, int event_to_poll_for, int fdToAdd);
		
		void delFDFromEpoll(int event_to_poll_for, int fdToDel);
		// ! OR:
		// void delFDFromEpoll(ASocket *ptr, int event_to_poll_for, int fdToAdd)

		void modFDInEpoll(int event_to_poll_for, int fdToMod);
		// ! OR:
		// void modFDInEpoll(ASocket *ptr, int event_to_poll_for, int fdToMod)

		void EpollWait();
};

std::ostream& operator<<(std::ostream& out_stream, const Epoll& Epoll);

#endif
