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

static void remove_trailing_newline(std::vector<char>& body)
{
	if (!body.empty() && body.back() == '\n')
		body.pop_back();
	if (!body.empty() && body.back() == '\r')
		body.pop_back();
}

static void add_file_headers_to_request(Request* request, std::string header)
{
	try
	{
		std::stringstream header_stream(header);
		std::string line;
		std::getline(header_stream, line);
		while (std::getline(header_stream, line))
		{
			size_t colon = line.find(":");
			if (colon == std::string::npos)
				break;
			std::string key = line.substr(0, colon);
			std::string value = line.substr(colon + 2);
			std::transform(key.begin(), key.end(), key.begin(), ::tolower);
			request->setHeader(key, value);
		}
	}
	catch (const std::exception& exception)
	{
		std::string exception_message = exception.what();
		throw StatusCodeException("Couldn't parse multipart request headers: " + exception_message, BAD_REQUEST);
	}
}

static std::vector<char> get_first_part(std::vector<char> body, std::string delimiter)
{
	auto pos = std::search(body.begin(), body.end(), delimiter.begin(), delimiter.end());
	if (pos == body.end())
		throw StatusCodeException("No body found for multipart request", BAD_REQUEST);
	auto start = pos + delimiter.size();
	auto end = std::search(start, body.end(), delimiter.begin(), delimiter.end());
	if (end == body.end())
		throw StatusCodeException("No delimiter found for in request", BAD_REQUEST);

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
		throw StatusCodeException("No boundary found for multipart request", BAD_REQUEST);
	std::string delimiter = "--" + boundary;
	return delimiter;
}

static void parse_multipart_form_data(Request* request)
{
	std::string delimiter = get_delimiter(request);
	std::vector<char> body = request->getBody();
	std::vector<char> first_part = get_first_part(body, delimiter);

	std::string header_end_marker = "\r\n\r\n";
	std::vector<char>::iterator header_end = std::search(first_part.begin(), first_part.end(), header_end_marker.begin(), header_end_marker.end());
	if (header_end == first_part.end())
		throw StatusCodeException("No header found for multipart request", BAD_REQUEST);
	header_end += header_end_marker.size();

	std::vector<char> header(first_part.begin(), header_end);
	std::vector<char> content(header_end, first_part.end());

	add_file_headers_to_request(request, std::string(header.begin(), header.end()));
	remove_trailing_newline(content);
	request->setBody(content);
}

static void parse_chunked_body(Request* request, unsigned long body_start, std::string buffer)
{
	try
	{
		std::vector<char> body;
		size_t position = body_start;
		size_t buffer_end = buffer.size();

		while (position < buffer_end)
		{
			size_t line_end = buffer.find('\n', position);
			if (line_end == std::string::npos)
				break;

			std::string line = buffer.substr(position, line_end - position);
			if (line == "0")
				break;

			int chunk_size = std::stoi(line, nullptr, 16);
			if (chunk_size == 0)
				break;

			position = line_end + strlen("\n");
			if (position + chunk_size > buffer_end)
				throw StatusCodeException("Chunk size exceeds buffer size", BAD_REQUEST);

			std::vector<char> chunk(buffer.begin() + position, buffer.begin() + position + chunk_size);
			body.insert(body.end(), chunk.begin(), chunk.end());

			position += chunk_size;
			if (position < buffer_end && buffer[position] == '\r')
				position++;
			if (position < buffer_end && buffer[position] == '\n')
				position++;
		}
		request->setBody(body);
	}
	catch (const std::exception& exception)
	{
		std::string exception_message = exception.what();
		throw StatusCodeException("Couldn't parse chunked body: " + exception_message, BAD_REQUEST);
	}
}

static void parse_identity_body(Request* request, unsigned long body_start, std::string buffer)
{
	try
	{
		if (buffer.size() - body_start != request->getContentLength())
			throw StatusCodeException("Content length incorrect", BAD_REQUEST);
		std::vector<char> body(buffer.begin() + body_start, buffer.end());
		if (body.size() != request->getContentLength())
			throw StatusCodeException("Content length incorrect", BAD_REQUEST);
		remove_trailing_newline(body);
		request->setBody(body);
	}
	catch (const std::exception& exception)
	{
		std::string exception_message = exception.what();
		throw StatusCodeException("Couldn't parse identity body: " + exception_message, BAD_REQUEST);
	}
}

void Request::parsePostRequest(std::string buffer)
{
	unsigned long body_start = buffer.find("\r\n\r\n");
	if (body_start == std::string::npos)
		throw StatusCodeException("No body found in request", BAD_REQUEST);
	body_start += strlen("\r\n\r\n");
	if (_transferEncoding == CHUNKED)
		parse_chunked_body(this, body_start, buffer);
	else
		parse_identity_body(this, body_start, buffer);
	auto it = _headers.find("content-type");
	if (it != _headers.end())
	{
		std::string content_type = it->second;
		if (content_type.find("multipart/form-data") != std::string::npos)
			parse_multipart_form_data(this);
	}
}