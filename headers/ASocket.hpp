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

#ifndef ASocket_HPP
# define ASocket_HPP

# include "webserv.hpp"
# include <netinet/in.h> // * for inet_ntop() only to print info - might remove?

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

		void	setSocketFD(int socket); // ? will we be using this?
		int		getSocketFD() const;
		virtual	~ASocket() = 0;

};

#endif