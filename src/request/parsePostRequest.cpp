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

static void parse_chunked_body(Request *request, std::stringstream& stringstream)
{
	try
	{
		int content_length = 0; // what if overflow?
		std::string body;
		std::string line;
	
		while (std::getline(stringstream, line))
		{
			if (line == "0")
				break;
			int chunk_size = std::stoi(line, nullptr, 0x16);
			if (chunk_size == 0)
				break;
			std::string chunk;
			chunk.resize(chunk_size);
			stringstream.read(&chunk[0], chunk_size);
			body += chunk;
			content_length += chunk_size;
			std::getline(stringstream, line); // read the empty line after each chunk
		}
		request->setContentLength(content_length);
		request->setBody(body);
	}
	catch (const std::exception& exception)
	{
		throw (BAD_REQUEST);
	}
}

static void parse_identity_body(Request *request, std::stringstream& stringstream)
{
	try
	{
		std::string body;
		body.resize(request->getContentLength());
		stringstream.read(&body[0], request->getContentLength());
		request->setBody(body);
	}
	catch (const std::exception& exception)
	{
		throw (BAD_REQUEST);
	}
}

static void get_content_length(Request *request, std::string transfer_encoding)
{
    std::map<std::string, std::string> headers = request->getHeaders();

    if (transfer_encoding == "" || transfer_encoding == "identity")
    {
        std::string content_length = "";
		auto it = headers.find("content-length");
		if (it != headers.end())
			content_length = it->second;
			
        if (content_length == "")
            throw (LENGTH_REQUIRED);
        else
            request->setContentLength(std::atoi(content_length.c_str()));
    }
    else request->setContentLength(-1);
}

static std::string verify_transfer_encoding(std::map<std::string, std::string> headers)
{
    std::string transfer_encoding = "";
	auto it = headers.find("transfer-encoding");
	if (it != headers.end())
		transfer_encoding = it->second;
    if (transfer_encoding != "" && transfer_encoding != "chunked" && transfer_encoding != "identity") 
		throw (NOT_IMPLEMENTED); // Not Implemented
	return transfer_encoding;
}

void Request::parsePostRequest(std::stringstream& stringstream)
{
    std::cout << "Parsing POST request" << std::endl;
	std::string transfer_encoding = verify_transfer_encoding(this->_headers);
    get_content_length(this, transfer_encoding);

	if (transfer_encoding == "chunked")
		parse_chunked_body(this, stringstream);
	else
		parse_identity_body(this, stringstream);
}