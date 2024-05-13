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

#include "Request.hpp"

static void get_body(std::stringstream &ss)
{
    std::string line;
    std::ofstream body;
    std::string filename = "./root/upload/body"; // make function to get the right name?

    std::getline(ss, line);
    if (line != "" && line != "\r")
        body.open(filename);
    //check if file is open?
    while (line != "" && line != "\r")
    {
        trim_cr(line);
        body << line << std::endl;
        std::getline(ss, line);
    }
    body.close();
}

static void get_content_length(Request *request)
{
    std::string content_lenth;
    auto headers = request->getHeaders();
    auto it = headers.find("content-encoding");
    if (it == headers.end() || (*it).second == "identity")
    {
        it = headers.find("content-length");
        if (it == headers.end())
        {
            if (request->getMethod() == POST)
                throw (411);
        }
        else
        {
            content_lenth = (*it).second;
            request->setContentLength(std::atoi(content_lenth.c_str()));
        }
    }
    else request->setContentLength(-1);
}

void Request::parsePostRequest(std::stringstream& ss)
{
    get_body(ss);
    get_content_length(this);
}