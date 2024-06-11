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
# include "AFileDescriptor.hpp"

class Client;

class CGI : public AFileDescriptor
{
	private:
		Client&				_client;
		char**				_envp{};
		const std::string	_script_string{};
		const std::string	_extension;

	public:
		CGI(int read_end, Client& client);
		CGI(int write_end, Client& client, char** envp, std::string script_string, std::string extension);
		~CGI();

		Client&				getClient() const;
		char**				getEnvp() const;
		const std::string	getScriptString() const;
		const std::string	getExtension() const;

		void	setEnvp(char **envp);

		int readFromPipe();
};

#endif