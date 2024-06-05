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

#ifndef CGI_HPP
# define CGI_HPP

# include "webserv.hpp"
# include "ASocket.hpp"

class Client;

class CGI : public ASocket
{
	private:
		Client&		_client;
		char**		_envp{};
		std::string	_script_string{};

	public:
		CGI(int pipe_fd, Client& client);
		CGI(int pipe_fd, Client& client, char** envp, std::string script_string);
		~CGI();

	Client&		getClient() const;
	char**		getEnvp() const;
	std::string	getScriptString() const;

	void run_script();
	void readFromPipe();

};

#endif