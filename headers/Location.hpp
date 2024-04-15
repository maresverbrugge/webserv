/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: felicia <felicia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 17:44:02 by felicia           #+#    #+#             */
/*   Updated: 2024/04/15 18:10:51 by felicia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
# define LOCATION_HPP

# include "webserv.hpp"

class Location
{
	private:
		std::array<bool, 3>	_allowedMethods;
		std::string			_path;
		std::string			_defaultPage;
		std::string			_redirectLink;
		std::string			_cgiExtension;
		std::string			_uploadFolder;
		bool				_directoryListing;
		
	public:
		Location();
		Location(const Location& src);
		~Location();
		Location& operator=(const Location& src);

		void	setAllowedMethods(std::array<bool, 3> allowedMethods);
		void	setPath(std::string path);
		void	setDefaultPage(std::string defaultPage);
		void	setRedirectLink(std::string redirectLink);
		void	setCgiExtension(std::string cgiExtension);
		void	setUploadFolder(std::string uploadFolder);
		void	setDirectoryListing(bool directoryListing);

		std::array<bool, 3>	getAllowedMethods();
		std::string			getPath();
		std::string			getDefaultPage();
		std::string			getRedirectLink();
		std::string			getCgiExtension();
		std::string			getUploadFolder();
		bool				getDirectoryListing();
};

#endif