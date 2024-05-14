/* ************************************************************************* */
/*      ##       ##      ## ##       ##      ## ##       ##      ##          */
/*       ##     ####    ##   ##     ####    ##   ##     ####    ##           */
/*        ##  ##   ##  ##     ##  ##   ##  ##     ##  ##   ##  ##            */
/*         ####     ####       ####     ####       ####     ####             */
/*          ##       ##         ##       ##         ##       ##              */
/*                                                                           */
/*           WONDERFUL            WEBSERV           WONDERTEAM               */
/*                                                                           */
/*      FELICIA KOOLHOVEN      FLEN HUISMAN       MARES VERBRUGGE            */
/*          fkoolhov             fhuisman             mverbrug               */
/*                                                                           */
/*          Codam Coding College        part of 42 network                   */
/*                            April - May 2024                               */
/* ************************************************************************* */

#include "webserv.hpp"
#include "configuration.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "RequestHandler.hpp"
#include "ErrorHandler.hpp"

int main(int argc, char** argv)
{
	// std::string http_request = "DELETE /cgi.html?mares=lief&felicia=mooi&flen=cool#section-3.6 HTTP/1.1\r\nheaderName: headerValue\r\n\t..continuation\r\n\t...CONTI\%4EUATIO\%4e\r\nContent-Length: 68\r\nheaderName: otherValue\r\nheaderName3:\r\nHost: localhost:8080\r\n\r\nThis is the body of the request\r\nThis is the body of the request\r\n\r\n";
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
	std::string http_request = "POST /upload HTTP/1.1\r\n"
							   "Host: example.com\r\n"
							   "Content-Type: multipart/form-data; boundary=1234567890\r\n"
							   "Content-Length: 304\r\n"
							   "\r\n"
							   "--1234567890\r\n"
							   "Content-Disposition: form-data; name=\"file1\"; filename=\"example1.txt\"\r\n"
							   "Content-Type: text/plain\r\n"
							   "\r\n"
							   "This is the content of file 1.\r\n"
							   "--1234567890\r\n"
							   "Content-Disposition: form-data; name=\"file2\"; filename=\"example2.txt\"\r\n"
							   "Content-Type: text/plain\r\n"
							   "\r\n"
							   "This is the content of file 2.\r\n"
							   "--1234567890--\r\n";

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