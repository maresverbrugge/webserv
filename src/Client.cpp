/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mverbrug <mverbrug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 13:20:46 by mverbrug          #+#    #+#             */
/*   Updated: 2024/04/30 15:18:55 by mverbrug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Client.hpp"

Client::Client()
{
	std::cout << "Client constructor called" << std::endl;
}

Client::~Client()
{
	// close(this->_ClientSocket); // ?
	std::cout << "Client destructor called" << std::endl;
}

// std::ostream& operator<<(std::ostream& out_stream, const Client& Client)
// {
// 	// out_stream << GREEN BOLD "\nClient: " RESET << Client.getHost() << ":" << Client.getPort() << std::endl;
// 	return out_stream;
// }