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

#ifndef AFileDescriptor_HPP
# define AFileDescriptor_HPP

# include "webserv.hpp"

# include <unistd.h>

class AFileDescriptor
{
	protected:
		int	_FD{};

	public:
		virtual	~AFileDescriptor() = 0;

		int	getFD() const;
		void setFD(int fd) = delete;
};

#endif