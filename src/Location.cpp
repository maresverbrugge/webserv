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

#include "Location.hpp"

Location::Location(std::string locationName, std::array<bool, 3> allowedMethods, std::string path, std::string defaultPage, std::string redirectLink, std::string cgiExtension, std::string uploadFolder, bool directoryListing)
	: _locationName(locationName),
	  _allowedMethods(allowedMethods),
	  _path(path),
	  _defaultPage(defaultPage),
	  _redirectLink(redirectLink),
	  _cgiExtension(cgiExtension),
	  _uploadFolder(uploadFolder),
	  _directoryListing(directoryListing)
{
	std::cout << "Location constructor called" << std::endl;
}

Location::~Location()
{
	std::cout << "Location destructor called" << std::endl;
}

void Location::setLocationName(std::string locationName)
{
	_locationName = locationName;
}

void Location::setAllowedMethods(std::array<bool, 3> allowedMethods)
{
	_allowedMethods = allowedMethods;
}

void Location::setPath(std::string path)
{
	_path = path;
}

void Location::setDefaultPage(std::string defaultPage)
{
	_defaultPage = defaultPage;
}

void Location::setRedirectLink(std::string redirectLink)
{
	_redirectLink = redirectLink;
}

void Location::setCgiExtension(std::string cgiExtension)
{
	_cgiExtension = cgiExtension;
}

void Location::setUploadFolder(std::string uploadFolder)
{
	_uploadFolder = uploadFolder;
}

void Location::setDirectoryListing(bool directoryListing)
{
	_directoryListing = directoryListing;
}

std::string Location::getLocationName() const
{
	return _locationName;
}

std::array<bool, 3> Location::getAllowedMethods() const
{
	return _allowedMethods;
}

std::string Location::getPath() const
{
	return _path;
}

std::string Location::getDefaultPage() const
{
	return _defaultPage;
}

std::string Location::getRedirectLink() const
{
	return _redirectLink;
}

std::string Location::getCgiExtension() const
{
	return _cgiExtension;
}

std::string Location::getUploadFolder() const
{
	return _uploadFolder;
}

bool Location::getDirectoryListing() const
{
	return _directoryListing;
}

std::ostream& operator<<(std::ostream& out_stream, const Location& location)
{
	out_stream << YELLOW BOLD "Location: " RESET << location.getLocationName() << std::endl;
	out_stream << "_allowedMethods: ";
	std::array<bool, 3> allowedMethods = location.getAllowedMethods();
	for (bool method : allowedMethods)
		out_stream << method << " ";
	out_stream << std::endl;
	out_stream << "_path: " << location.getPath() << std::endl;
	out_stream << "_defaultPage: " << location.getDefaultPage() << std::endl;
	out_stream << "_redirectLink: " << location.getRedirectLink() << std::endl;
	out_stream << "_cgiExtension: " << location.getCgiExtension() << std::endl;
	out_stream << "_uploadFolder: " << location.getUploadFolder() << std::endl;
	out_stream << "_directoryListing: " << location.getDirectoryListing() << std::endl;
	return out_stream;
}