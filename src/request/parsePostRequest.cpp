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
	try
	{
		std::map<std::string, std::string> headers;
		std::stringstream header_stream(header);
		std::string line;
		std::getline(header_stream, line);
		while (std::getline(header_stream, line))
		{
			size_t colon = line.find(":");
			if (colon == std::string::npos)
				throw(BAD_REQUEST);
			std::string key = line.substr(0, colon);
			std::string value = line.substr(colon + 2);
			std::transform(key.begin(), key.end(), key.begin(), ::tolower);
			request->setHeader(key, value);
		}
	}
	catch (const std::exception& exception)
	{
		std::string exception_message = exception.what();
		throw_error("Couldn't parse multipart request headers: " + exception_message, BAD_REQUEST);
	}
}

static std::vector<char> get_first_part(std::vector<char> body, std::string delimiter)
{
	auto pos = std::search(body.begin(), body.end(), delimiter.begin(), delimiter.end());
	if (pos == body.end())
		throw_error("No body found for multipart request", BAD_REQUEST);
	auto start = pos + delimiter.size();
	auto end = std::search(start, body.end(), delimiter.begin(), delimiter.end());
	if (end == body.end())
		throw_error("No delimiter found for in request", BAD_REQUEST);

	std::vector<char> first_part(start, end);
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
	if (boundary.empty())
		throw_error("No boundary found for multipart request", BAD_REQUEST);
	std::string delimiter = "--" + boundary;
	return delimiter;
}

static void parse_multipart_form_data(Request* request) // check if \r\n is handled correctly
{
	std::string delimiter = get_delimiter(request);
	std::vector<char> body = request->getBody();
	std::vector<char> first_part = get_first_part(body, delimiter);

	std::string header_end_marker = "\r\n\r\n";
	std::vector<char>::iterator header_end = std::search(first_part.begin(), first_part.end(), header_end_marker.begin(), header_end_marker.end());
	if (header_end == first_part.end())
		throw_error("No header found for multipart request", BAD_REQUEST);

	std::vector<char> header(first_part.begin(), header_end);
	std::vector<char> content(header_end + header_end_marker.size(), first_part.end());

	add_file_headers_to_request(request, std::string(header.begin(), header.end()));
	request->setBody(content);
}

static void parse_chunked_body(Request* request, unsigned long body_start, std::string request_string) // test if this works somehow???
{
	try
	{
		std::vector<char> body;
		size_t current_position = body_start;
		size_t buffer_end = request_string.size();

		while (current_position < buffer_end)
		{
			size_t line_end = request_string.find('\n', current_position);
			if (line_end == std::string::npos)
				break;

			std::string line = request_string.substr(current_position, line_end - current_position);
			if (line == "0")
				break;

			int chunk_size = std::stoi(line, nullptr, 16);
			if (chunk_size == 0)
				break;

			current_position = line_end + 1; // skip the newline character?
			if (current_position + chunk_size > buffer_end)
				throw_error("Chunk size exceeds buffer size", BAD_REQUEST);

			std::vector<char> chunk(request_string.begin() + current_position, request_string.begin() + current_position + chunk_size);
			body.insert(body.end(), chunk.begin(), chunk.end());

			current_position += chunk_size;
			if (current_position < buffer_end && request_string[current_position] == '\r')
				current_position++;
			if (current_position < buffer_end && request_string[current_position] == '\n')
				current_position++;
		}
		request->setBody(body);
	}
	catch (const std::exception& exception)
	{
		std::string exception_message = exception.what();
		throw_error("Couldn't parse chunked body: " + exception_message, BAD_REQUEST);
	}
}

static void parse_identity_body(Request* request, unsigned long body_start, std::string request_string)
{
	try
	{
		if (request_string.size() - body_start != request->getContentLength())
			throw_error("Content length incorrect", BAD_REQUEST);
		std::vector<char> body(request_string.begin() + body_start, request_string.end());
		if (body.size() != request->getContentLength())
			throw_error("Content length incorrect", BAD_REQUEST);
		request->setBody(body);
	}
	catch (const std::exception& exception)
	{
		std::string exception_message = exception.what();
		throw_error("Couldn't parse identity body: " + exception_message, BAD_REQUEST);
	}
}

static void get_content_length(Request* request, std::string transfer_encoding)
{
	std::map<std::string, std::string> headers = request->getHeaders();

	if (transfer_encoding.empty() || transfer_encoding == "identity")
	{
		std::string content_length = "";
		auto it = headers.find("content-length");
		if (it != headers.end())
			content_length = it->second;
			
		if (content_length.empty())
			throw (LENGTH_REQUIRED);
		else
			request->setContentLength(std::stoll(content_length.c_str()));
	}
}

static std::string verify_transfer_encoding(std::map<std::string, std::string> headers)
{
	std::string transfer_encoding = "";
	auto it = headers.find("transfer-encoding");
	if (it != headers.end())
		transfer_encoding = it->second;
	if (transfer_encoding != "" && transfer_encoding != "chunked" && transfer_encoding != "identity") 
		throw_error("Invalid transfer encoding", NOT_IMPLEMENTED);
	return transfer_encoding;
}

void Request::parsePostRequest(std::stringstream& stringstream, std::string request)
{
	std::string transfer_encoding = verify_transfer_encoding(this->_headers);
	get_content_length(this, transfer_encoding);
	unsigned long body_start = static_cast<int>(stringstream.tellg());

	if (transfer_encoding == "chunked")
		parse_chunked_body(this, body_start, request); // test if this works somehow???
	else
		parse_identity_body(this, body_start, request);

	auto it = this->_headers.find("content-type");
	if (it != this->_headers.end())
	{
		std::string content_type = it->second;
		if (content_type.find("multipart/form-data") != std::string::npos)
			parse_multipart_form_data(this);
	}
}