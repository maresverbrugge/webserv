/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkoolhov <fkoolhov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 15:01:35 by mverbrug          #+#    #+#             */
/*   Updated: 2024/05/13 13:39:38 by fkoolhov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "configuration.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "RequestHandler.hpp"
#include "ErrorHandler.hpp"

int main(int argc, char** argv)
{
	std::string http_request = "DELETE /cgi.html?mares=lief&felicia=mooi&flen=cool#section-3.6 HTTP/1.1\r\nheaderName: headerValue\r\n\t..continuation\r\n\t...CONTI\%4EUATIO\%4e\r\nContent-Length: 68\r\nheaderName: otherValue\r\nheaderName3:\r\nHost: localhost:8080\r\n\r\nThis is the body of the request\r\nThis is the body of the request\r\n\r\n";
	// std::string http_request = "POST /path HTTP/1.1\r\nHost: example.com\r\nTransfer-Encoding: chunked\r\n\r\n"
	// 								  "6\r\n"
	// 								  "Hello \r\n"
	// 								  "9\r\n"
	// 								  "World! \r\n\r\n"
	// 								  "7\r\n"
	// 								  "Chunks!\r\n"
	// 								  "0\r\n"
	// 								  "\r\n";
	// std::string http_request = "POST /path HTTP/1.1\r\nHost: example.com\r\nContent-Length: 16\r\n\r\n"
	// 						   "Hello World!\nyay\r\n";

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
		std::unique_ptr<RequestHandler> requestHandler = std::make_unique<RequestHandler>(*request, *serverpool->getServers().front());
		std::unique_ptr<Response> response = std::make_unique<Response>(*requestHandler);
		std::cout << *response << std::endl; // for for debugging purposes
	}
	catch (const e_status& statusCode)
	{
		std::unique_ptr<ErrorHandler> errorHandler = std::make_unique<ErrorHandler>(statusCode, *serverpool->getServers().front());
		std::unique_ptr<Response> response = std::make_unique<Response>(*errorHandler);
		std::cout << "statusCode: " << statusCode << std::endl; //for debugging purposes
		std::cout << *response << std::endl; // for for debugging purposes
	}
	return (EXIT_SUCCESS);
}