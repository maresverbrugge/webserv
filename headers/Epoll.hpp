/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Epoll.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mverbrug <mverbrug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 17:54:08 by felicia           #+#    #+#             */
/*   Updated: 2024/04/25 14:16:23 by mverbrug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EPOLL_HPP
# define EPOLL_HPP

# include "webserv.hpp"
# include <sys/epoll.h> // for epoll

class Epoll
{
	private:
		int	_fdEpoll;

	public:
		Epoll();
		~Epoll();

		int		getfdEpoll() const;

		void	EpollCTL();
		void	EpollWait();
};

std::ostream& operator<<(std::ostream& out_stream, const Epoll& Epoll);

#endif