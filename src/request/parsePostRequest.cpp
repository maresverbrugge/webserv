/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parsePostRequest.cpp                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: fhuisman <fhuisman@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/01 11:01:45 by fhuisman      #+#    #+#                 */
/*   Updated: 2024/05/01 11:12:57 by fhuisman      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

static void get_body(std::stringstream &ss)
{
    std::string line;
    std::ofstream body;
    std::string filename = "./root/upload/body"; // make function to get the right name?

    std::getline(ss, line);
    if (line != "" && line != "\r")
        body.open(filename);
    while (line != "" && line != "\r")
    {
        trim_cr(line);
        body << line << std::endl;
        std::getline(ss, line);
    }
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
                throw (404);
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