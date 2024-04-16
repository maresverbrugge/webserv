/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkoolhov <fkoolhov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 15:01:35 by mverbrug          #+#    #+#             */
/*   Updated: 2024/04/16 15:20:54 by fkoolhov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		// handle error
		return (EXIT_FAILURE);
	}
	
	configure_servers(argv[1]);
	return (EXIT_SUCCESS);
}