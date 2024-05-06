/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Epoll.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mverbrug <mverbrug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 17:54:08 by felicia           #+#    #+#             */
/*   Updated: 2024/05/06 15:17:50 by mverbrug         ###   ########.fr       */
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

	public:
		Epoll();
		~Epoll();

		void	EpollCTL();
		void	EpollWait();
};

std::ostream& operator<<(std::ostream& out_stream, const Epoll& Epoll);

#endif