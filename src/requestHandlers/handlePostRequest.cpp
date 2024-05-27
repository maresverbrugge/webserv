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

static std::string get_unique_filename()
{
	std::string filename;

	std::time_t current_time = std::time(nullptr);
	std::stringstream stringstream;
	stringstream << std::put_time(std::localtime(&current_time), "%d%m%Y%H%M%S");
	filename = stringstream.str() + ".bin"; // Should get extension from content-type header or too much effort?
	return filename;
}

static std::string get_filename_from_header(std::map<std::string, std::string> headers)
{
	try
	{
		std::string filename;
		auto it = headers.find("content-disposition");
		if (it == headers.end())
			return get_unique_filename();
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
		throw_error("Couldn't get filename", BAD_REQUEST);
		return "";
	}
}

void RequestHandler::handlePostRequest()
{
	std::cout << BLUE BOLD "Handling POST request" RESET << std::endl;
	Request& request = this->getRequest();
	std::string filename = get_filename_from_header(request.getHeaders());
	std::string upload_folder = _location.getUploadFolder();
	if (upload_folder.length() == 0)
		throw_error("No upload folder specified on this location", INTERNAL_SERVER_ERROR);
	filename = upload_folder + "/" + filename;

	std::ofstream outfile(filename);
	if (!std::filesystem::exists(filename))
		throw_error("Couldn't create " + filename, INTERNAL_SERVER_ERROR);
	if (!outfile.is_open())
		throw_error("Couldn't open " + filename + " for writing", INTERNAL_SERVER_ERROR);

    outfile.write(request.getBody().data(), request.getBody().size());
    outfile.close();
}