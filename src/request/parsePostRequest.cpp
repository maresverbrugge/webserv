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

static void add_file_headers_to_request(Request* request, std::string header)
{
	std::map<std::string, std::string> headers;
	std::stringstream header_stream(header);
	std::string line;
	std::getline(header_stream, line); // skip the empty line after the boundary?
	while (std::getline(header_stream, line))
	{
		size_t colon = line.find(":");
		if (colon == std::string::npos)
			throw (BAD_REQUEST);
		std::string key = line.substr(0, colon);
		std::string value = line.substr(colon + 2);
		std::transform(key.begin(), key.end(), key.begin(), ::tolower);
		request->setHeader(key, value);
	}
}

static std::string get_first_part(std::string body, std::string delimiter)
{
	size_t pos = body.find(delimiter);
	if (pos == std::string::npos)
		throw (BAD_REQUEST);
	pos += delimiter.size();
	size_t end = body.find(delimiter, pos);
	if (end == std::string::npos)
		throw (BAD_REQUEST);
	std::string first_part = body.substr(pos, end - pos);
	return first_part;
}

static std::string get_delimiter(Request* request)
{
	std::string boundary = "";
	std::map<std::string, std::string> headers = request->getHeaders();
	auto it = headers.find("content-type");
	if (it != headers.end())
	{
		std::string content_type = it->second;
		size_t pos = content_type.find("boundary=");
		if (pos != std::string::npos)
			boundary = content_type.substr(pos + 9);
	}
	if (boundary == "")
		throw (BAD_REQUEST);
	std::string delimiter = "--" + boundary;
	return delimiter;
}

static void parse_multipart_form_data(Request* request) // check if \r\n is handled correctly
{
	std::string delimiter = get_delimiter(request);
	std::string body = request->getBody();
	std::string first_part = get_first_part(body, delimiter);
	size_t header_end = first_part.find("\r\n\r\n");
	if (header_end == std::string::npos)
		throw (BAD_REQUEST);
	std::string header = first_part.substr(0, header_end);
	std::string content = first_part.substr(header_end + 4); 
	add_file_headers_to_request(request, header);
	request->setBody(content);
	request->setContentLength(content.size()); // not accidentally including \r\n? should this actually be the length of the entire body?
}

static void parse_chunked_body(Request* request, std::stringstream& stringstream)
{
	try
	{
		int content_length = 0; // what if overflow? choose a different type (ull)?
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

static void parse_identity_body(Request* request, std::stringstream& stringstream)
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

static void get_content_length(Request* request, std::string transfer_encoding)
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
		throw (NOT_IMPLEMENTED);
	return transfer_encoding;
}

void Request::parsePostRequest(std::stringstream& stringstream)
{
	std::string transfer_encoding = verify_transfer_encoding(this->_headers);
    get_content_length(this, transfer_encoding);

	// handle \0 in images?
	if (transfer_encoding == "chunked")
		parse_chunked_body(this, stringstream);
	else
		parse_identity_body(this, stringstream);

	std::string content_type = "";
	auto it = this->_headers.find("content-type");
	if (it != this->_headers.end())
		content_type = it->second;
	if (content_type == "")
		throw (BAD_REQUEST);
	if (content_type.find("multipart/form-data") != std::string::npos)
		parse_multipart_form_data(this);
}