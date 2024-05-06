/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ASocket.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mverbrug <mverbrug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 12:50:12 by mverbrug          #+#    #+#             */
/*   Updated: 2024/05/06 13:26:02 by mverbrug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "ASocket.hpp"

ASocket::~ASocket()
{
	std::cout << "ASocket destructor called" << std::endl;
}

int ASocket::getSocketFD() const
{
	return this->_socketFD;
}