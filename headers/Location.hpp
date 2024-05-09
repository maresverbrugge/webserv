/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkoolhov <fkoolhov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 17:44:02 by felicia           #+#    #+#             */
/*   Updated: 2024/05/09 16:00:32 by fkoolhov         ###   ########.fr       */
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
		bool				_directoryListing;
		
	public:
		Location(std::string locationName,
					std::array<bool, 3> allowedMethods,
					std::string path,
					std::string defaultPage,
					std::string redirectLink,
					std::string cgiExtension,
					bool directoryListing);
		~Location();

		void	setLocationName(std::string locationName);
		void	setAllowedMethods(std::array<bool, 3> allowedMethods);
		void	setPath(std::string path);
		void	setDefaultPage(std::string defaultPage);
		void	setRedirectLink(std::string redirectLink);
		void	setCgiExtension(std::string cgiExtension);
		void	setDirectoryListing(bool directoryListing);
		void	setIsDefaultLocation(bool isDefaultLocation);

		std::string			getLocationName() const;
		std::array<bool, 3>	getAllowedMethods() const;
		std::string			getPath() const;
		std::string			getDefaultPage() const;
		std::string			getRedirectLink() const;
		std::string			getCgiExtension() const;
		bool				getDirectoryListing() const;
		bool				getIsDefaultLocation() const;
};

std::ostream& operator<<(std::ostream& out_stream, const Location& location);

#endif