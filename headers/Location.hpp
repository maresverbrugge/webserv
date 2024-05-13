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

#ifndef LOCATION_HPP
# define LOCATION_HPP

# include "webserv.hpp"

class Location
{
	private:
		std::string			_locationName;
		std::array<bool, 3>	_allowedMethods;
		std::string			_path;
		std::string			_defaultPage;
		std::string			_redirectLink;
		std::string			_cgiExtension;
		std::string			_uploadFolder;
		bool				_directoryListing;
		
	public:
		Location(std::string locationName,
					std::array<bool, 3> allowedMethods,
					std::string path,
					std::string defaultPage,
					std::string redirectLink,
					std::string cgiExtension,
					std::string uploadFolder,
					bool directoryListing);
		~Location();

		void	setLocationName(std::string locationName);
		void	setAllowedMethods(std::array<bool, 3> allowedMethods);
		void	setPath(std::string path);
		void	setDefaultPage(std::string defaultPage);
		void	setRedirectLink(std::string redirectLink);
		void	setCgiExtension(std::string cgiExtension);
		void	setUploadFolder(std::string uploadFolder);
		void	setDirectoryListing(bool directoryListing);
		void	setIsDefaultLocation(bool isDefaultLocation);

		std::string			getLocationName() const;
		std::array<bool, 3>	getAllowedMethods() const;
		std::string			getPath() const;
		std::string			getDefaultPage() const;
		std::string			getRedirectLink() const;
		std::string			getCgiExtension() const;
		std::string			getUploadFolder() const;
		bool				getDirectoryListing() const;
		bool				getIsDefaultLocation() const;
};

std::ostream& operator<<(std::ostream& out_stream, const Location& location);

#endif