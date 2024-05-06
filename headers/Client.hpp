/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mverbrug <mverbrug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 17:54:08 by felicia           #+#    #+#             */
/*   Updated: 2024/05/06 13:26:29 by mverbrug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef Client_HPP
# define Client_HPP

# include "ASocket.hpp"
# include "webserv.hpp"
# include <sys/socket.h> // ! needed? for accept()

class Client : public ASocket
{
	private:

	public:
		Client();
		~Client();
};

std::ostream& operator<<(std::ostream& out_stream, const Client& Client);

#endif