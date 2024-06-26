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

void Request::findContentLength()
{
	auto it = _headers.find("content-length");
	if (it != _headers.end())
	{
		if (!it->second.empty())
			_contentLength = std::stoll(it->second.c_str());
	}
	else if (_transferEncoding == IDENTITY)
		throw StatusCodeException("Content-length not specified", LENGTH_REQUIRED);    
}

void Request::verifyTransferEncoding()
{
	std::string transfer_encoding_str;
	auto it = _headers.find("transfer-encoding");
	if (it != _headers.end())
		transfer_encoding_str = it->second;
	str_to_lower(transfer_encoding_str);
	if (transfer_encoding_str == "identity")
		_transferEncoding = IDENTITY;
	else if (transfer_encoding_str == "chunked")
		_transferEncoding = CHUNKED;
	else if (transfer_encoding_str.empty())
	{
		if (_method == POST)
			_transferEncoding = IDENTITY;
	}
	else
		throw StatusCodeException("Invalid transfer encoding", NOT_IMPLEMENTED);
}

static std::string look_for_header_continuation(std::stringstream &ss, std::string &header_line)
{
	std::string next_line;
	
	std::getline(ss, next_line);
	auto it = next_line.begin();
	while (*it == ' ' || *it == '\t')
	{
		trim_cr(next_line);
		trim_lws(next_line);
		header_line += next_line;
		std::getline(ss, next_line);
		it = next_line.begin();
	}
	return next_line;
}

static void add_headers(Request *request, std::stringstream &ss)
{
	std::string header_line;
	std::string next_line;

	std::getline(ss, header_line);
	while (header_line != "" && header_line != "\r")
	{
		std::string header_name;
		std::string header_value;
		size_t semicolon;

		trim_cr(header_line);
		next_line = look_for_header_continuation(ss, header_line);
		semicolon = header_line.find_first_of(':');
		if (semicolon == std::string::npos)
			throw StatusCodeException("No header seperator", BAD_REQUEST);
		header_name = header_line.substr(0, semicolon);
		header_value = header_line.substr(semicolon + 1, header_line.size() - semicolon);
		trim_lws(header_value);
		header_name = decodePercentEncodedString(header_name);
		header_value = decodePercentEncodedString(header_value);
		str_to_lower(header_name);
		request->setHeader(header_name, header_value);
		header_line = next_line;
	}
}

static void parse_request_line(std::stringstream& ss, Request *request)
{
	std::string request_line;
	std::string method;
	std::string uri;
	std::string protocol;

	std::getline(ss, request_line);
	trim_cr(request_line);
	size_t sp1 = request_line.find(' ');
	size_t sp2 = request_line.find(' ', sp1 + 1);
	method = request_line.substr(0, sp1);
	protocol = request_line.substr(sp2 + 1, request_line.size() - sp2 - 1);
	if (sp1 == std::string::npos || sp2 == std::string::npos || !is_valid_method(method) || !is_http_protocol(protocol))
		throw StatusCodeException("Request line error", BAD_REQUEST);
	if (!is_http1_1_protocol(protocol))
		throw StatusCodeException("Invalid HTTP version", HTTP_VERSION_NOT_SUPPORTED);
	uri = request_line.substr(sp1 + 1, sp2 - sp1 - 1);
	request->setMethod(method);
	request->setUri(uri);
}

Request::Request(std::string request) : _port(-1), _contentLength(0), _transferEncoding(UNDEFINED)
{
	std::stringstream ss(request);

	parse_request_line(ss, this);
	add_headers(this, ss);
	parseURI(_uri);
	verifyTransferEncoding();
	findContentLength();
}

Request::~Request() {}

int Request::getMethod() const
{
	return _method;
}

std::string Request::getUri() const
{
	return _uri;
}

std::string Request::getHost() const
{
	return _host;
}

std::string Request::getPath() const
{
	return _path;
}

std::vector<std::string> Request::getQuery() const
{
	return _query;
}

std::string Request::getFragmentIdentifier() const
{
	return _fragmentIdentifier;
}

std::map<std::string, std::string> Request::getHeaders() const
{
	return _headers;
}

std::vector<char> Request::getBody() const
{
	return _body;
}

int Request::getPort() const
{
	return _port;
}

unsigned long long Request::getContentLength() const
{
	return _contentLength;
}

e_transfer_encoding Request::getTransferEncoding() const
{
	return _transferEncoding;
}

void Request::setMethod(std::string method)
{
	if (method == "GET")
		_method = GET;
	else if (method == "DELETE")
		_method = DELETE;
	else if (method == "POST")
		_method = POST;
}

void Request::setUri(std::string uri)
{
	_uri = uri;
}

void Request::setHost(std::string host)
{
	_host = host;
}

void Request::setPath(std::string path)
{
	_path = path;
}

void Request::setQuery(std::vector<std::string> query)
{
	_query = query;
}

void Request::setFragmentIdentifier(std::string fragmentIdentifier)
{
	_fragmentIdentifier = fragmentIdentifier;
}

void Request::setHeader(std::string headerName, std::string headerValue)
{
	auto it = _headers.find(headerName);
	if (it != _headers.end())
		it->second += ", " + headerValue;
	else
		_headers[headerName] = headerValue;
}

void Request::setBody(std::vector<char> body)
{
	_body = body;
}

void Request::setPort(int port)
{
	if (port == ERROR)
		_port = 80;
	else
		_port = port;
}

void Request::setContentLength(unsigned long long contentLength)
{
	_contentLength = contentLength;
}

void Request::setTransferEndocing(e_transfer_encoding transferEncoding)
{
    _transferEncoding = transferEncoding;
}

void Request::parseBody(std::string full_request, unsigned long long client_max_body_size)
{
	if (_method == POST)
	{
		parsePostRequest(full_request);
		if (_body.size() > client_max_body_size)
			throw StatusCodeException("Body too large", CONTENT_TOO_LARGE);
	}
}

std::ostream &operator<<(std::ostream &os, const Request &request)
{
	std::cout << YELLOW BOLD "\nRequest:" RESET << std::endl;
	int method = request.getMethod();
	if (method == GET)
		os << "GET";
	else if (method == DELETE)
		os << "DELETE";
	else if (method == POST)
		os << "POST";
	os << " " << request.getHost() << ":" << request.getPort() << request.getPath();
	if (!request.getQuery().empty())
	{
		for (auto str : request.getQuery())
		{
			if (str == request.getQuery().front() && !str.empty())
				os << '?';
			os << str;
			if (str != request.getQuery().back())
			os << '&';
		}
	}
	if (request.getFragmentIdentifier() != "")
		os << "#" << request.getFragmentIdentifier();
	os << " HTTP/1.1" << std::endl;
	for (auto it : request.getHeaders())
		os << it.first << ": " << it.second << std::endl;
	os << std::endl;

	std::cout << "Body:" << std::endl;
	for (unsigned long i = 0; i < request.getBody().size(); i++)
	{
		os << request.getBody()[i];
	}
	std::cout << std::endl;

	if (request.getContentLength() == 0 && request.getMethod() == POST)
		os << "This request is encoded" << std::endl;
	else
		os << "Content-Length: " << request.getContentLength() << std::endl;
	return os;
}