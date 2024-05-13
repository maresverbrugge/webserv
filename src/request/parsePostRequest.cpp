/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsePostRequest.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkoolhov <fkoolhov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 11:01:45 by fhuisman          #+#    #+#             */
/*   Updated: 2024/05/13 13:17:26 by fkoolhov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

static void parse_chunked_body(std::stringstream& stringstream, std::ofstream& outfile)
{
	std::string line;
	
	try
	{
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
			outfile.write(chunk.c_str(), chunk_size);
			std::getline(stringstream, line); // read the empty line after each chunk
		}
		outfile.close();
	}
	catch (const std::exception& exception)
	{
		outfile.close();
		throw (BAD_REQUEST); // bad request (for example if stoi fails)
	}
}

static void parse_identity_body(std::stringstream& stringstream, std::ofstream& outfile, int content_length)
{
	std::string line;
	
	try
	{
		std::string body;
		body.resize(content_length);
		stringstream.read(&body[0], content_length);
		outfile.write(body.c_str(), content_length);
    	outfile.close();
	}
	catch (const std::exception& exception)
	{
		outfile.close();
		throw (BAD_REQUEST); // Bad Request
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
            throw (LENGTH_REQUIRED); // Length Required
        else
            request->setContentLength(std::atoi(content_length.c_str()));
    }
    else request->setContentLength(-1);
}

static std::string get_file_extension(std::map<std::string, std::string> headers)
{
	std::string content_type = "";
	auto it = headers.find("content-type");
	if (it != headers.end())
		content_type = it->second;
	
	if (content_type == "text/html")
		return (".html");
	else if (content_type == "text/plain")
		return (".txt");
	else if (content_type == "image/jpeg")
		return (".jpeg");
	else if (content_type == "image/png")
		return (".png");
	else
		return (".bin");
}

static std::string get_unique_filename(std::map<std::string, std::string> headers)
{
	std::string filename;

	std::time_t current_time = std::time(nullptr);
	std::stringstream stringstream;
	stringstream << std::put_time(std::localtime(&current_time), "%d%m%Y%H%M%S");
	filename = stringstream.str() + get_file_extension(headers);
	return filename;
}

static std::string get_filename_from_header(Request *request)
{
	try
	{
		std::map<std::string, std::string> headers = request->getHeaders();
		std::string filename;
		auto it = headers.find("content-disposition");
		if (it == headers.end())
			return get_unique_filename(headers);
		std::string content_disposition = it->second;
		std::string::size_type pos = content_disposition.find("filename=");
		if (pos != std::string::npos)
		{
			std::string::size_type start_pos = pos + 10;
			std::string::size_type end_pos = content_disposition.find_first_of("\"'", start_pos);
			if (end_pos != std::string::npos)
				filename = content_disposition.substr(start_pos, end_pos - start_pos);
		}
		return filename;
	}
	catch (const std::exception& exception)
	{
		throw (BAD_REQUEST); // bad request
	}
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
	std::string filename = get_filename_from_header(this);
	filename = "./root/upload/" + filename; // we should get upload folder from config

	std::ofstream outfile(filename);
	if (!outfile.is_open())
		throw (INTERNAL_SERVER_ERROR); // Internal server error

	if (transfer_encoding == "chunked")
		parse_chunked_body(stringstream, outfile);
	else
		parse_identity_body(stringstream, outfile, this->_contentLength);
}