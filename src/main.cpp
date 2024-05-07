/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: fkoolhov <fkoolhov@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/12/18 15:01:35 by mverbrug      #+#    #+#                 */
/*   Updated: 2024/05/07 12:15:12 by fhuisman      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "configuration.hpp"
#include "Request.hpp"
#include "Response.hpp"

int main(int argc, char** argv)
{
	std::string http_request = "GET /cgi.html?mares=lief&felicia=mooi&flen=cool#section-3.6 HTTP/1.1\r\nheaderName: headerValue\r\n\t..continuation\r\n\t...CONTI\%4EUATIO\%4e\r\nContent-Length: 68\r\nheaderName: otherValue\r\nheaderName3:\r\nHost: localhost:8080\r\n\r\nThis is the body of the request\r\nThis is the body of the request\r\n\r\n";

	if (argc != 2)
	{
		// handle error
		return (EXIT_FAILURE);
	}
	
	std::unique_ptr<ServerPool> serverpool = configure_serverpool(argv[1]);
	std::cout << *serverpool << std::endl; // for debugging purposes
	try 
	{
		std::unique_ptr<Request> request = std::make_unique<Request>(http_request);
		std::cout << *request << std::endl; // for for debugging purposes
		std::unique_ptr<Response> response = std::make_unique<Response>(std::move(request), (serverpool->getServers()).front());
		std::cout << *response << std::endl; // for for debugging purposes
	}
	catch (int statusCode)
	{
		std::unique_ptr<Response> response = std::make_unique<Response>((short) statusCode, (serverpool->getServers()).front());
		std::cout << "statusCode: " << statusCode << std::endl; //for debugging purposes
		std::cout << *response << std::endl; // for for debugging purposes
	}
	return (EXIT_SUCCESS);
}