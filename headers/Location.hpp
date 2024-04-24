/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: felicia <felicia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 17:44:02 by felicia           #+#    #+#             */
/*   Updated: 2024/04/24 11:05:04 by felicia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
		bool				_isDefaultLocation;
		
	public:
		Location();
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