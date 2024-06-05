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

#ifndef SIGNAL_HPP
# define SIGNAL_HPP

# include "webserv.hpp"
# include "ASocket.hpp"

# include <csignal>
# include <sys/signalfd.h>

class Signal : public ASocket
{
	private:

	public:
		Signal();
		Signal(const Signal&) = delete;
        Signal& operator=(const Signal&) = delete;
		~Signal();

		void readSignal();
};

#endif
