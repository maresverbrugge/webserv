/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: fkoolhov <fkoolhov@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/12/18 15:01:35 by mverbrug      #+#    #+#                 */
/*   Updated: 2024/04/25 17:04:27 by fhuisman      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "configuration.hpp"
#include "Request.hpp"

int main(int argc, char** argv)
{
	std::string http_request = "GET /index.html?flen=cool&mares=lief&felicia=mooi#section-3.6 HTTP/1.1\r\nheaderName: headerValue\r\n\t..continuation\r\n\t...CONTI\%4EUATIO\%4e\r\nheaderName2: headerValue2\r\nheaderName: otherValue\r\nheaderName3:\r\nHost: localhost:8080\r\n\r\nThis is the body of the request\r\nThis is the body of the request\r\n\r\n";
	
	if (argc != 2)
	{
		// handle error
		return (EXIT_FAILURE);
	}
	
	std::unique_ptr<ServerPool> serverpool = configure_serverpool(argv[1]);
	std::cout << *serverpool << std::endl; // for debugging purposes
	try 
	{
		Request request(http_request);
		std::cout << request << std::endl; // for debugging purposes
		//send response
	}
	catch (int error)
	{
		std::cout << error << std::endl; //for debugging purposes
		//send error response
	}
	return (EXIT_SUCCESS);
}