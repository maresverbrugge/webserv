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

static std::string getServerErrorReasonPhrase(short status_code)
{
	switch (status_code)
	{
		case INTERNAL_SERVER_ERROR:
			return ("Internal Server Error");
		case NOT_IMPLEMENTED:
			return ("Not Implemented");
		case BAD_GATEWAY:
			return ("Bad Gateway");
		case SERVICE_UNAVAILABLE:
			return ("Service Unavailable");
		case GATEWAY_TIMEOUT:
			return ("Gateway Time-out");
		case HTTP_VERSION_NOT_SUPPORTED:
			return ("HTTP Version not supported");
		default:
			return ("Unknown Status Code");
	}
}

static std::string getClientErrorReasonPhrase(short status_code)
{
	switch (status_code)
	{
		case BAD_REQUEST:
			return ("Bad Request");
		case UNAUTHORIZED:
			return ("Unauthorized");
		case PAYMENT_REQUIRED:
			return ("Payment Required");
		case FORBIDDEN:	
			return ("Forbidden");
		case NOT_FOUND:
			return ("Not Found");
		case METHOD_NOT_ALLOWED:
			return ("Method Not Allowed");
		case NOT_ACCEPTABLE:
			return ("Not Acceptable");
		case PROXY_AUTHENTICATION_REQUIRED:
			return ("Proxy Authentication Required");
		case REQUEST_TIMEOUT:
			return ("Request Time-out");
		case CONFLICT:
			return ("Conflict");
		case GONE:
			return ("Gone");
		case LENGTH_REQUIRED:
			return ("Length Required");
		case PRECONDITION_FAILED:
			return ("Precondition Failed");
		case CONTENT_TOO_LARGE:
			return ("Request Entity Too Large");
		case URI_TOO_LARGE:
			return ("Request-URI Too Large");
		case UNSUPPORTED_MEDIA_TYPE:
			return ("Unsupported Media Type");
		case REQUESTED_RANGE_NOT_SATISFIABLE:
			return ("Requested range not satisfiable");
		case EXPECTATION_FAILED:
			return ("Expectation Failed");
		default:
			return ("Unknown Status Code");
	}
}

static std::string getRedirectionReasonPhrase(short status_code)
{
	switch (status_code)
	{
		case MULTIPLE_CHOICES:
			return ("Multiple Choices");
		case MOVED_PERMANENTLY:
			return ("Moved Permanently");
		case FOUND:
			return ("Found");
		case SEE_OTHER:
			return ("See Other");
		case NOT_MODIFIED:
			return ("Not Modified");
		case USE_PROXY:
			return ("Use Proxy");
		case TEMPORARY_REDIRECT:
			return ("Temporary Redirect");
		default:
			return ("Unknown Status Code");
	}
}

static std::string getSuccessfulReasonPhrase(short status_code)
{
	switch (status_code)
	{
		case OK:
			return ("OK");
		case CREATED:
			return ("Created");
		case ACCEPTED:
			return ("Accepted");
		case NON_AUTHORITATIVE_INFORMATION:
			return ("Non-Authoritative Information");
		case NO_CONTENT:
			return ("No Content");
		case RESET_CONTENT:
			return ("Reset Content");
		case PARTIAL_CONTENT:
			return ("Partial Content");
		default:
			return ("Unknown Status Code");
	}
}

static std::string getInformationalReasonPhrase(short status_code)
{
	switch (status_code)
	{
		case CONTINUE:
			return ("Continue");
		case SWITCHING_PROTOCOLS:
			return ("Switching Protocols");
		default:
			return ("Unknown Status Code");
	}
}

std::string getReasonPhrase(short status_code)
{
	if (status_code >= 100 && status_code < 200)
		return (getInformationalReasonPhrase(status_code));
	else if (status_code >= 200 && status_code < 300)
		return (getSuccessfulReasonPhrase(status_code));
	else if (status_code >= 300 && status_code < 400)
		return (getRedirectionReasonPhrase(status_code));
	else if (status_code >= 400 && status_code < 500)
		return (getClientErrorReasonPhrase(status_code));
	else if (status_code >= 500 && status_code < 600)
		return (getServerErrorReasonPhrase(status_code));
	else
		return ("Unknown Status Code");
}

std::string constructBodyFromFile(std::string pathToFile)
{
	std::ifstream file(pathToFile, std::ios::binary);
	if (file.fail())
		throw StatusCodeException("Couldn't open specified file", NOT_FOUND);

	file.seekg(0, std::ios::end);
	std::streamsize size = file.tellg();
	file.seekg(0, std::ios::beg);

	std::vector<char> binary_buffer(size);
	if (!file.read(binary_buffer.data(), size))
	{
		file.close();
		throw StatusCodeException("Couldn't read file", NOT_FOUND);
	}
	file.close();
	std::string buffer(binary_buffer.begin(), binary_buffer.end());
	return buffer;
}

std::string getContentType(std::string extension)
{
	std::map<std::string, std::string> contentTypes = {{".html", "text/html"}, {".css", "text/css"}, {".js", "application/javascript"}, {".json", "application/json"}, {".jpg", "image/jpeg"}, {".jpeg", "image/jpeg"}, {".png", "image/png"}, {".gif", "image/gif"}, {".pdf", "application/pdf"}, {".txt", "text/plain"}, {".xml", "application/xml"}};

	auto it = contentTypes.find(extension);
	if (it == contentTypes.end())
		return ("application/octet-stream");
	return ((*it).second);
}
