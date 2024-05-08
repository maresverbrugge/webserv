/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ASocket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mverbrug <mverbrug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 17:54:08 by felicia           #+#    #+#             */
/*   Updated: 2024/04/30 16:52:24 by mverbrug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ASocket_HPP
# define ASocket_HPP

# include "webserv.hpp"
# include <netinet/in.h> // * for inet_ntop() only to print info - might remove?

class ASocket
{
	protected:
		int 	_socketFD{};

	public:
		// * ONLY TO PRINT INFO ON SOCKET - might remove later?
		void *addr{};
		std::string versionIP{};
		int port{};
		char strIP[INET6_ADDRSTRLEN]{}; // needs enogh space to store IPv6 addresses
		// * END OF PRINT CODE

		void	setSocketFD(int socket); // ? will we be using this?
		int		getSocketFD() const;
		virtual	~ASocket() = 0;

};

#endif