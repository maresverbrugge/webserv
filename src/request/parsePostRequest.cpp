/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsePostRequest.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkoolhov <fkoolhov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 11:01:45 by fhuisman          #+#    #+#             */
/*   Updated: 2024/05/08 18:48:11 by fkoolhov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

static void parse_chunked_body(std::stringstream& stringstream, std::ofstream& outfile)
{
	std::cout << "Parsing chunked body" << std::endl; // for debugging purposes
	std::string line;
	
	try
	{
		while (std::getline(stringstream, line))
		{
			if (line == "0")
			{
				break;
			}
			std::cout << "line: " << line << std::endl; // for debugging purposes
			int chunk_size = std::stoi(line, nullptr, 0x16);
			if (chunk_size == 0)
			{
				break;
			}
			std::string chunk;
			chunk.resize(chunk_size);
			stringstream.read(&chunk[0], chunk_size);
			outfile.write(chunk.c_str(), chunk_size);
			std::getline(stringstream, line); // read the empty line after each chunk
		}
		outfile.close();
	}
	catch(const std::exception& exception)
	{
		outfile.close();
		throw (400); // bad request (for example if stoi fails)
	}
}

static void parse_identity_body(std::stringstream& stringstream, std::ofstream& outfile, int content_length)
{
	std::cout << "Parsing identity body" << std::endl; // for debugging purposes
	std::string line;
	
	try
	{
		std::string body;
		body.resize(content_length);
		stringstream.read(&body[0], content_length);
		outfile.write(body.c_str(), content_length);
    	outfile.close();
	}
	catch(const std::exception& exception)
	{
		outfile.close();
		throw (400); // Bad Request
	}
}

static void get_content_length(Request *request)
{
    auto headers = request->getHeaders();
    std::string transfer_encoding = headers["transfer-encoding"];

    if (transfer_encoding == "" || transfer_encoding == "identity")
    {
        std::string content_length = headers["content-length"];
        if (content_length == "")
        {
            throw (411); // Length Required
        }
        else
        {
            request->setContentLength(std::atoi(content_length.c_str()));
        }
    }
    else request->setContentLength(-1);
}

void Request::parsePostRequest(std::stringstream& stringstream)
{
    std::cout << "Parsing POST request" << std::endl;
    get_content_length(this);

    std::string transfer_encoding = this->_headers["transfer-encoding"];
    if (transfer_encoding != "" && transfer_encoding != "chunked" && transfer_encoding != "identity") 
	{
    	throw (501); // Not Implemented
	}

	std::string filename = "./root/upload/body"; // make function to get the right name?
	std::ofstream outfile;

	outfile.open(filename);
	if (!outfile.is_open())
	{
		throw (500); // Internal server error
	}

	if (transfer_encoding == "chunked")
	{
		parse_chunked_body(stringstream, outfile);
	}
	else
	{
		parse_identity_body(stringstream, outfile, this->_contentLength);
	}
}