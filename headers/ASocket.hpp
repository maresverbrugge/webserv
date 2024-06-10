/* ************************************************************************* */
/*		##       ##      ## ##       ##      ## ##       ##      ##     	 */
/*		 ##     ####    ##   ##     ####    ##   ##     ####    ##  		 */
/*		  ##  ##   ##  ##     ##  ##   ##  ##     ##  ##   ##  ##   		 */
/*		   ####     ####       ####     ####       ####     ####    		 */
/*		    ##       ##         ##       ##         ##       ##     		 */
/*																			 */
/*			 WONDERFUL			  WEBSERV			WONDERTEAM				 */
/* 																			 */
/*	 	FELICIA KOOLHOVEN	   FLEN HUISMAN		  MARES VERBRUGGE			 */
/*			fkoolhov			 fhuisman			  mverbrug				 */
/* 																			 */
/* 			Codam Coding College		part of 42 network					 */
/*							  April - May 2024								 */
/* ************************************************************************* */

#ifndef ASOCKET_HPP
# define ASOCKET_HPP

# include "webserv.hpp"

# include <netinet/in.h> // * for inet_ntop() only to print info - might remove?
# include <unistd.h>

class ASocket
{
	protected:
		int 	_socketFD{};

	public:
		// * ONLY TO PRINT INFO ON SOCKET - might remove later?
		void *addr{};
		std::string versionIP{};
		int portOfASocket{};
		char strIP[INET6_ADDRSTRLEN]{}; // needs enogh space to store IPv6 addresses
		
		// * END OF PRINT CODE
		virtual	~ASocket() = 0;

		void	setSocketFD(int socket); // ? will we be using this? No, but it's good to have a setter for each getter
		int		getSocketFD() const;

};

#endif