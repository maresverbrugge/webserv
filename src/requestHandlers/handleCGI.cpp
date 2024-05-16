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

#include "RequestHandler.hpp"

// char **getEnv(std::string query)
// {
    
// }

void RequestHandler::handleCGI()
{
    std::cout << "handleCGI called" << std::endl;
    //get variables from the quesry string -> worden ENV variables in new proces
    //start a new process
    //that process writes its output on the standard output (cout <<)
    //i think the read end of the pipe is added to epoll
    //ePoll will call the event and the response can be generated.
    //
}