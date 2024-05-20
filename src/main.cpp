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

# include "webserv.hpp"
# include "configuration.hpp"
# include "Request.hpp"
# include "Response.hpp"
# include "Epoll.hpp"

// int g_state = 1;

// void siginthandler(int signum)
// {
//     if (signum == SIGINT)
//     {
//         g_state = 0;
//     }
// }

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
	Epoll& epoll_instance = serverpool->getEpollInstance();
	std::cout << epoll_instance << std::endl; // for debugging purposes
	// try
	// {
	// 	std::unique_ptr<Request> request = std::make_unique<Request>(http_request);
	// 	std::unique_ptr<Response> response = std::make_unique<Response>(std::move(request));
	// 	std::cout << *(response->getRequest()) << std::endl; // for for debugging purposes
	// 	std::cout << *response << std::endl; // for for debugging purposes
	// 	response->constructResponseMessage(); // function to fill in, should construct http response string
	// 	std::string http_response = response->getResponseMessage();
	// 	std::cout << "Response: " << http_response << std::endl; // for for debugging purposes
	// }
	// catch (int statusCode)
	// {
	// 	std::unique_ptr<Response> response = std::make_unique<Response>((short) statusCode);
	// 	std::cout << "statusCode: " << statusCode << std::endl; //for debugging purposes
	// 	std::cout << *response << std::endl; // for for debugging purposes
	// 	response->constructResponseMessage(); // function to fill in, should construct http response string
	// 	std::string http_response = response->getResponseMessage();
	// 	std::cout << "Response: " << http_response << std::endl; // for for debugging purposes
	// }

	epoll_instance.EpollWait();
    // close(epoll_instance.getSocketFD());

	return (EXIT_SUCCESS);
}
