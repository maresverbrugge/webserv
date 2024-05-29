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

void RequestHandler::handleDeleteRequest()
{
    std::filesystem::path filePath(_absPath);

    if (std::filesystem::exists(filePath))
    {
        if (_absPath == _location.getPath())
        {
            setStatusCode(FORBIDDEN);
            setBody("{\r\n\t\"success\": false,\r\n\t\"message\": \"Acces denied.\"\r\n}");
        }
        else if (std::filesystem::remove_all(filePath))
        {
            setStatusCode(OK);
            setBody("{\r\n\t\"success\": true,\r\n\t\"message\": \"File deleted successfully.\"\r\n}");
        }
        else
            throw (INTERNAL_SERVER_ERROR);
    }
    else
    {
        setStatusCode(NOT_FOUND);
        setBody("{\r\n\t\"success\": false,\r\n\t\"message\": \"The file you are trying to delete doesn\'t exist.\"\r\n}");
    }
    addHeader("Content-Type", "application/json");
}
